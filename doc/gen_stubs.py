#!/usr/bin/env python3
"""
Boost.Python stub generator.

Generates typed .pyi stubs by parsing the structured docstring signatures
emitted by Boost.Python — much cleaner than mypy stubgen for this use case.

Improvements over `mypy stubgen`:
  - Instance methods are NOT decorated with @classmethod
  - Parameter types extracted from Boost.Python docstrings
  - Return types extracted from Boost.Python docstrings
  - Overloaded constructors/methods use @overload with per-overload docstrings
  - Enums derive from int instead of Boost.Python.enum
  - Read-only properties typed from their getter docstring
  - No Incomplete / no Boost.Python base classes

Usage:
    python doc/gen_stubs.py ikomia.core.pycore > ikomia/core/pycore.pyi
"""
from __future__ import annotations

import importlib
import inspect
import re
import sys
import textwrap
from typing import NamedTuple

# ---------------------------------------------------------------------------
# Type mapping
# ---------------------------------------------------------------------------
_PRIM: dict[str, str] = {
    "int": "int",
    "long": "int",
    "size_t": "int",
    "float": "float",
    "double": "float",
    "str": "str",
    "bool": "bool",
    "None": "None",
    # Boost.Python uses 'object' for generic Python objects (list, dict, …)
    "object": "Any",
    "dict": "dict[str, Any]",
    "tuple": "tuple[Any, ...]",
}


def _map(t: str) -> str:
    return _PRIM.get(t.strip(), t.strip())


# ---------------------------------------------------------------------------
# Docstring helpers
# ---------------------------------------------------------------------------

def _clean_description(text: str) -> str:
    """Dedent and strip a raw description block from a Boost.Python docstring."""
    lines = text.splitlines()
    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()
    if not lines:
        return ""
    return textwrap.dedent("\n".join(lines)).strip()


def _emit_docstring(text: str, indent: str) -> list[str]:
    """Render `text` as a Python docstring block at the given indent level.

    Returns an empty list when text is empty (caller uses `: ...` instead).
    """
    if not text:
        return []
    doc_lines = text.splitlines()
    if len(doc_lines) == 1:
        return [f'{indent}"""{text}"""']
    result = [f'{indent}"""{doc_lines[0]}']
    for line in doc_lines[1:]:
        result.append(f"{indent}{line}" if line.strip() else "")
    result.append(f'{indent}"""')
    return result


# ---------------------------------------------------------------------------
# Signature parsing
# ---------------------------------------------------------------------------
_PARAM_RE = re.compile(r"\((\w+)\)([\w]+)")


class Overload(NamedTuple):
    params: list[tuple[str, str]]  # [(py_type, name), ...]
    ret: str
    is_static: bool
    description: str  # human-readable text extracted from the docstring block


def _extract_blocks(fname: str, doc: str) -> list[tuple[str, str]]:
    """Split a Boost.Python docstring into (signature_line, body_text) blocks.

    Each block starts on a line beginning with ``fname(``.  The body is
    everything between that line and the next signature line.
    """
    blocks: list[tuple[str, str]] = []
    current_sig: str | None = None
    current_body: list[str] = []

    for line in doc.splitlines():
        if line.strip().startswith(f"{fname}("):
            if current_sig is not None:
                blocks.append((current_sig, "\n".join(current_body)))
            current_sig = line.strip()
            current_body = []
        elif current_sig is not None:
            current_body.append(line)

    if current_sig is not None:
        blocks.append((current_sig, "\n".join(current_body)))

    return blocks


def _parse_sig_line(
    fname: str, sig_line: str, cls_name: str
) -> tuple[list[tuple[str, str]], str, bool] | None:
    """Parse one Boost.Python signature line.

    Returns (params, ret_type, is_static) or None if the line cannot be parsed.
    """
    # Zero-param case: fname() -> ret
    m0 = re.match(
        rf"^\s*{re.escape(fname)}\s*\(\s*\)\s*(?:->\s*(.+?))?(?:\s*:|$)",
        sig_line,
    )
    if m0:
        ret = _map((m0.group(1) or "None").strip().rstrip(" :"))
        return [], ret, True

    # General case — greedy (.+) captures up to the last ) before -> or :
    m = re.match(
        rf"^\s*{re.escape(fname)}\s*\((.+)\)\s*(?:->\s*(.+?))?(?:\s*:|$)",
        sig_line,
    )
    if not m:
        return None

    params_raw = m.group(1) or ""
    ret_raw = _map((m.group(2) or "None").strip().rstrip(" :"))
    raw_params = _PARAM_RE.findall(params_raw)

    if not raw_params:
        return [], ret_raw, True

    first_type, first_name = raw_params[0]
    # Instance-method heuristic:
    #   • type is 'object'  → always a self placeholder in Boost.Python
    #   • name is 'self'    → explicit self
    #   • name is 'arg1' and type matches the class name → self placeholder
    is_instance = (
        first_type == "object"
        or first_name == "self"
        or (first_name == "arg1" and first_type in ("object", cls_name))
    )
    rest = raw_params[1:] if is_instance else raw_params
    typed = [(_map(t), n) for t, n in rest]
    return typed, ret_raw, not is_instance


def _parse_overloads(fname: str, doc: str | None, cls_name: str = "") -> list[Overload]:
    """Extract typed overloads (with descriptions) from a Boost.Python docstring."""
    if not doc:
        return []

    overloads: list[Overload] = []
    for sig_line, body in _extract_blocks(fname, doc):
        result = _parse_sig_line(fname, sig_line, cls_name)
        if result is None:
            continue
        params, ret, is_static = result
        description = _clean_description(body)
        overloads.append(Overload(params, ret, is_static, description))

    return _dedup_overloads(overloads)


def _dedup_overloads(overloads: list[Overload]) -> list[Overload]:
    """Deduplicate overloads sharing the same parameter signature.

    Keeps the variant with the most specific return type; when return types
    are equal, keeps the one that carries a description.
    """
    by_params: dict[str, Overload] = {}
    for o in overloads:
        key = repr((o.params, o.is_static))
        if key not in by_params:
            by_params[key] = o
        else:
            existing = by_params[key]
            # Prefer a more specific (non-None/Any) return type
            better_ret = existing.ret in ("None", "Any") and o.ret not in ("None", "Any")
            # Prefer a variant that has a description
            has_doc = not existing.description and o.description
            if better_ret or (not better_ret and has_doc):
                by_params[key] = o
    return list(by_params.values())


# ---------------------------------------------------------------------------
# Property helpers
# ---------------------------------------------------------------------------

def _prop_ret_type(prop: property) -> str:
    """Infer the return type of a property from its getter docstring."""
    doc = getattr(prop.fget, "__doc__", None) or ""
    m = re.search(r"->\s*(\S+)", doc)
    return _map(m.group(1).rstrip(":")) if m else "Any"


def _prop_description(prop: property) -> str:
    """Extract the human-readable part of a property's docstring."""
    doc = getattr(prop, "__doc__", None) or getattr(prop.fget, "__doc__", None) or ""
    # Strip the leading Boost.Python signature line if present
    lines = doc.splitlines()
    body: list[str] = []
    in_body = False
    for line in lines:
        stripped = line.strip()
        # A signature line contains '(' and ')'  and '->'
        if not in_body and re.match(r"^\w+\(.*\)\s*(?:->.+)?(?:\s*:|$)", stripped):
            in_body = True
            continue
        if in_body:
            body.append(line)
    return _clean_description("\n".join(body)) if body else _clean_description(doc)


# ---------------------------------------------------------------------------
# Enum detection
# ---------------------------------------------------------------------------

def _is_boost_enum(cls: type) -> bool:
    return (
        isinstance(getattr(cls, "names", None), dict)
        and isinstance(getattr(cls, "values", None), dict)
    )


def _enum_members(cls: type) -> list[str]:
    skip = {"names", "values"}
    return [
        k for k in vars(cls)
        if not k.startswith("_")
        and k not in skip
        and isinstance(getattr(cls, k, None), cls)
    ]


# ---------------------------------------------------------------------------
# Code generation
# ---------------------------------------------------------------------------
_I = "    "  # one indent level

_SKIP_DUNDER = frozenset({
    "__class__", "__dict__", "__doc__", "__module__", "__weakref__",
    "__reduce__", "__reduce_ex__", "__sizeof__", "__subclasshook__",
    "__instance_size__", "__getstate_manages_dict__", "__safe_for_unpickling__",
    "__getstate__", "__setstate__",
})

_KEEP_DUNDER = frozenset({
    "__init__", "__repr__", "__str__",
    "__eq__", "__ne__", "__lt__", "__le__", "__gt__", "__ge__",
    "__hash__", "__len__", "__getitem__", "__setitem__", "__iter__",
    "__contains__", "__bool__", "__int__", "__float__",
    "__add__", "__radd__", "__mul__", "__rmul__",
    "__getinitargs__",
})


def _fmt_params(o: Overload) -> str:
    return ", ".join(f"{n}: {t}" for t, n in o.params)


def _emit_method(fname: str, params: str, ret: str, is_static: bool, description: str) -> list[str]:
    """Emit one method definition (possibly with docstring)."""
    if is_static:
        sig = f"{_I}def {fname}({params}) -> {ret}:"
        prefix = [f"{_I}@staticmethod"]
    else:
        sep = ", " if params else ""
        sig = f"{_I}def {fname}(self{sep}{params}) -> {ret}:"
        prefix = []

    doc_lines = _emit_docstring(description, indent=_I * 2)
    if doc_lines:
        return prefix + [sig] + doc_lines
    return prefix + [sig.rstrip(":") + ": ..."]


def _gen_method(fname: str, obj: object, cls_name: str = "") -> list[str]:
    overloads = _parse_overloads(fname, inspect.getdoc(obj), cls_name)

    if not overloads:
        return [f"{_I}def {fname}(self, *args: Any, **kwargs: Any) -> Any: ..."]

    all_static = all(o.is_static for o in overloads)
    lines: list[str] = []

    if len(overloads) == 1:
        o = overloads[0]
        lines.extend(_emit_method(fname, _fmt_params(o), o.ret, all_static, o.description))
    else:
        for o in overloads:
            params = _fmt_params(o)
            if all_static:
                overload_prefix = [f"{_I}@staticmethod", f"{_I}@overload"]
            else:
                overload_prefix = [f"{_I}@overload"]
            lines.extend(overload_prefix)
            lines.extend(_emit_method(fname, params, o.ret, all_static, o.description))

    return lines


def _gen_enum(name: str, cls: type) -> list[str]:
    lines = [f"class {name}(int):"]
    doc = inspect.getdoc(cls)
    if doc:
        lines += _emit_docstring(doc, indent=_I) or []
    # Boost.Python adds these two dicts to every enum class
    lines.append(f"{_I}names: ClassVar[dict[str, {name}]]")
    lines.append(f"{_I}values: ClassVar[dict[int, {name}]]")
    members = _enum_members(cls)
    lines += [f"{_I}{m}: ClassVar[{name}]" for m in members] or [f"{_I}..."]
    lines.append("")
    return lines


def _gen_class(name: str, cls: type, mod_name: str) -> list[str]:
    if _is_boost_enum(cls):
        return _gen_enum(name, cls)

    bases = [
        b.__name__
        for b in cls.__bases__
        if b.__name__ not in ("instance", "object")
        and "Boost" not in str(getattr(b, "__module__", ""))
    ]
    head = f"class {name}({', '.join(bases)})" if bases else f"class {name}"
    lines = [f"{head}:"]

    doc = inspect.getdoc(cls)
    if doc:
        lines += _emit_docstring(doc, indent=_I) or []

    body: list[str] = []

    # ── Properties ────────────────────────────────────────────────────────
    for attr in sorted(dir(cls)):
        if attr.startswith("_"):
            continue
        raw = inspect.getattr_static(cls, attr, None)
        if not isinstance(raw, property):
            continue
        t = _prop_ret_type(raw)
        description = _prop_description(raw)
        if raw.fset is None:
            body.append(f"{_I}@property")
            body.extend(_emit_method(attr, "", t, is_static=False, description=description))
        else:
            # Read-write: emit as typed attribute; docstring goes as a comment
            body.append(f"{_I}{attr}: {t}")

    # ── Methods ───────────────────────────────────────────────────────────
    emitted: set[str] = set()
    for attr, val in sorted(inspect.getmembers(cls)):
        if attr in emitted:
            continue
        if isinstance(inspect.getattr_static(cls, attr, None), property):
            emitted.add(attr)
            continue
        if attr.startswith("__"):
            if attr not in _KEEP_DUNDER or attr in _SKIP_DUNDER:
                emitted.add(attr)
                continue
        if callable(val):
            emitted.add(attr)
            body.extend(_gen_method(attr, val, cls_name=name))

    lines.extend(body or [f"{_I}..."])
    lines.append("")
    return lines


# ---------------------------------------------------------------------------
# Module entry point
# ---------------------------------------------------------------------------

def gen_stubs(mod_name: str) -> str:
    """Return the full .pyi content for *mod_name* as a string."""
    mod = importlib.import_module(mod_name)
    lines = [
        "# Auto-generated — do not edit manually.",
        f"# Regenerate: python doc/gen_stubs.py -m {mod_name} [-o <dir>]",
        "from __future__ import annotations",
        "from typing import Any, ClassVar, overload",
        "",
    ]
    for obj_name in sorted(dir(mod)):
        if obj_name.startswith("_"):
            continue
        obj = getattr(mod, obj_name)
        if inspect.isclass(obj) and getattr(obj, "__module__", "") == mod_name:
            lines.extend(_gen_class(obj_name, obj, mod_name))
    return "\n".join(lines)


def _output_path(mod_name: str, output_dir: str) -> "Path":
    from pathlib import Path
    return Path(output_dir).joinpath(*mod_name.split(".")).with_suffix(".pyi")


def main() -> int:
    import argparse
    from pathlib import Path

    parser = argparse.ArgumentParser(
        description="Generate typed .pyi stubs for Boost.Python extension modules.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Examples:\n"
            "  # write to stdout\n"
            "  python doc/gen_stubs.py -m ikomia.core.pycore\n\n"
            "  # write ikomia/core/pycore.pyi under ./out/\n"
            "  python doc/gen_stubs.py -m ikomia.core.pycore -o out\n\n"
            "  # multiple modules at once\n"
            "  python doc/gen_stubs.py -m ikomia.core.pycore -m ikomia.dataprocess.pydataprocess -o out\n"
        ),
    )
    parser.add_argument(
        "-m", "--module",
        action="append",
        dest="modules",
        default=[],
        metavar="MODULE",
        help="Module to process (dotted name). May be repeated.",
    )
    parser.add_argument(
        "-o", "--output-dir",
        default=None,
        metavar="DIR",
        help=(
            "Root directory for .pyi output. "
            "The file path mirrors the module hierarchy "
            "(e.g. ikomia.core.pycore → <DIR>/ikomia/core/pycore.pyi). "
            "Omit to write to stdout."
        ),
    )
    # Positional fallback for backward-compat: gen_stubs.py ikomia.core.pycore
    parser.add_argument(
        "positional",
        nargs="*",
        metavar="MODULE",
        help=argparse.SUPPRESS,
    )

    args = parser.parse_args()
    modules: list[str] = args.modules + args.positional
    if not modules:
        parser.error("Specify at least one module with -m/--module.")

    for mod_name in modules:
        content = gen_stubs(mod_name)
        if args.output_dir:
            out = _output_path(mod_name, args.output_dir)
            out.parent.mkdir(parents=True, exist_ok=True)
            out.write_text(content, encoding="utf-8")
            print(f"Wrote {out}", file=sys.stderr)
        else:
            print(content)

    return 0


if __name__ == "__main__":
    sys.exit(main())
