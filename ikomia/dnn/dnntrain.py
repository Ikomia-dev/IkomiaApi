# Copyright (C) 2021 Ikomia SAS
# Contact: https://www.ikomia.com
#
# This file is part of the Ikomia API libraries.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

"""
Module dedicated to Deep Learning training.
"""

from ikomia import core, dataprocess
import mlflow
from datetime import datetime


class TrainProcess(dataprocess.CDnnTrainTask):
    """
    Base class for task dedicated to Deep Learning training.
    It includes MLflow framework and handle connections with it:

        - experiment creation
        - runs management
        - parameters logging
        - metrics logging
        - training dashboard

    It must be use with :py:class:`~ikomia.dnn.dnntrain.TrainParam` or derived for parameters.
    """
    def __init__(self, name, param):
        """
        Constructor. Initialize MLflow local server.

        Args:
            name (str): task name
            param (:py:class:`~ikomia.dataprocess.PyDataProcess.CDnnTrainProcessParam`): task parameters
        """
        dataprocess.CDnnTrainTask.__init__(self, name, param)
        self.experiment_id = -1
        self._init_mlflow()

    def _init_mlflow(self):
        """
        Internal use only
        """
        # Create experiment
        date_time_obj = datetime.now()
        time_stamp_str = date_time_obj.strftime('%d-%m-%Y_%H:%M:%S')
        mlflow.set_tracking_uri('http://localhost:5000')

        try:
            self.experiment_id = mlflow.create_experiment('experiment_' + time_stamp_str)
        except:
            print("MLflow server is not accessible.")

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
