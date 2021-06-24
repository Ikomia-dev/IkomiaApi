import subprocess
from ikomia.core import config


def start_mlflow():
    proc = subprocess.Popen(["mlflow", "server",
                             "--backend-store-uri", config.main_cfg["mlflow"]["store_uri"],
                             "--default-artifact-root", config.main_cfg["mlflow"]["artifact_uri"],
                             "--host", "0.0.0.0"])
    poll = proc.poll()
    if poll is None:
        print("MLflow server started successfully at ", config.main_cfg["mlflow"]["tracking_uri"])


def start_tensorboard():
    proc = subprocess.Popen(["tensorboard",
                             "--logdir", config.main_cfg["tensorboard"]["log_uri"]])
    poll = proc.poll()
    if poll is None:
        print("Tensorboard server started successfully at ", config.main_cfg["tensorboard"]["tracking_uri"])


if __name__ == "__main__":
    start_mlflow()
    start_tensorboard()
