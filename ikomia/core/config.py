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

# Load local environment variables
load_dotenv()

config_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "config.yaml")
with open(config_path, "r", encoding="utf-8") as f:
    main_cfg = yaml.safe_load(f)

    # Ikomia root folder
    if not main_cfg["root_folder"]:
        main_cfg["root_folder"] = os.path.join(os.path.expanduser("~"), "Ikomia")

    # Ikomia registry
    if not main_cfg["registry"]["path"]:
        main_cfg["registry"]["path"] = os.path.join(main_cfg["root_folder"], "Plugins") + os.sep

    # Ikomia workflows
    if not main_cfg["workflow"]["path"]:
        main_cfg["workflow"]["path"] = os.path.join(main_cfg["root_folder"], "Workflows") + os.sep

    # Data directory
    if not main_cfg["data"]["path"]:
        main_cfg["data"]["path"] = os.path.join(main_cfg["root_folder"], "Data") + os.sep

    # MLflow
    if not main_cfg["mlflow"]["artifact_uri"]:
        main_cfg["mlflow"]["artifact_uri"] = os.path.join(main_cfg["root_folder"], "MLflow") + os.sep

    if not main_cfg["mlflow"]["store_uri"]:
        main_cfg["mlflow"]["store_uri"] = os.path.join(main_cfg["root_folder"], "MLflow") + os.sep

    # Tensorboard
    if not main_cfg["tensorboard"]["log_uri"]:
        main_cfg["tensorboard"]["log_uri"] = os.path.join(main_cfg["root_folder"], "Tensorboard") + os.sep

    # Auto-completion: could be override by environment variable
    if "IKOMIA_AUTO_COMPLETION" in os.environ:
        main_cfg["registry"]["auto_completion"] = utils.strtobool(os.getenv("IKOMIA_AUTO_COMPLETION"))
