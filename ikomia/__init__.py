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
import logging
from ikomia import utils
from ikomia.utils import autocomplete
from ikomia.core.auth import LoginSession
from ikomia.core import config
from ikomia.dataprocess import registry

# Add built-in OpenCV to Python path
if not utils.isAppStarted():
    if sys.platform == "win32":
        lib_path = os.path.dirname(os.path.realpath(__file__)) + os.sep + "lib"
        sys.path.insert(0, lib_path)
        os.environ["PATH"] += lib_path
    else:
        sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)) + os.sep + "opencv")

global ik_api_session
ik_api_session = None

global ik_registry
ik_registry = None

logger = logging.getLogger()


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
    os.makedirs(config.main_cfg["root_folder"], exist_ok=True)
    os.makedirs(config.main_cfg["registry"]["path"], exist_ok=True)
    os.makedirs(os.path.join(config.main_cfg["registry"]["path"], "C++/"), exist_ok=True)
    os.makedirs(os.path.join(config.main_cfg["registry"]["path"], "Python/"), exist_ok=True)
    os.makedirs(os.path.join(config.main_cfg["registry"]["path"], "Transfer/"), exist_ok=True)
    os.makedirs(config.main_cfg["workflow"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["data"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["artifact_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["store_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["tensorboard"]["log_uri"], exist_ok=True)


def init_logging(rank=-1):
    if rank in [-1, 0]:
        logger.handlers = []
        logger.setLevel(logging.INFO)

        # log to stdout and stderr
        formatter = logging.Formatter("%(message)s")
        info = logging.StreamHandler(sys.stdout)
        info.setLevel(logging.INFO)
        info.setFormatter(formatter)
        logger.addHandler(info)

        err = logging.StreamHandler(sys.stderr)
        err.setLevel(logging.ERROR)
        err.setFormatter(formatter)
        logger.addHandler(err)

        # log to file
        log_path = config.main_cfg["root_folder"] + "/log.txt"
        file_formatter = logging.Formatter("%(asctime)s;%(levelname)s;%(message)s", "%Y-%m-%d %H:%M:%S")
        file_handler = logging.FileHandler(log_path, 'w')
        file_handler.setLevel(logging.DEBUG)
        file_handler.setFormatter(file_formatter)
        logger.addHandler(file_handler)
    else:
        logging.basicConfig(format="%(message)s", level=logging.WARN)


# API initialization
_check_directories()
init_logging()

if not utils.isAppStarted():
    ik_registry = registry.IkomiaRegistry()

    if config.main_cfg["registry"]["auto_completion"]:
        autocomplete.make_local_plugins()
