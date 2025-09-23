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
Module dedicated to workflow management. It implements the Workflow class that offers high-level features
based on a C++ implementation. You will be able to create, modify, load and run workflows composed by built-in
Ikomia algorithms or any of those available in Ikomia HUB.
"""
import os
import logging
import enum
import datetime
from urllib.parse import urlparse
from typing import Optional, Union
import numpy as np
from ikomia import utils
from ikomia.core import config, IODataType, CWorkflowTask, CWorkflowTaskIO, auth  # pylint: disable=E0611
from ikomia.core.task import conform_parameters, get_output
from ikomia.dataio import CDataImageIO, CDataVideoIO  # pylint: disable=E0611
from ikomia.dataprocess import CWorkflow, CImageIO, CVideoIO, CPathIO, CHardwareConfig  # pylint: disable=E0611
from ikomia.dataprocess.registry import IkomiaRegistry, ik_registry


logger = logging.getLogger(__name__)


class Workflow(CWorkflow):
    """
    Workflow management of Computer Vision tasks. Implement features to create, modify and run graph-based pipeline of
    :py:class:`~ikomia.core.pycore.CWorkflowTask` objects or derived. Workflows can be created from scratch
    by using :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry` to instanciate and connect task objects.
    Workflows can also be loaded from JSON file created with the interactive designer of Ikomia Studio.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CWorkflow`.
    """
    @enum.unique
    class RunMode(enum.Enum):
        """
        Enum for processing mode
        """
        SINGLE = 1
        DIRECTORY = 2

    def __init__(self, name: str = "Untitled", registry: IkomiaRegistry = ik_registry):
        """
        Construct Workflow object with the given name and an :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry`
        object. The latter is used to instanciate algorithm from their unique name when added to the workflow. Thus,
        you are able to use any Ikomia algorithms (built-in and Ikomia HUB) in your workflow.

        Args:
            name (str): workflow name
            registry (IkomiaRegistry): Ikomia algorithm registry, default: global Ikomia registry
        """
        if registry is None:
            CWorkflow.__init__(self, name)
        else:
            CWorkflow.__init__(self, name, registry)

        self.registry = registry
        self.output_folder = os.path.join(config.main_cfg["workflow"]["path"], self.name) + os.sep
        root_id = self.get_root_id()
        self.root_uuid = self.get_task(root_id).uuid
        self.task_to_id = {self.root_uuid: root_id}

    def __repr__(self):
        return f"Workflow(\"{self.name}\")"

    def root(self):
        """
        Get workflow root node.

        Returns:
            CWorkflowTask: root task instance
        """
        return self.get_task(self.get_root_id())

    def init_long_process(self):
        """
        Perform time consuming initialization process for all tasks.
        This method gives you the flexibility to perform initialization steps like
        models downloading, models loading or models compiling when it is best suited for your usecase.
        Under the hood, it calls virtual method init_long_process() for all tasks.
        """
        tasks = self.get_tasks()
        for t in tasks:
            t.init_long_process()

    def set_image_input(self, array: np.ndarray = None, path: str ="", url: str = "", index: int = -1,
                        datatype: IODataType = IODataType.IMAGE):
        """
        Set image as global input of the workflow. Image can be specified by a Numpy array, a path or an URL thanks to
        keyword arguments, you have to choose one of them.

        Args:
            array (ndarray): image input as Numpy array
            path (str): image input as file path (valid formats are those managed by OpenCV)
            url (str): valid URL to image file (valid formats are those managed by OpenCV)
            index (int): zero-based input index, if -1 a new input is added
            datatype (:py:class:`~ikomia.core.pycore.IODataType`): image type
        """
        if array is not None:
            img_input = CImageIO(datatype, array, "")
        elif path:
            img_input = CImageIO(datatype, "", path)
        elif url:
            parsed = urlparse(url)
            img_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, img_path)
            img_input = CImageIO(datatype, "Image", img_path)
        else:
            raise ValueError("Unable to set input: you must set either array, path or url.")

        if index == -1 or index >= self.get_input_count():
            self.add_input(img_input)
        else:
            self.set_input(img_input, index, True)

    def set_video_input(self, path: str = "", url: str = "", index: int = -1,
                        datatype: IODataType = IODataType.VIDEO):
        """
        Set video as global input of the workflow. Video can be specified by a path or an URL thanks to
        keyword arguments, you have to choose one of them.

        Args:
            path (str): image input as file path (valid formats are those managed by OpenCV)
            url (str): valid URL to image file (valid formats are those managed by OpenCV)
            index (int): zero-based input index, if -1 a new input is added
            datatype (:py:class:`~ikomia.core.pycore.IODataType`): image type
        """
        if path:
            video_input = CVideoIO(datatype, "", path)
        elif url:
            parsed = urlparse(url)
            video_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, video_path)
            video_input = CVideoIO(datatype, "Video", video_path)
        else:
            raise ValueError("Unable to set input: you must set either path or url.")

        if index == -1 or index >= self.get_input_count():
            self.add_input(video_input)
        else:
            self.set_input(video_input, index, True)

    def set_directory_input(self, folder: str = "", index: int = -1):
        """
        Set folder as global input of the workflow. For image-based workflows, all images inside the directory
        (recursively) will be processed.

        Args:
            folder (str): images folder
            index (int): zero-based input index, if -1 a new input is added
        """
        if not os.path.isdir(folder):
            logger.error("Directory input not set: you must pass an existing directory.")
            return

        dir_input = CPathIO(IODataType.FOLDER_PATH, folder)
        if index == -1 or index >= self.get_input_count():
            self.add_input(dir_input)
        else:
            self.set_input(dir_input, index, True)

    def set_parameters(self, params: dict, task_obj: CWorkflowTask = None, task_name: str = "", index: int = -1):
        """
        Set task parameters as a simple key-value dict.
        You can get parameters keys for each by calling:

        .. code-block:: python

            print(task_obj.get_param_object())

        Args:
            params (dict): key-value pairs of parameters to modify.
            task_obj (:py:class:`~ikomia.core.CWorkflowTask` based object): task instance. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): algorithm name to be found. Multiple candidates may exist, so use task_index parameter to specify one. Method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the wanted task. If -1, the function modifies all candidates parameters.
        """
        if task_obj is None and not task_name:
            raise RuntimeError("Unable to set parameters: you must set either a valid name or task instance.")

        # Ensure parameter values are string due to C++ typing constraint
        params = conform_parameters(params)

        if task_obj is not None:
            task_obj.set_parameters(params)
        elif task_name:
            task_obj = self.find_task(task_name)
            if task_obj is None:
                raise RuntimeError(f"Algorithm {task_name} can't be found.")

            if not isinstance(task_obj, list):
                task_obj.set_parameters(params)
            else:
                if index == -1:
                    for t in task_obj:
                        t.set_parameters(params)
                elif 0 <= index < len(task_obj):
                    task_obj[index].set_parameters(params)

    def set_workflow_parameters(self, params: dict):
        """
        Set workflow parameters. Available parameters are those exposed when workflow is saved.
        Actually, an exposed parameter is bound to a task parameter within a workflow.
        The aim is to be able to select meaningfull parameters with respect to the workflow objective.

        Args:
            params (dict): key-value pairs
        """
        try:
            params = conform_parameters(params)
            for name in params:
                self.set_exposed_parameter(name, params[name])
        except RuntimeError as e:
            available_params = self.get_exposed_parameters()
            logger.error(e)
            logger.error("Available parameters: %s", available_params)

    def set_task_enabled(self, task: CWorkflowTask = None, name: str = "", index: int = -1, enabled: bool = True):
        """
        Enable/Disable task for running.

        Args:
            task (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): algorithm instance
            name (str): algorithm unique name
            index (int): zero-based index of the wanted task. If -1, the function modifies all candidates parameters.
            enabled (bool): True if algorithm has to be ran, False otherwise
        """
        if task is None and not name:
            raise RuntimeError("Unable to set task status: parameters must include either a name or task instance.")

        if task is not None:
            task.set_enabled(enabled)
        else:
            tasks = self.find_task(name=name, index=index)
            if tasks is None:
                raise RuntimeError(f"Algorithm {name} can't be found.")

            if not isinstance(tasks, list):
                tasks.set_enabled(enabled)
            else:
                if index == -1:
                    for t in tasks:
                        t.set_enabled(enabled)
                elif 0 <= index < len(tasks):
                    tasks[index].set_enabled(enabled)

    def get_time_metrics(self) -> dict:
        """
        Get metrics around workflow execution time. This includes the total execution time of the workflow, and for
        each task, the execution time and the execution time from the start.

        Returns:
             dict: metrics
        """
        metrics = {"total_time": self.get_total_elapsed_time()}
        ids = self.get_task_ids()

        for task_id in ids:
            t = self.get_task(task_id)
            task_metrics = {"task_time": t.get_elapsed_time(), "time_from_start": self.get_elapsed_time_to(task_id)}
            metrics[t.name] = task_metrics

        return metrics

    def get_task_output(self, task_obj = None, task_name: str = "", task_index: int = 0,
                        types: list = [IODataType.IMAGE], output_index: int = -1) -> CWorkflowTaskIO:
        """
        Get specific output(s) defined by their types (:py:class:`~ikomia.core.PyCore.IODataType`) for the given task.

        Args:
            task_obj (task_obj (:py:class:`~ikomia.core.CWorkflowTask` based object): task instance. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): algorithm name to be found. Multiple candidates may exist, so use task_index parameter to specify one. Method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            task_index (int): zero-based index of the wanted task. If -1, the function returns all candidates outputs.
            types (list of :py:class:`~ikomia.core.PyCore.IODataType`): output data types.
            output_index (int): zero-based index of he wanted output.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CWorkflowTaskIO` based object: task output of the given data types (can be a list).
        """
        if task_obj is None and not task_name:
            raise RuntimeError("Unable to get task output: parameters must include either a valid name or task instance.")

        if task_obj is None:
            task_obj = self.find_task(task_name, task_index)
            if task_obj is None:
                raise RuntimeError(f"Algorithm {task_name} can't be found.")

        outputs = []
        if isinstance(task_obj, list):
            if len(task_obj) == 1:
                return get_output(task_obj[0], types, output_index)

            for t in task_obj:
                outs = get_output(t, types, output_index)
                if isinstance(outs, list):
                    outputs.extend(outs)
                else:
                    outputs.append(outs)

            return outputs

        return get_output(task_obj, types, output_index)

    def get_tasks(self) -> list:
        """
        Get all tasks composing the workflow.

        Returns:
             list of :py:class:`~ikomia.core.pycore.CWorkflowTask` based object
        """
        tasks = []
        ids = self.get_task_ids()
        for task_id in ids:
            t = self.get_task(task_id)
            if t is not None:
                tasks.append(t)

        return tasks

    def get_task_id(self, task) -> int:
        """
        Get task unique identifier from the task instance.

        Args:
            task (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): task instance

        Returns:
            int: task unique identifier
        """
        return self.task_to_id[task.uuid]

    def get_workflow_parameters(self):
        """
        Get parameters exposed at workflow level.

        Returns:
            dict: list of key-values
        """
        return self.get_exposed_parameters()

    def add_task(self, task: CWorkflowTask = None, name: str = "", params: dict = None, auto_connect: bool = False,
                 public_hub: bool = True, private_hub: bool = False) -> CWorkflowTask:
        """
        Add task identified by its unique name in the workflow. If the given task is not yet in the registry, it will be
        firstly downloaded and installed from Ikomia HUB. Task unique identifier can then be retrieved with
        :py:meth:`get_task_id`.

        Args:
            task (CWorkflowTask): algorithm instance
            name (str): algorithm unique name
            params (:py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object): algorithm parameters
            auto_connect (bool): True to connect with parent tasks automatically
            public_hub (bool): True if algorithm can be installed from Ikomia HUB
            private_hub (bool): True if algorithm can be installed from private HUB

        Returns:
            :py:class:`~ikomia.core.pycore.CWorkflowTask` based object: task instance
        """
        if task is None and not name:
            raise RuntimeError("Unable to add task to workflow: you must set either a valid name or task instance.")

        if task is None:
            task = self.registry.create_algorithm(name=name,
                                                  parameters=params,
                                                  public_hub=public_hub,
                                                  private_hub=private_hub)
            if task is None:
                raise RuntimeError(f"Algorithm {name} can't be created.")

        parent_id = self.get_last_task_id()
        task_id = super().add_task(task)
        self.task_to_id[task.uuid] = task_id

        if auto_connect:
            parent_task = self.get_task(parent_id)
            if parent_task is not None:
                self.connect_tasks(parent_task, task)
            else:
                self.connect_tasks(self.root(), task)

        return task

    def remove_task(self, task: CWorkflowTask = None, name: str = "", index: int = 0):
        """
        Remove task from workflow specified by task instance or name (with corresponding index).

        Args:
            task (:py:class:`~ikomia.core.pycore.CWorkflowTask`): task object instance
            name (str): algorithm name to be found. Multiple candidates may exist, so use task_index parameter to specify one. Method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the wanted task.
        """
        if task is None and not name:
            raise RuntimeError("Unable to remove task: parameters must include either a valid name or task instance.")

        if task is None:
            task = self.find_task(name, index)
            if task is None:
                raise RuntimeError(f"Algorithm {name} can't be found.")

        super().delete_task(self.task_to_id[task.uuid])
        del self.task_to_id[task.uuid]

    def find_task(self, name: str, index: int = -1) -> Union[CWorkflowTask, list]:
        """
        Get identifiers and instance of tasks with the given name in the workflow.

        Args:
             name (str): algorithm name. Multiple candidates may exist, so use index parameter to specify one.
             index (int): zero-based index of the wanted task. If -1, the function returns all candidates.

        Returns:
            list of :py:class:`~ikomia.core.pycore.CWorkflowTask` based objects: if index == -1
            :py:class:`~ikomia.core.pycore.CWorkflowTask` based object : if index != -1
        """
        tasks = []
        ids = self.get_task_ids()

        for task_id in ids:
            task = self.get_task(task_id)
            if task.name == name:
                tasks.append(task)

        if 0 <= index < len(tasks):
            return tasks[index]

        return tasks

    def connect_tasks(self, src: CWorkflowTask, target: CWorkflowTask, edges: list = None):
        """
        Connect two tasks of the workflow. Depending of the inputs/outputs configuration, multiple connections between
        the two tasks can be set. A connection is a pair (ie tuple) composed by the output index of the source task
        and the input index of the target task.

        Args:
            src (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): source task or None (connect to root)
            target (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): target task
            edges (list of pair): connections. If empty list is passed, auto-connection is enabled so that the system will try to find the best connections automatically with respect to inputs and outputs data types.
        """
        if src.uuid == self.root_uuid:
            src_id = self.get_root_id()
        else:
            src_id = self.get_task_id(src)

        target_id = self.get_task_id(target)

        if edges is None or len(edges) == 0:
            self.connect(src_id, target_id, -1, -1)
        else:
            for edge in edges:
                self.connect(src_id, target_id, edge[0], edge[1])

    def run(self):
        """
        Start workflow execution on global input. Each :py:class:`~ikomia.core.pycore.CWorkflowTask` object or derived
        must reimplement the *run()* function that will be called in the right order by the workflow. Please note that
        global inputs should be set before calling this function (see
        :py:meth:`~ikomia.dataprocess.pydataprocess.CWorkflow.set_input`,
        :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_image_input`,
        :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_directory_input`).
        """
        self.update_start_time()
        run_mode = self._get_run_mode()

        if run_mode == Workflow.RunMode.SINGLE:
            super().run()
            metrics = self.get_time_metrics()
            total_time = metrics['total_time']
        else:
            time_start = datetime.datetime.now()
            self._run_directory()
            time_stop = datetime.datetime.now()
            total_time = (time_stop - time_start).total_seconds() * 1000
            logger.info("Workflow output data are saved in %s", self.get_last_run_folder())

        logger.info("Workflow %s run successfully in %s ms.", self.name, total_time)

    def run_on(self, array: np.ndarray = None, path: str = "", url: str = "", folder: str = ""):
        """
        Convenient function to run the workflow on common inputs. For more advanced use, please consult
        :py:class:`~ikomia.dataprocess.workflow.Workflow`.
        See also :py:func:`~ikomia.dataprocess.workflow.Workflow.set_image_input`
        and :py:func:`~ikomia.dataprocess.workflow.Workflow.set_directory_input`

        Args:
            array (Numpy array): image or generic array as numpy array
            path (str): path to image (valid formats are those managed by OpenCV)
            url (str): URL to image file (valid formats are those managed by OpenCV)
            folder (str): image folder
        """
        if not self._check_run_input(array, path, url, folder):
            raise RuntimeError("Workflow input is invalid: you must pass either an numpy array, a path to image/video"
                               "or a folder containing images or videos.")

        if folder:
            self.set_auto_save(True)
            self.set_directory_input(folder=folder)
            self.run()
        else:
            if self._is_image_input(array, path, url):
                self.set_image_input(array=array, path=path, url=url, index=0)
                self.set_cfg_entry("WholeVideo", str(int(False)))
            elif self._is_video_input(path, url):
                self.set_video_input(path=path, url=url, index=0)
                self.set_cfg_entry("WholeVideo", str(int(True)))
            else:
                raise RuntimeError("Workflow run failed: unsupported input type.")

            self.run()

    def load(self, path: str):
        """
        Load the worflow file at the given path. The function will try to install algorithms if they are not available.

        Args:
            path (str): full path to the workflow definition file to load.
        """
        try:
            super().load(path)
        except RuntimeError:
            # Some algorithms are missing, try to install it from HUB
            prepare_runtime_env(path)
            super().load(path)

        # Update map task -> id
        self.task_to_id.clear()
        ids = self.get_task_ids()

        for task_id in ids:
            task = self.get_task(task_id)
            self.task_to_id[task.uuid] = task_id

    def save(self, path: str, exposed_params: Optional[dict] = None, exposed_outputs: Optional[dict] = None):
        """
        Save workflow into a JSON definition file.

        Args:
            path (str): full path to the workflow definition file to save.
            exposed_params (dict): set the list of task parameters that has to be exposed at workflow level. The aim is
                to be able to select meaningfull parameters with respect to the workflow objective.

                Example of the dict structure:

                .. code-block:: python

                    exposed_params = {
                        "task_name": {
                            "task_parameter_name_1": {
                                "name": "my_workflow_parameter_name_1",
                                "description": "my_description_1",
                            },
                            "task_parameter_name_2": {
                                "name": "my_workflow_parameter_name_2",
                                "description": "my_description_2",
                            },
                        }
                    }

                To specify a task, you can give a task name, a task instance or a task id. Be careful with task name
                because, in case of workflow involving several times the same task, the function will expose parameters
                of the first task only. Prefer using task instance or task id in this case.

                If 'name" field is empty, the original task parameter name is used.

                The 'description' field is also optional.

                If you want to expose all parameters of a task, just pass an empty dict for the value of the
                task name key.
            exposed_outputs (dict): set the list of outputs that has to be exposed at workflow level. The aim is
                to be able to select meaningfull outputs with respect to the workflow objective.

                Example of the dict structure:

                .. code-block:: python

                    exposed_outputs = {
                        "task_name": [
                            {
                                "description": "my_description_1",
                                "index": 0,
                            },
                            {
                                "description": "my_description_2",
                                "index": 1,
                            },
                        ]
                    }

                To specify a task, you can give a task name, a task instance or a task id. Be careful with task name
                because, in case of workflow involving several times the same task, the function will expose parameters
                of the first task only. Prefer using task instance or task id in this case.

                The 'description' field is optional.

                If you want to expose all outputs of a task, just pass an empty list for the value of the
                task name key.
        """
        if exposed_params:
            self._expose_parameters(exposed_params)

        if exposed_outputs:
            self._expose_outputs(exposed_outputs)

        super().save(path)

    def _run_directory(self):
        for i in range(self.get_input_count()):
            input_type = self.get_input_data_type(i)
            if input_type == IODataType.FOLDER_PATH:
                dir_input = self.get_input(i)

                for root, _, files in os.walk(dir_input.get_path(), topdown=True):
                    for file in files:
                        file_path = os.path.join(root, file)
                        if self._is_image_input(path=file_path):
                            self.set_image_input(path=file_path, index=i)
                            self.set_cfg_entry("WholeVideo", str(int(False)))
                        elif self._is_video_input(path=file_path):
                            self.set_video_input(path=file_path, index=i)
                            self.set_cfg_entry("WholeVideo", str(int(True)))
                        else:
                            logger.warning("Skipping file %s as it is neither a supported image or video.", file)
                            continue

                        try:
                            super().run()
                        except Exception as e:
                            msg = f"Error occurred while processing {file}: {e}"
                            logger.error(msg)

                self.set_input(dir_input, i, False)

    def _get_run_mode(self):
        input_types = []
        for i in range(self.get_input_count()):
            input_types.append(self.get_input_data_type(i))

        target_types = self.get_root_target_types()

        if IODataType.FOLDER_PATH in input_types and IODataType.FOLDER_PATH not in target_types:
            return Workflow.RunMode.DIRECTORY

        return Workflow.RunMode.SINGLE

    @staticmethod
    def _check_run_input(array: np.ndarray, path: str, url: str, folder: str) -> bool:
        return array is not None or path or url or folder

    @staticmethod
    def _is_image_input(array: np.ndarray = None, path: str = "", url: str = "") -> bool:
        if array is not None:
            return True

        if path:
            _, ext = os.path.splitext(path)
            return CDataImageIO.is_image_format(ext)

        if url:
            parsed_url = urlparse(url)
            _, ext = os.path.splitext(parsed_url.path)
            return CDataImageIO.is_image_format(ext)

        return False

    @staticmethod
    def _is_video_input(path: str = "", url: str = "") -> bool:
        if path:
            _, ext = os.path.splitext(path)
            return CDataVideoIO.is_video_format(ext, True)

        if url:
            parsed_url = urlparse(url)
            _, ext = os.path.splitext(parsed_url.path)
            return CDataVideoIO.is_video_format(ext, True)

        return False

    def _expose_parameters(self, exposed_params: dict):
        self.clear_exposed_parameters()

        for task_key in exposed_params:
            if isinstance(task_key, CWorkflowTask):
                task_id = self.task_to_id[task_key.uuid]
            elif isinstance(task_key, int):
                task_id = task_key
            elif isinstance(task_key, str):
                # TODO: how to manage multiple tasks with the same name
                task = self.find_task(task_key, 0)
                task_id = self.task_to_id[task.uuid]
            else:
                raise TypeError("Task identification must be either a task instance, a task id or a task name.")

            if len(exposed_params[task_key]) == 0:
                # Expose all parameters
                t = self.get_task(task_id)
                task_params = t.get_parameters()

                for param in task_params:
                    self.add_exposed_parameter("", "", task_id, param)
            else:
                # Expose specified parameters only
                for param_name, info in exposed_params[task_key].items():
                    if not isinstance(param_name, str):
                        raise TypeError("String expected for task parameter name.")

                    name = info["name"] if "name" in info else ""
                    description = info["description"] if "description" in info else ""
                    self.add_exposed_parameter(name, description, task_id, param_name)

    def _expose_outputs(self, outputs: dict):
        self.clear_outputs()

        for task_key in outputs:
            if isinstance(task_key, CWorkflowTask):
                task_id = self.task_to_id[task_key.uuid]
            elif isinstance(task_key, int):
                task_id = task_key
            elif isinstance(task_key, str):
                # TODO: how to manage multiple tasks with the same name
                task = self.find_task(task_key, 0)
                task_id = self.task_to_id[task.uuid]
            else:
                raise TypeError("Task identification must be either a task instance, a task id or a task name.")

            if len(outputs[task_key]) == 0:
                # Expose all outputs
                t = self.get_task(task_id)
                task_outputs = t.get_outputs()

                for i, _ in enumerate(task_outputs):
                    self.add_output("", task_id, i)
            else:
                # Expose specified output only
                for output_info in outputs[task_key]:
                    description = output_info["description"] if "description" in output_info else ""
                    self.add_output(description, task_id, output_info["index"])


def create(name: str = "untitled"):
    """
    Create new empty workflow.
    See also :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`,
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks` and
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.run`.

    Args:
        name (str): workflow name.

    Returns:
        Workflow: workflow instance
    """
    return Workflow(name, ik_registry)


def load(path: str) -> Workflow:
    """
    Load Ikomia workflow from the given path.

    Args:
        path (str)

    Returns:
        :py:class:`~ikomia.dataprocess.workflow.Workflow`: loaded workflow.
    """
    wf = Workflow("untitled", ik_registry)
    wf.load(path)
    return wf


def prepare_runtime_env(workflow_path: str):
    """
    Install all algorithms needed to execute the workflow stored at the given path.
    If algorithms are not locally installed, the function will try to install them
    from Ikomia HUB (public and private if authenticated).

    Args:
        workflow_path (str): path to workflow definition file (.json)
    """
    tasks = Workflow.get_required_tasks(workflow_path)
    available_tasks = ik_registry.get_algorithms()
    private_hub = bool(auth.ik_api_session.is_authenticated())

    for t in tasks:
        if t not in available_tasks:
            try:
                ik_registry.create_algorithm(name=t, public_hub=True, private_hub=private_hub)
            except Exception as e:
                raise RuntimeError(f"Workflow preparation failed at algorithm {t} for the following reason: {e}") from e


def install_requirements(path: str) -> bool:
    """
    Install Python requirements from all algorithms of the workflow stored at the given path.
    Algorithms must be installed locally before calling this function.

    Args:
        path (str): path to workflow definition file (.json)

    Returns:
        True if all installations succeeded else False
    """
    tasks = Workflow.get_required_tasks(path)
    available_tasks = ik_registry.get_algorithms()
    plugins_directory = ik_registry.get_plugins_directory()

    for t in tasks:
        if t not in available_tasks:
            plugin_dir = os.path.join(plugins_directory, "Python", t)
            if os.path.isdir(plugin_dir):
                utils.plugintools.install_requirements(plugin_dir)
            else:
                msg = f"Requirements installation failed: algorithm folder {t} cannot be found."
                logger.error(msg)
                return False
    return True


def get_min_hardware_config(path: str) -> CHardwareConfig:
    """
    Get minimum hardware configuration for the given workflow file.
    The configuration is determined from the minimum hardware configuration of workflow tasks.

    Args:
         path (str): path to workflow definition file (.json)

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CHardwareConfig`: hardware configuration
    """
    prepare_runtime_env(path)
    min_hw_config = CHardwareConfig()
    task_names = Workflow.get_required_tasks(path)

    for name in task_names:
        info = ik_registry.get_algorithm_info(name)
        min_hw_config.min_cpu = max(min_hw_config.min_cpu, info.hardware_config.min_cpu)
        min_hw_config.min_ram = max(min_hw_config.min_ram, info.hardware_config.min_ram)
        min_hw_config.gpu_required = min_hw_config.gpu_required or info.hardware_config.gpu_required
        min_hw_config.min_vram = max(min_hw_config.min_vram, info.hardware_config.min_vram)

    return min_hw_config
