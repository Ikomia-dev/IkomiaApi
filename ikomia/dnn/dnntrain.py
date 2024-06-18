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
Module dedicated to Deep Learning training.
"""
import logging
from typing import Optional
from datetime import datetime
import mlflow
from ikomia.core import config, CWorkflowTaskParam  # pylint: disable=E0611
from ikomia.dataprocess import CDnnTrainTask  # pylint: disable=E0611
from ikomia.dnn import datasetio, monitoring


logger = logging.getLogger(__name__)


class TrainProcess(CDnnTrainTask):
    """
    Base class for task dedicated to Deep Learning training.
    It includes mlflow framework and Tensorboard and handle connections with them:

        - experiment creation
        - runs management
        - parameters logging
        - metrics logging
        - training dashboard

    It must be used with :py:class:`~ikomia.core.task.TaskParam` or derived for parameters.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CDnnTrainTask`.
    """
    def __init__(self, name: str, param: CWorkflowTaskParam):
        """
        Constructor. Initialize mlflow local server.

        Args:
            name (str): task name
            param (:py:class:`~ikomia.core.task.TaskParam`): task parameters
        """
        CDnnTrainTask.__init__(self, name, param)
        self.add_input(datasetio.IkDatasetIO())
        self.experiment_id = None
        self._init_mlflow()
        self._init_tensorboard()

    @staticmethod
    def _init_mlflow():
        """
        Internal use only
        """
        monitoring.check_mlflow_server()
        mlflow.set_tracking_uri(config.main_cfg["mlflow"]["tracking_uri"])

    def _create_mlflow_experiment(self):
        try:
            date_time_obj = datetime.now()
            time_stamp_str = date_time_obj.strftime('%d-%m-%Y_%H:%M:%S')
            self.experiment_id = mlflow.create_experiment('experiment_' + time_stamp_str)
        except Exception as e:
            self.experiment_id = None
            logger.warning("Unable to create MLFlow experiment. Please check for server startup errors.")
            logger.debug(e)

    def _is_experiment_exists(self) -> bool:
        try:
            current_exp = mlflow.get_experiment(str(self.experiment_id))
            return current_exp.lifecycle_stage == "active"
        except Exception:
            return False

    @staticmethod
    def _init_tensorboard():
        try:
            monitoring.check_tensorboard_server()
        except Exception as e:
            logger.warning("TensorBoard can't be started so training metrics will not be monitor in it.")
            logger.debug(e)

    def begin_task_run(self):
        """
        Proceed to training job initialization:

            - start new mlflow run
            - log hyperparameters contained in :py:class:`~ikomia.dataprocess.PyDataProcess.CDnnTrainProcessParam`
        """
        super().begin_task_run()
        mlflow.end_run()

        if self.experiment_id is None or not self._is_experiment_exists():
            self._create_mlflow_experiment()
            if self.experiment_id is None:
                return

        mlflow.start_run(experiment_id=self.experiment_id, run_name=self.name)

        # Log parameters
        param = self.get_param_object()
        if param is not None:
            self.log_params(param.cfg)

    def log_param(self, key: str, value: any):
        """
        Log parameter to mlflow server

        Args:
            key (str): parameter name
            value: parameter value (numerical)
        """
        if self.experiment_id is not None:
            mlflow.log_param(key, value)

    def log_params(self, params: dict):
        """
        Log parameters to mlflow server

        Args:
            params (dict): parameters
        """
        if self.experiment_id is not None:
            mlflow.log_params(params)

    def log_metric(self, key: str, value: any, step: Optional[int] = None):
        """
        Log metric to mlflow server

        Args:
            key (str): metric name
            value: metric value (numerical)
            step (int): epoch/iteration index
        """
        if self.experiment_id is not None:
            mlflow.log_metric(key, value, step)

    def log_metrics(self, metrics: dict, step: Optional[int] = None):
        """
        Log metrics to mlflow server

        Args:
            metrics (dict): parameters
            step (int): epoch/iteration index
        """
        if self.experiment_id is not None:
            mlflow.log_metrics(metrics, step)

    def log_artifact(self, file_path: str):
        """
        Log artifact to mlflow server.
        Artifact could be any file (model weights, configuration file...)

        Args:
            file_path (str): source file
        """
        if self.experiment_id is not None:
            mlflow.log_artifact(file_path)

    def log_artifacts(self, folder_path: str):
        """
        Log artifacts to mlflow server.
        Artifacts could be any files stored in the same folder (model weights, configuration file...)

        Args:
            folder_path (str): artifacts folder
        """
        if self.experiment_id is not None:
            mlflow.log_artifacts(folder_path)

    def end_task_run(self):
        """
        Finalize mlflow run.
        """
        super().end_task_run()
        mlflow.end_run()

    def stop(self):
        """
        Request training job to stop.
        """
        super().stop()
