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

import subprocess
import logging
import requests
from ikomia.utils import is_colab
from ikomia.core import config
from ikomia.dnn.dataset import *
from ikomia.dnn.datasetio import *
from ikomia.dnn.dnntrain import *
from ikomia.dnn.dataset import *

logger = logging.getLogger(__name__)


def start_monitoring():
    colab = is_colab()
    _start_mlflow(colab)

    if not colab:
        _start_tensorboard()


def _start_mlflow(colab=False):
    # check if server is already running
    url = config.main_cfg["mlflow"]["tracking_uri"] + "/health"
    try:
        r = requests.get(url=url)
        running = r.status_code == 200
    except Exception:
        running = False

    if not running:
        proc = subprocess.Popen(["mlflow", "server",
                                 "--backend-store-uri", config.main_cfg["mlflow"]["store_uri"],
                                 "--default-artifact-root", config.main_cfg["mlflow"]["artifact_uri"],
                                 "--host", "0.0.0.0"])
        poll = proc.poll()
        if poll is None:
            print("MLflow server started successfully at ", config.main_cfg["mlflow"]["tracking_uri"])

    if colab:
        from pyngrok import ngrok

        # create ngrok tunnel to access dashboard via public URL
        # Terminate open tunnels if exist
        ngrok.kill()

        # Setting the authtoken (optional)
        # Get your authtoken from https://dashboard.ngrok.com/auth
        NGROK_AUTH_TOKEN = ""
        ngrok.set_auth_token(NGROK_AUTH_TOKEN)

        # Open an HTTPs tunnel on port 5000 for http://localhost:5000
        ngrok_tunnel = ngrok.connect(addr="5000", proto="http", bind_tls=True)
        logger.info("MLflow Tracking UI:" + ngrok_tunnel.public_url)


def _start_tensorboard():
    # check if server is already running
    url = config.main_cfg["tensorboard"]["tracking_uri"]
    try:
        r = requests.get(url=url)
        running = r.status_code == 200
    except:
        running = False

    if not running:
        proc = subprocess.Popen(["tensorboard",
                                 "--logdir", config.main_cfg["tensorboard"]["log_uri"]])
        poll = proc.poll()
        if poll is None:
            print("Tensorboard server started successfully at ", config.main_cfg["tensorboard"]["tracking_uri"])