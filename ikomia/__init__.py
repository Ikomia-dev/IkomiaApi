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
The ikomia module provides direct access to:
- Global algorithms registry through ikomia.ik_registry
- Authentication function

Import dedicated submodules to leverage all other features.
"""

import os
import sys
import time
import logging
from ikomia.core import config
from ikomia.utils import is_app_started, iklogger, autocomplete  # pylint: disable=E0611
from ikomia.core.auth import ik_api_session

__version__ = "0.15.0"

logger = logging.getLogger(__name__)


# ----------------------------------------------
# ----- Add built-in OpenCV to Python path -----
# ----------------------------------------------
if not is_app_started():
    if sys.platform == "win32":
        lib_path = os.path.dirname(os.path.realpath(__file__)) + os.sep + "lib"
        sys.path.insert(0, lib_path)
        os.environ["PATH"] += lib_path
    else:
        sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)) + os.sep + "opencv")
# ----------------------------------------------


# -------------------------------------
# ----- Create new empty log file -----
# -------------------------------------
if not is_app_started():
    log_path = config.main_cfg["root_folder"] + "/log.txt"
    with open(log_path, "w", encoding="utf-8") as f:
        logger.debug("Logging started at %s", time.strftime('%Y-%m-%d %H:%M:%S'))
# -------------------------------------


# ------------------------------------------
# ----- Auto-completion initialization -----
# ------------------------------------------
if not is_app_started() and config.main_cfg["registry"]["auto_completion"]: # noqa 1101
    autocomplete.ik_autocomplete.make()
# ------------------------------------------


def authenticate(token: str = None, username: str = None, password: str = None):
    """
    Authentication to Ikomia Scale. Mandatory if user need access to private algorithms.
    If no credentials are provided, Ikomia API will try to load them from environment variables:
    IKOMIA_TOKEN or (IKOMIA_USER, IKOMIA_PWD).

    Args:
        token (str): Ikomia token provided by Ikomia Scale or Ikomia CLI
        username (str): Ikomia Scale username
        password (str): Ikomia Scale password
    """
    ik_api_session.authenticate(token=token, username=username, password=password)
