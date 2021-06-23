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



