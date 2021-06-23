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
from ikomia.utils import init_logging
from ikomia.core.auth import LoginSession
from ikomia.core import config

global api_session
api_session = None

init_logging()


def initialize(username, pwd):
    global api_session
    api_session = LoginSession(username, pwd)
    check_directories()


def check_directories():
    os.makedirs(config.main_cfg["registry"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["workflow"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["data"]["path"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["artifact_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["mlflow"]["store_uri"], exist_ok=True)
    os.makedirs(config.main_cfg["tensorboard"]["log_uri"], exist_ok=True)