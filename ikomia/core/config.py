import os
import yaml

global main_cfg

directory = os.path.dirname(os.path.abspath(__file__))
main_cfg = yaml.safe_load(open(directory + "/config.yaml"))

# Ikomia registry
if not main_cfg["registry"]["path"]:
    main_cfg["registry"]["path"] = os.path.join(os.path.expanduser("~"), "Ikomia/Plugins/")

# Ikomia workflows
if not main_cfg["workflow"]["path"]:
    main_cfg["workflow"]["path"] = os.path.join(os.path.expanduser("~"), "Ikomia/Workflows/")

# Data directory
if not main_cfg["data"]["path"]:
    main_cfg["data"]["path"] = os.path.join(os.path.expanduser("~"), "Ikomia/Data/")

# MLflow
if not main_cfg["mlflow"]["artifact_uri"]:
    main_cfg["mlflow"]["artifact_uri"] = os.path.join(os.path.expanduser("~"), "Ikomia/MLflow/")

if not main_cfg["mlflow"]["store_uri"]:
    main_cfg["mlflow"]["store_uri"] = os.path.join(os.path.expanduser("~"), "Ikomia/MLflow/")

# Tensorboard
if not main_cfg["tensorboard"]["log_uri"]:
    main_cfg["tensorboard"]["log_uri"] = os.path.join(os.path.expanduser("~"), "Ikomia/Tensorboard/")
