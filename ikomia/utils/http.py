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

import os.path
import ikomia
import functools
import requests
import logging
from tqdm import tqdm

logger = logging.getLogger(__name__)


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
def download_file(url, path, public=True):
    if public:
        with requests.get(url, stream=True) as r:
            r.raise_for_status()
            total_size = int(r.headers.get('content-length', 0))
            name = os.path.basename(path)

            with open(path, "wb") as f, \
                    tqdm(desc=name, total=total_size, unit='iB', unit_scale=True, unit_divisor=1024, ) as bar:
                for data in r.iter_content(chunk_size=1024):
                    if data:
                        size = f.write(data)
                        bar.update(size)
    else:
        s = ikomia.ik_api_session
        if s.token is None:
            logger.error("Download failed, authentication required.")
            return

        with s.session.get(url, stream=True) as r:
            r.raise_for_status()
            total_size = int(r.headers.get('content-length', 0))
            name = os.path.basename(path)

            with open(path, "wb") as f,\
                    tqdm(desc=name, total=total_size, unit='iB', unit_scale=True, unit_divisor=1024,) as bar:
                for data in r.iter_content(chunk_size=1024):
                    if data:
                        size = f.write(data)
                        bar.update(size)
