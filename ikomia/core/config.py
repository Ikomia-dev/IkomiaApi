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

"""
The module config loads the local configuration.
This configuration stands in a file named config.yaml where parameters can be changed.
"""
import os
import yaml
from dotenv import load_dotenv
from ikomia import utils


def load(config_path: str) -> dict:
    """
    Load Ikomia API configuration file (YAML)

    Args:
        config_path (str): path to the configuration file

    Returns:
        configuration data: dict
    """
    with open(config_path, "r", encoding="utf-8") as f:
        cfg = yaml.safe_load(f)

        # Ikomia root folder
        if not cfg["root_folder"]:
            cfg["root_folder"] = os.path.join(os.path.expanduser("~"), "Ikomia")

        # Ikomia registry
        if not cfg["registry"]["path"]:
            cfg["registry"]["path"] = os.path.join(cfg["root_folder"], "Plugins") + os.sep

        # Ikomia workflows
        if not cfg["workflow"]["path"]:
            cfg["workflow"]["path"] = os.path.join(cfg["root_folder"], "Workflows") + os.sep

        # Data directory
        if not cfg["data"]["path"]:
            cfg["data"]["path"] = os.path.join(cfg["root_folder"], "Data") + os.sep

        # MLflow
        if not cfg["mlflow"]["artifact_uri"]:
            cfg["mlflow"]["artifact_uri"] = os.path.join(cfg["root_folder"], "MLflow") + os.sep

        if not cfg["mlflow"]["store_uri"]:
            cfg["mlflow"]["store_uri"] = os.path.join(cfg["root_folder"], "MLflow") + os.sep

        # Tensorboard
        if not cfg["tensorboard"]["log_uri"]:
            cfg["tensorboard"]["log_uri"] = os.path.join(cfg["root_folder"], "Tensorboard") + os.sep

        # Auto-completion: could be override by environment variable
        if "IKOMIA_AUTO_COMPLETION" in os.environ:
            cfg["registry"]["auto_completion"] = utils.strtobool(os.getenv("IKOMIA_AUTO_COMPLETION"))

        return cfg


def save(config_path: str, cfg: dict):
    """
    Save given configuration data to file.

    Args:
        config_path (str): path where the configuration file will be saved
        cfg (dict): configuration data
    """
    with open(config_path, "w", encoding="utf-8") as f:
        yaml.safe_dump(cfg, f)


def save_main_config():
    """
    Save current configuration to file (overwrite).
    Use this function to change default configuration permanently.
    """
    save(main_config_path, main_cfg)


# Load local environment variables
load_dotenv()

# Main config
main_config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "config.yaml")
main_cfg = load(main_config_path)
