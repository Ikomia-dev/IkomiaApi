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
Helper module to ease somme http operations.
"""
import os.path
import functools
import logging
import requests
from tqdm import tqdm

logger = logging.getLogger(__name__)


def http_no_raise(func):
    """
    Decorator to avoid raising exception when using requests.
    """
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

        return None

    return wrapper


def download_file(url: str, path: str, ik_session=None):
    """
    Download the given file and save it.

    Args:
        url (str): source URL
        path (str): target path to save the downloaded file
        ik_session (LoginSession): authenticated session
    """
    if ik_session is None:
        s = requests.Session()
    else:
        s = ik_session.session

    with s.get(url, stream=True) as r:
        r.raise_for_status()
        total_size = int(r.headers.get('content-length', 0))
        name = os.path.basename(path)

        with open(path, "wb") as f, \
                tqdm(desc=name, total=total_size, unit='iB', unit_scale=True, unit_divisor=1024,) as progress:
            for data in r.iter_content(chunk_size=1024):
                if data:
                    size = f.write(data)
                    progress.update(size)
