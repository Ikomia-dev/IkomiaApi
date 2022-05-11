import sys
import os
import requests
import subprocess
import logging
from ikomia.core import config
from ikomia.utils import is_colab

logger = logging.getLogger(__name__)


def check_mlflow_server():
    try:
        url = config.main_cfg["mlflow"]["tracking_uri"] + "/health"
        r = requests.get(url)
        r.raise_for_status()
        logger.info(f"MLflow server is started at {config.main_cfg['mlflow']['tracking_uri']}")
    except:
        # Start server if needed
        if sys.platform == "win32":
            store_uri = config.main_cfg["mlflow"]["store_uri"].replace(':', '')
            artifact_uri = config.main_cfg["mlflow"]["artifact_uri"].replace(':', '')
            store_uri = store_uri.replace('\\', '/')
            artifact_uri = artifact_uri.replace('\\', '/')
            store_uri = "file://" + store_uri
            artifact_uri = "file://" + artifact_uri
        else:
            store_uri = config.main_cfg["mlflow"]["store_uri"]
            artifact_uri = config.main_cfg["mlflow"]["artifact_uri"]

        proc = subprocess.Popen(["mlflow", "server",
                                 "--backend-store-uri", store_uri,
                                 "--default-artifact-root", artifact_uri,
                                 "--host", "0.0.0.0"])
        poll = proc.poll()
        if poll is None:
            logger.info(f"MLflow server started successfully at {config.main_cfg['mlflow']['tracking_uri']}")

    colab = is_colab()
    if colab:
        try:
            from pyngrok import ngrok
        except:
            logger.error("MLflow dashboard won't be accessible. You need to install pyngrok before starting you training workflow: !pip install pyngrok.")
            return

        # create ngrok tunnel to access dashboard via public URL
        # Terminate open tunnels if exist
        ngrok.kill()

        # Setting the authtoken (optional)
        # Get your authtoken from https://dashboard.ngrok.com/auth
        token = os.environ["NGROK_AUTH_TOKEN"]
        ngrok.set_auth_token(token)
        # Open an HTTPs tunnel on port 5000 for http://localhost:5000
        ngrok_tunnel = ngrok.connect(addr="5000", proto="http", bind_tls=True)
        logger.info(f"MLflow Tracking UI: {ngrok_tunnel.public_url}")


def check_tensorboard_server():
    colab = is_colab()
    if colab:
        logger.info("To enable Tensorboard on Colab, please use the magic command: %load_ext tensorboard")
        return

    try:
        url = config.main_cfg["tensorboard"]["tracking_uri"]
        r = requests.get(url)
        r.raise_for_status()
        logger.info(f"Tensorboard server is started at {config.main_cfg['tensorboard']['tracking_uri']}")
    except:
        # Start server if needed
        proc = subprocess.Popen(["tensorboard", "--logdir", config.main_cfg["tensorboard"]["log_uri"]])
        poll = proc.poll()

        if poll is None:
            logger.info(f"Tensorboard server started successfully at {config.main_cfg['tensorboard']['tracking_uri']}")
