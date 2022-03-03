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

from ikomia.core import config
from ikomia.utils import http
import logging
import requests

logger = logging.getLogger(__name__)


class LoginSession:
    def __init__(self, username, pwd):
        self.session = requests.Session()
        self.token = None
        self.authenticate(username, pwd)

    @http.http_except
    def authenticate(self, username, pwd):
        url = config.main_cfg["marketplace"]["url"] + "/api/rest-auth/login/"
        data = {"username": username, "password": pwd}
        r = self.session.post(url, json=data)
        r.raise_for_status()
        json_response = r.json()
        self.token = json_response["key"]
        header = {"Content-Type": "application/json", "Authorization": "Token " + str(self.token)}
        self.session.headers.update(header)



