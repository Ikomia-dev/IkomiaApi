import sys
import requests
import subprocess
from ikomia.core import config


def check_mlflow_server():
    try:
        url = config.main_cfg["mlflow"]["tracking_uri"] + "/health"
        r = requests.get(url)
        r.raise_for_status()
        print("MLflow server is started at ", config.main_cfg["mlflow"]["tracking_uri"])
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
            print("MLflow server started successfully at ", config.main_cfg["mlflow"]["tracking_uri"])


def check_tensorboard_server():
    try:
        url = config.main_cfg["tensorboard"]["tracking_uri"]
        r = requests.get(url)
        r.raise_for_status()
        print("Tensorboard server is started at ", config.main_cfg["tensorboard"]["tracking_uri"])
    except:
        # Start server if needed
        proc = subprocess.Popen(["tensorboard", "--logdir", config.main_cfg["tensorboard"]["log_uri"]])
        poll = proc.poll()

        if poll is None:
            print("Tensorboard server started successfully at ", config.main_cfg["tensorboard"]["tracking_uri"])
