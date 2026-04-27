"""Global enums definition."""
from enum import IntEnum, unique


def _extend_boost_enum(bp_enum, name: str | None, values: dict[str, int]):
    name = name or bp_enum.__name__
    members = {}

    for attr in dir(bp_enum):
        if attr.startswith("_"):
            continue

        value = getattr(bp_enum, attr)
        # Keep only actual enum values
        if isinstance(value, bp_enum):
            members[attr] = int(value)

    # Combine both
    overlap = set(members) & set(values)
    if overlap:
        raise ValueError(f"Extension keys conflict with existing {bp_enum.__name__} members: {overlap}")

    all_members = {**members, **values}
    return unique(IntEnum(name, all_members))
