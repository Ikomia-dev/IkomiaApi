# Copyright (C) 2021 Ikomia SAS
# Contact: https://www.ikomia.com
#
# This file is part of the Ikomia API libraries.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import os
import sys
from ikomia import utils
from ikomia.utils import init_logging, autocomplete
from ikomia.core.auth import LoginSession
from ikomia.core import config
from ikomia.dataprocess import registry

# Add built-in OpenCV to Python path
sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)) + os.sep + "opencv")

global ik_api_session
ik_api_session = None

global ik_registry
ik_registry = None


def authenticate():
    from dotenv import load_dotenv
    load_dotenv()
    username = os.environ.get("IKOMIA_USER")
    pwd = os.environ.get("IKOMIA_PWD")
    global ik_api_session
    ik_api_session = LoginSession(username, pwd)

    if config.main_cfg["registry"]["auto_completion"]:
        autocomplete.make_online_plugins()


def _check_directories():
    os.makedirs(os.path.join(os.path.expanduser("~"), "Ikomia/"), exist_ok=True)
    os.makedirs(config.main_cfg["registry"]["path"], exist_ok=True)
    os.makedirs(os.path.join(config.main_cfg["registry"]["path"], "C++/"), exist_ok=True)
    os.makedirs(os.path.join(config.main_cfg["registry"]["path"], "Python/"), exist_ok=True)
    os.makedirs(config.main_cfg["workflow"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["data"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["artifact_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["store_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["tensorboard"]["log_uri"], exist_ok=True)


# API initialization
_check_directories()
init_logging()

if not utils.isAppStarted():
    ik_registry = registry.IkomiaRegistry()

    if config.main_cfg["registry"]["auto_completion"]:
        autocomplete.make_local_plugins()
