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
The core is mainly a Python binding of the corresponding C++ library from Ikomia Core.
"""
import os

try:
    # Valid for Ikomia Studio or Linux platform
    from ikomia.core.pycore import *
except ImportError:
    # Valid for Windows standalone API
    from ikomia.lib.pycore import *

from ikomia.core import config


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


def get_ikomia_root_folder() -> str:
    """
    Get root Ikomia folder.

    Returns:
        str: root folder, default: '$HOME/Ikomia' on Linux and 'C:\\Users\\your-user\\Ikomia on Windows'.
    """
    return config.main_cfg["root_folder"]


# API initialization
_check_directories()
