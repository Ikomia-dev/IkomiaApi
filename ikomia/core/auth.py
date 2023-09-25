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
from ikomia.core import config
from ikomia.utils import http
import logging
import requests
from requests.auth import HTTPBasicAuth
from dotenv import load_dotenv

logger = logging.getLogger(__name__)


class LoginSession:
    def __init__(self):
        load_dotenv()
        self.session = requests.Session()
        self.token = os.environ.get("IKOMIA_TOKEN")
        self.username = os.environ.get("IKOMIA_USER")
        self.password = os.environ.get("IKOMIA_PWD")

    def authenticate(self, token: str = None, username: str = None, password: str = None):
        if token is not None:
            self.token = token
        elif username is not None and password is not None:
            self.username = username
            self.password = password
            self.token = self._create_token()
        elif self.token:
            pass
        elif self.token is None and self.username is not None and self.password is not None:
            self.token = self._create_token()
        else:
            raise RuntimeError("Authentication required token or user credentials")

        header = {
            "User-Agent": "Ikomia API",
            "Content-Type": "application/json",
            "Authorization": "Token " + str(self.token)
        }
        self.session.headers.update(header)

        # check connection
        url = f"{config.main_cfg['hub']['url']}/v1/users/me/"
        r = self.session.get(url)
        r.raise_for_status()

    def is_authenticated(self):
        if self.token is None:
            return False

        url = f"{config.main_cfg['hub']['url']}/v1/users/me/"
        try:
            r = self.session.get(url)
            r.raise_for_status()
        except:
            return False

        return True

    def _create_token(self, ttl: int = 3600):
        url = config.main_cfg["hub"]["url"] + "/v1/users/me/tokens/"
        data =  {"name": "Ikomia API token", "ttl": ttl}
        r = self.session.post(url, json=data, auth=HTTPBasicAuth(self.username, self.password))
        r.raise_for_status()
        json_response = r.json()
        return json_response["clear_token"]



