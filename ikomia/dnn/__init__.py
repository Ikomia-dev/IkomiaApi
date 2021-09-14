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

import subprocess
import logging
import requests
from ikomia.utils import is_colab
from ikomia.core import config
from .dataset import *
from .datasetio import *
from .dnntrain import *
from .dataset import *

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
    except:
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