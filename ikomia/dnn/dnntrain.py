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
import mlflow
from ikomia.core import config
from ikomia.dataprocess import CDnnTrainTask
from ikomia.dnn import datasetio, monitoring
from datetime import datetime


class TrainProcess(CDnnTrainTask):
    """
    Base class for task dedicated to Deep Learning training.
    It includes MLflow framework and Tensorboard and handle connections with them:

        - experiment creation
        - runs management
        - parameters logging
        - metrics logging
        - training dashboard

    It must be use with :py:class:`~ikomia.core.task.TaskParam` or derived for parameters.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CDnnTrainTask`.
    """
    def __init__(self, name, param):
        """
        Constructor. Initialize MLflow local server.

        Args:
            name (str): task name
            param (:py:class:`~ikomia.core.task.TaskParam`): task parameters
        """
        CDnnTrainTask.__init__(self, name, param)
        self.addInput(datasetio.IkDatasetIO())
        self.experiment_id = -1
        self._init_mlflow()
        self._init_tensorboard()

    def _init_mlflow(self):
        """
        Internal use only
        """
        monitoring.check_mlflow_server()
        # Create experiment
        date_time_obj = datetime.now()
        time_stamp_str = date_time_obj.strftime('%d-%m-%Y_%H:%M:%S')
        mlflow.set_tracking_uri(config.main_cfg["mlflow"]["tracking_uri"])

        try:
            self.experiment_id = mlflow.create_experiment('experiment_' + time_stamp_str)
        except:
            print("MLflow server is not accessible.")

    @staticmethod
    def _init_tensorboard():
        monitoring.check_tensorboard_server()

    def beginTaskRun(self):
        """
        Proceed to training job initialization:

            - start new MLflow run
            - log hyper-parameters contained in :py:class:`~ikomia.dataprocess.PyDataProcess.CDnnTrainProcessParam`
        """
        super().beginTaskRun()
        mlflow.end_run()

        if self.experiment_id != -1:
            mlflow.start_run(experiment_id=self.experiment_id, run_name=self.name)

            # Log parameters
            param = self.getParam()
            if param is not None:
                self.log_params(param.cfg)

    def log_param(self, key, value):
        """
        Log parameter to MLflow server

        Args:
            key (str): parameter name
            value: parameter value (numerical)
        """
        if self.experiment_id != -1:
            mlflow.log_param(key, value)

    def log_params(self, params):
        """
        Log parameters to MLflow server

        Args:
            params (dict): parameters
        """
        if self.experiment_id != -1:
            mlflow.log_params(params)

    def log_metric(self, key, value, step=None):
        """
        Log metric to MLflow server

        Args:
            key (str): metric name
            value: metric value (numerical)
        """
        if self.experiment_id != -1:
            mlflow.log_metric(key, value, step)

    def log_metrics(self, metrics, step=None):
        """
        Log metrics to MLflow server

        Args:
            metrics (dict): parameters
        """
        if self.experiment_id != -1:
            mlflow.log_metrics(metrics, step)

    def log_artifact(self, file_path):
        """
        Log artifact to MLflow server.
        Artifact could be any file (model weights, configuration file...)

        Args:
            file_path (str): source file
        """
        if self.experiment_id != -1:
            mlflow.log_artifact(file_path)

    def log_artifacts(self, folder_path):
        """
        Log artifacts to MLflow server.
        Artifacts could be any files stored in the same folder (model weights, configuration file...)

        Args:
            folder_path (str): artifacts folder
        """
        if self.experiment_id != -1:
            mlflow.log_artifacts(folder_path)

    def endTaskRun(self):
        """
        Finalize MLflow run.
        """
        super().endTaskRun()
        mlflow.end_run()

    def stop(self):
        """
        Request training job to stop.
        """
        super().stop()
