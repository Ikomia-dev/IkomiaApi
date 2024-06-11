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

"""
The module utils is mainly a Python binding of the corresponding C++ library from Ikomia Core.
"""
from ikomia.utils import update_sys_path

try:
    # Valid for Ikomia Studio or Linux platform
    from ikomia.utils.pyutils import *
except ImportError:
    # Valid for Windows standalone API
    from ikomia.lib.pyutils import *


def is_colab() -> bool:
    """
    Helper function to check if we are in a Google Colab environment.

    Returns:
        bool: True in Colab environment, False otherwise
    """
    # Is environment a Google Colab instance?
    try:
        import google.colab
        return True
    except Exception:
        return False


def strtobool(val: str) -> bool:
    """Convert a string representation of truth to true (1) or false (0).
    True values are 'y', 'yes', 't', 'true', 'on', and '1'; false values
    are 'n', 'no', 'f', 'false', 'off', and '0'.  Raises ValueError if
    'val' is anything else.

    Args:
        val (str): value to convert

    Returns:
        bool: boolean conversion from val
    """
    val = val.lower()
    if val in ('y', 'yes', 't', 'true', 'on', '1'):
        return True

    if val in ('n', 'no', 'f', 'false', 'off', '0'):
        return False

    raise ValueError(f"invalid truth value: {val}")
