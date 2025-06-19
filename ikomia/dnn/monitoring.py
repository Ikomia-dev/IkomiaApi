"""
The module monitoring provides helper functions to manage mlflow and tensorboard tracking servers.
"""
import sys
import os
import subprocess
import logging
import requests
from requests.adapters import HTTPAdapter, Retry
from ikomia.core import config
from ikomia.utils import is_colab

logger = logging.getLogger(__name__)


def check_mlflow_server():
    """
    Start mlflow tracking server if it is not already started.
    Storage URIs are set according to Ikomia local configuration.
    """
    try:
        url = config.main_cfg["mlflow"]["tracking_uri"] + "/health"
        r = requests.get(url, timeout=5)
        r.raise_for_status()
        logger.info("MLflow server is started at %s", config.main_cfg['mlflow']['tracking_uri'])
    except Exception:
        # Start server if needed
        if sys.platform == "win32":
            store_uri = config.main_cfg["mlflow"]["store_uri"].replace(':', '')
            artifact_uri = config.main_cfg["mlflow"]["artifact_uri"].replace(':', '')
            store_uri = store_uri.replace('\\', '/')
            artifact_uri = artifact_uri.replace('\\', '/')
            store_uri = "file://" + store_uri
            artifact_uri = "file://" + artifact_uri
        else:
            store_uri = "file://" + config.main_cfg["mlflow"]["store_uri"]
            artifact_uri = "file://" + config.main_cfg["mlflow"]["artifact_uri"]

        logger.info("Starting MLflow server...")
        proc = subprocess.Popen(["mlflow", "server",
                                 "--backend-store-uri", store_uri,
                                 "--default-artifact-root", artifact_uri,
                                 "--host", "0.0.0.0"])
        poll = proc.poll()
        if poll is None:
            logger.info("MLflow server started successfully at %s", config.main_cfg['mlflow']['tracking_uri'])

    colab = is_colab()
    if colab:
        try:
            from pyngrok import ngrok

            # create ngrok tunnel to access dashboard via public URL
            # Terminate open tunnels if exist
            ngrok.kill()

            # Setting the authtoken (optional)
            # Get your authtoken from https://dashboard.ngrok.com/auth
            token = os.environ["NGROK_AUTH_TOKEN"]
            ngrok.set_auth_token(token)
            # Open an HTTPs tunnel on port 5000 for http://localhost:5000
            ngrok_tunnel = ngrok.connect(addr="5000", proto="http", bind_tls=True)
            logger.info("MLflow Tracking UI: %s", ngrok_tunnel.public_url)
        except Exception:
            logger.error("MLflow dashboard won't be accessible. You need to install pyngrok before starting your "
                         "training workflow: !pip install pyngrok. You also need a free ngrok account at least.")


def check_tensorboard_server():
    """
    Start tensorboard tracking server if it is not already started.
    Storage URIs are set according to Ikomia local configuration.
    """
    colab = is_colab()
    if colab:
        logger.info("To enable Tensorboard on Colab, please use the magic command: %load_ext tensorboard")
        return

    url = config.main_cfg["tensorboard"]["tracking_uri"]
    try:
        r = requests.get(url, timeout=5)
        r.raise_for_status()
        logger.info("Tensorboard server is started at %s", config.main_cfg['tensorboard']['tracking_uri'])
    except Exception:
        # Start server if needed
        logger.info("Starting Tensorboard server...")
        proc = subprocess.Popen(["tensorboard", "--logdir", config.main_cfg["tensorboard"]["log_uri"]])
        poll = proc.poll()

        if poll is None:
            retries = 5
            session = requests.session()
            retry = Retry(
                total=retries,
                read=retries,
                connect=retries,
                backoff_factor=0.2,
                status_forcelist=[500, 502, 503, 504], )
            adapter = HTTPAdapter(max_retries=retry)
            session.mount("http://", adapter)
            session.mount("https://", adapter)
            session.get(url)
            logger.info("Tensorboard server started successfully at %s", config.main_cfg['tensorboard']['tracking_uri'])
