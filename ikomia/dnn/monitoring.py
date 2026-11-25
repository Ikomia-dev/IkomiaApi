"""
The module monitoring provides helper functions to manage mlflow and tensorboard tracking servers.
"""

import logging
import os
import sqlite3
import subprocess
import time
from contextlib import closing
from pathlib import Path
from urllib.parse import urlparse

import requests
from requests.adapters import HTTPAdapter, Retry

from ikomia.core import config
from ikomia.utils import is_colab

logger = logging.getLogger(__name__)

# Seconds to wait for a newly started MLflow server to become healthy
MLFLOW_STARTUP_TIMEOUT = 30
# SQLite database file name, created in the configured MLflow store directory
MLFLOW_DB_NAME = "mlflow.db"


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
        store_uri = get_mlflow_backend_store_uri()
        # Build RFC 8089 file URI (file:///home/... or file:///C:/...). Python 3.14 urllib
        # rejects the former "file:////home/..." form produced on POSIX.
        artifact_uri = Path(config.main_cfg["mlflow"]["artifact_uri"]).resolve().as_uri()

        if not _is_database_uri(config.main_cfg["mlflow"]["store_uri"]):
            if not _migrate_file_store(Path(config.main_cfg["mlflow"]["store_uri"]).resolve()):
                return

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
            ]
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


def _is_database_uri(store_uri: str) -> bool:
    """
    Internal use only. MLflow store_uri in configuration is either a database URI
    (sqlite:///..., postgresql://...) used as is, or a directory hosting the SQLite database.
    """
    return "://" in store_uri


def get_mlflow_backend_store_uri() -> str:
    """
    Get the MLflow backend store URI according to Ikomia local configuration.
    If the configured store_uri is a directory, runs are stored in a SQLite database inside it.

    Returns:
        str: MLflow backend store URI
    """
    store_uri = config.main_cfg["mlflow"]["store_uri"]
    if _is_database_uri(store_uri):
        return store_uri

    db_path = Path(store_uri).resolve() / MLFLOW_DB_NAME
    return "sqlite:///" + db_path.as_posix()


def _has_file_store_data(store_dir: Path) -> bool:
    """
    Internal use only. Detect experiments written by the former MLflow filesystem backend.
    Artifact folders of the SQLite backend live in the same directory but have no meta.yaml.
    """
    if not store_dir.is_dir():
        return False

    for folder in (store_dir, store_dir / ".trash"):
        if folder.is_dir() and any(
            d.is_dir() and d.name.isdigit() and (d / "meta.yaml").is_file()
            for d in folder.iterdir()
        ):
            return True

    return False


def _remove_sqlite_db(db_path: Path):
    for path in (db_path, Path(f"{db_path}-wal"), Path(f"{db_path}-shm"), Path(f"{db_path}-journal")):
        path.unlink(missing_ok=True)


def _normalize_file_uris(db_path: Path):
    """
    Internal use only. Rewrite legacy "file:////abs/path" URIs (former Ikomia default on POSIX)
    as "file:///abs/path": Python 3.14 urllib rejects them, making artifacts unreachable.
    """
    with closing(sqlite3.connect(db_path)) as connection, connection:
        for table, column in (("runs", "artifact_uri"), ("experiments", "artifact_location")):
            connection.execute(
                f"UPDATE {table} SET {column} = 'file://' || substr({column}, 9) "
                f"WHERE {column} LIKE 'file:////%'"
            )


def _migrate_file_store(store_dir: Path) -> bool:
    """
    Internal use only. One-time migration of runs stored with the former MLflow filesystem
    backend into the SQLite database. Legacy files are left untouched, artifacts stay in place.

    Returns:
        bool: True if the server can be started (nothing to migrate or migration succeeded)
    """
    db_path = store_dir / MLFLOW_DB_NAME
    if db_path.exists() or not _has_file_store_data(store_dir):
        return True

    target_uri = "sqlite:///" + db_path.as_posix()
    logger.info(
        "Migrating MLflow runs from file storage to %s (one-time operation)...", db_path
    )
    result = subprocess.run(
        ["mlflow", "migrate-filestore", "--source", str(store_dir), "--target", target_uri, "--no-progress"],
        stdin=subprocess.DEVNULL,
        capture_output=True,
        text=True,
        check=False,
    )
    try:
        if result.returncode != 0:
            raise RuntimeError(result.stderr.strip().splitlines()[-1] if result.stderr.strip() else
                               f"exit code {result.returncode}")
        _normalize_file_uris(db_path)
    except Exception as e:
        # Remove partial database so that migration is retried on next start
        _remove_sqlite_db(db_path)
        logger.error(
            "MLflow runs migration failed (%s). MLflow server is not started to preserve your history, "
            "training metrics will not be logged. To migrate manually: "
            "mlflow migrate-filestore --source \"%s\" --target \"%s\"",
            e,
            store_dir,
            target_uri,
        )
        return False

    logger.info("MLflow runs migration completed successfully.")
    return True


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
