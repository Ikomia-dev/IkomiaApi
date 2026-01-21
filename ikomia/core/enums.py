"""Global enums definition."""
from enum import IntEnum, unique

from ikomia.core import TaskType  # pylint: disable=E0611


def _extend_boost_enum(bp_enum, name: str, values: dict):
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
    all_members = {**members, **values}
    return unique(IntEnum(name, all_members))


WorkflowType = _extend_boost_enum(TaskType, "WorkflowType", {"LLM": 101})
