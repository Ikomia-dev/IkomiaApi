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

import os
import yaml

global main_cfg

directory = os.path.dirname(os.path.abspath(__file__))
main_cfg = yaml.safe_load(open(directory + "/config.yaml"))

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
