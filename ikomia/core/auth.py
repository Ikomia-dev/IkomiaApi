from ikomia.core import config
import logging
import requests
import functools

logger = logging.getLogger(__name__)

api_token = None


def http_except(func):
    @functools.wraps(func)
    def wrapper(*argc, **kwargs):
        try:
            return func(*argc, **kwargs)
        except requests.exceptions.HTTPError as errh:
            logger.error(errh)
        except requests.exceptions.ConnectionError as errc:
            logger.error(errc)
        except requests.exceptions.Timeout as errt:
            logger.error(errt)
        except requests.exceptions.RequestException as err:
            logger.error(err)

    return wrapper


@http_except
def authenticate(username, pwd):
    url = config.main_cfg["marketplace"]["url"] + "/api/rest-auth/login/"
    data = {"username": username, "password": pwd}
    r = requests.post(url, json=data)
    r.raise_for_status()
    json_response = r.json()
    global api_token
    api_token = json_response["key"]


