"""
The module monitoring provides helper functions to manage mlflow and tensorboard tracking servers.
"""

import logging
import os
import subprocess
import time
from pathlib import Path
from urllib.parse import urlparse

import requests
from requests.adapters import HTTPAdapter, Retry

from ikomia.core import config
from ikomia.utils import is_colab

logger = logging.getLogger(__name__)

# Seconds to wait for a newly started MLflow server to become healthy
MLFLOW_STARTUP_TIMEOUT = 30


def check_mlflow_server():
    """
    Start mlflow tracking server if it is not already started.
    Storage URIs are set according to Ikomia local configuration.
    """
    try:
        url = config.main_cfg["mlflow"]["tracking_uri"] + "/health"
        r = requests.get(url, timeout=5)
        r.raise_for_status()
        logger.info(
            "MLflow server is started at %s", config.main_cfg["mlflow"]["tracking_uri"]
        )
    except Exception:
        # Start server if needed
        # Build RFC 8089 file URIs (file:///home/... or file:///C:/...). Python 3.14 urllib
        # rejects the former "file:////home/..." form produced on POSIX.
        store_uri = Path(config.main_cfg["mlflow"]["store_uri"]).resolve().as_uri()
        artifact_uri = Path(config.main_cfg["mlflow"]["artifact_uri"]).resolve().as_uri()
        # MLflow >= 3.14 refuses to start on a filesystem backend store unless explicitly allowed.
        # Ignored by older versions.
        env = os.environ.copy()
        env.setdefault("MLFLOW_ALLOW_FILE_STORE", "true")

        logger.info("Starting MLflow server...")
        proc = subprocess.Popen(
            [
                "mlflow",
                "server",
                "--backend-store-uri",
                store_uri,
                "--default-artifact-root",
                artifact_uri,
                "--host",
                "0.0.0.0",
                "--port",
                str(urlparse(config.main_cfg["mlflow"]["tracking_uri"]).port or 5000),
            ],
            env=env,
        )
        _wait_mlflow_server(proc, url)

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
            logger.error(
                "MLflow dashboard won't be accessible. You need to install pyngrok before starting your "
                "training workflow: !pip install pyngrok. You also need a free ngrok account at least."
            )


def _wait_mlflow_server(proc: subprocess.Popen, health_url: str):
    """
    Internal use only. Wait until the freshly started MLflow server answers its health endpoint,
    the process exits, or the timeout expires. Failures are logged, not raised.
    """
    tracking_uri = config.main_cfg["mlflow"]["tracking_uri"]
    deadline = time.monotonic() + MLFLOW_STARTUP_TIMEOUT

    while time.monotonic() < deadline:
        return_code = proc.poll()
        if return_code is not None:
            logger.error(
                "MLflow server failed to start (exit code %s), training metrics will not be logged. "
                "See server output above for details.",
                return_code,
            )
            return

        try:
            requests.get(health_url, timeout=2).raise_for_status()
            logger.info("MLflow server started successfully at %s", tracking_uri)
            return
        except requests.RequestException:
            time.sleep(0.5)

    logger.error(
        "MLflow server did not answer at %s within %d seconds, training metrics may not be logged.",
        tracking_uri,
        MLFLOW_STARTUP_TIMEOUT,
    )


def check_tensorboard_server():
    """
    Start tensorboard tracking server if it is not already started.
    Storage URIs are set according to Ikomia local configuration.
    """
    colab = is_colab()
    if colab:
        logger.info(
            "To enable Tensorboard on Colab, please use the magic command: %load_ext tensorboard"
        )
        return

    url = config.main_cfg["tensorboard"]["tracking_uri"]
    try:
        r = requests.get(url, timeout=5)
        r.raise_for_status()
        logger.info(
            "Tensorboard server is started at %s",
            config.main_cfg["tensorboard"]["tracking_uri"],
        )
    except Exception:
        # Start server if needed
        logger.info("Starting Tensorboard server...")
        proc = subprocess.Popen(
            ["tensorboard", "--logdir", config.main_cfg["tensorboard"]["log_uri"]]
        )
        poll = proc.poll()

        if poll is None:
            retries = 5
            session = requests.session()
            retry = Retry(
                total=retries,
                read=retries,
                connect=retries,
                backoff_factor=0.2,
                status_forcelist=[500, 502, 503, 504],
            )
            adapter = HTTPAdapter(max_retries=retry)
            session.mount("http://", adapter)
            session.mount("https://", adapter)
            session.get(url)
            logger.info(
                "Tensorboard server started successfully at %s",
                config.main_cfg["tensorboard"]["tracking_uri"],
            )
