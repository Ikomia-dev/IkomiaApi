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
The module auth manages authentication to the Ikomia Scale platform (private algorithms HUB)
"""
import os
import logging
import requests
from requests.auth import HTTPBasicAuth
from ikomia.core import config

logger = logging.getLogger(__name__)


class LoginSession:
    """
    Session class to ease communication with Ikomia Scale/HUB through authenticated http requests.
    """
    def __init__(self):
        self.session = requests.Session()
        self.token = os.environ.get("IKOMIA_TOKEN")
        self.username = os.environ.get("IKOMIA_USER")
        self.password = os.environ.get("IKOMIA_PWD")

    def authenticate(self, token: str = None, username: str = None, password: str = None):
        """
        Authenticate user from token or classical credentials (username - password).

        Args:
            token (str): access token generated from Ikomia Scale platform or Ikomia CLI.
            username (str): username of your Ikomia Scale account.
            password (str): password of your Ikomia Scale account.
        """
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

    def is_authenticated(self) -> bool:
        """
        Check if user is authenticated.

        Returns:
            bool: authentication status.
        """
        if self.token is None:
            return False

        url = f"{config.main_cfg['hub']['url']}/v1/users/me/"
        try:
            r = self.session.get(url)
            r.raise_for_status()
        except Exception:
            return False

        return True

    def _create_token(self, ttl: int = 3600):
        url = config.main_cfg["hub"]["url"] + "/v1/users/me/tokens/"
        data = {"name": "Ikomia API token", "ttl": ttl}
        r = self.session.post(url, json=data, auth=HTTPBasicAuth(self.username, self.password))
        r.raise_for_status()
        json_response = r.json()
        return json_response["clear_token"]


# ---------------------------------------
# ----- Global Ikomia Scale session -----
# ---------------------------------------
ik_api_session = LoginSession()
# ---------------------------------------
