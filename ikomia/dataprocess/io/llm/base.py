# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""LLM shared base classes."""

from dataclasses import MISSING, asdict, dataclass, field, fields
from types import UnionType
from typing import Any, Literal, Union, get_args, get_origin


class LlmObject:
    """Base class for serializable LLM dataclasses."""

    def to_dict(self) -> dict[str, Any]:
        """Convert to a dictionary."""
        return asdict(self)

    @classmethod
    def from_dict(cls, data: dict[str, Any]):
        """Build an instance from a dictionary."""
        init_kwargs = {}
        for f in fields(cls):
            if f.name in data:
                init_kwargs[f.name] = cls._deserialize_value(data[f.name], f.type)
            elif f.default is MISSING and f.default_factory is MISSING:
                raise ValueError(
                    f"Unable to deserialize {cls.__name__}: "
                    f"missing field '{f.name}'."
                )

        return cls(**init_kwargs)

    @classmethod
    def _deserialize_value(cls, value: Any, annotation: Any) -> Any:
        """Deserialize a field value.

        Args:
            value: The value to deserialize.
            annotation: The type annotation of the field.

        Returns:
            The deserialized value.
        """
        if value is None:
            return None

        # Retrieve the original type from the annotation.
        origin = get_origin(annotation)

        # If the field expects a list, deserialize each list item.
        if origin is list:
            inner_annotation = get_args(annotation)[0]
            return [cls._deserialize_value(item, inner_annotation) for item in value]

        # If the field expects a union
        if origin in (Union, UnionType):
            # Retrieve possible union types (excluding None)
            union_args = [arg for arg in get_args(annotation) if arg is not type(None)]

            # If there is one non-None union type (eg: LlmObject | None), deserialize the value against that type.
            if len(union_args) == 1:
                return cls._deserialize_value(value, union_args[0])

            # If one of the union variants is a list type and the payload is a list,
            # deserialize against that list annotation so nested LLM objects are restored.
            if isinstance(value, list):
                list_arg = next(
                    (arg for arg in union_args if get_origin(arg) is list),
                    None,
                )
                if list_arg is not None:
                    return cls._deserialize_value(value, list_arg)

            # Otherwise, if the value is a dictionary and contains a "type" field, it looks like a LLM object.
            # We try to match the value against each union type.
            if isinstance(value, dict) and "type" in value:
                for union_arg in union_args:
                    if not isinstance(union_arg, type) or not issubclass(
                        union_arg, LlmObject
                    ):
                        continue
                    if union_arg.matches_raw(value):
                        return union_arg.from_dict(value)

            # If no match is found, return the value as is.
            return value

        # If the field expects a LLM object, deserialize the value against that type.
        if (
            isinstance(annotation, type)
            and issubclass(annotation, LlmObject)
            and isinstance(value, dict)
        ):
            return annotation.from_dict(value)

        return value

    @classmethod
    def matches_raw(cls, raw: dict[str, Any]) -> bool:
        """Whether the raw dictionary is a serialization of the dataclass."""
        # By default, we identify a match if the raw dictionary contains all
        # literal-typed fields of the dataclass with their default values.
        # This allows for matching {type: "message", role: "assistant", content: [...]} against LlmAssistantMessage.
        return all(
            raw.get(f.name) == f.default
            for f in fields(cls)
            if f.default is not MISSING and get_origin(f.type) is Literal
        )


@dataclass(kw_only=True)
class LlmBaseItem(LlmObject):
    """Base class for LLM context items."""

    type: str
    id: str | None = None
    extras: dict[str, Any] = field(default_factory=dict)

    def to_dict(self) -> dict[str, Any]:
        """
        Convert to a flat dictionary.

        Extras are merged into the top-level dict so the output matches
        the original API format.
        """
        data = asdict(self)
        extras = data.pop("extras", {})
        if data.get("id") is None:
            data.pop("id")
        data.update(extras)
        return data

    @classmethod
    def from_dict(cls, data: dict[str, Any]):
        known_names = {f.name for f in fields(cls)}
        unknown = {k: v for k, v in data.items() if k not in known_names}
        extras = {**data.get("extras", {}), **unknown}
        clean = {k: v for k, v in data.items() if k in known_names}
        clean["extras"] = extras
        return super().from_dict(clean)


@dataclass(kw_only=True)
class LlmBaseEvent(LlmBaseItem):
    """Base class for LLM streaming response events."""

    sequence_number: int | None = None
    output_index: int
