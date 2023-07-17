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

import os
import sys
from ikomia import utils
from ikomia.utils import autocomplete
from ikomia.core import config
from ikomia.core.auth import LoginSession
from ikomia.dataprocess import registry

__version__ = "0.9.2"


# ----------------------------------------------
# ----- Add built-in OpenCV to Python path -----
# ----------------------------------------------
if not utils.is_app_started():
    if sys.platform == "win32":
        lib_path = os.path.dirname(os.path.realpath(__file__)) + os.sep + "lib"
        sys.path.insert(0, lib_path)
        os.environ["PATH"] += lib_path
    else:
        sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)) + os.sep + "opencv")
# ----------------------------------------------


global ik_api_session
ik_api_session = LoginSession()

# --------------------------------------
# ----- Ikomia algorithms registry -----
# --------------------------------------
global ik_registry
ik_registry = None

if not utils.is_app_started():
    ik_registry = registry.IkomiaRegistry(lazy_load=config.main_cfg["registry"]["lazy_load"])


# ------------------------------------------
# ----- Auto-completion initialization -----
# ------------------------------------------
if not utils.is_app_started() and config.main_cfg["registry"]["auto_completion"]:
    autocomplete.make()
# ------------------------------------------


def authenticate():
    from dotenv import load_dotenv
    load_dotenv()
    username = os.environ.get("IKOMIA_USER")
    pwd = os.environ.get("IKOMIA_PWD")
    global ik_api_session
    ik_api_session.authenticate(username, pwd)
