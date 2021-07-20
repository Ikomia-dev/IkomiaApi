#!/usr/bin/env python
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

# -*- coding: utf-8 -*-
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



