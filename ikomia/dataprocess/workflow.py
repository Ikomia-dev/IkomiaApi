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
import ikomia
from ikomia import utils, core, dataprocess, dataio
from ikomia.core import config, task, IODataType
from urllib.parse import urlparse

logger = logging.getLogger(__name__)


class Workflow(dataprocess.CWorkflow):
    """
    Workflow management of Computer Vision tasks. Implement features to create, modify and run graph-based pipeline of
    :py:class:`~ikomia.core.pycore.CWorkflowTask` objects or derived. Workflows can be created from scratch
    by using :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry` to instanciate and connect task objects.
    Workflows can also be loaded from JSON file created with the interactive designer of Ikomia Studio.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CWorkflow`.
    """
    @enum.unique
    class RunMode(enum.Enum):
        SINGLE = 1
        DIRECTORY = 2

    def __init__(self, name="Untitled", registry=ikomia.ik_registry):
        """
        Construct Workflow object with the given name and an :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry`
        object. The latter is used to instanciate algorithm from their unique name when added to the workflow. Thus,
        you are able to use any Ikomia algorithms (built-in and Ikomia HUB) in your workflow.
        """
        if registry is None:
            dataprocess.CWorkflow.__init__(self, name)
        else:
            dataprocess.CWorkflow.__init__(self, name, registry)

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
        """
        return self.get_task(self.get_root_id())

    def set_image_input(self, array=None, path="", url="", index=-1, datatype=core.IODataType.IMAGE):
        """
        Set image as global input of the workflow. Image can be specified by a Numpy array, a path or an URL thanks to
        keyword arguments, you have to choose one of them.

        Args:
            array: image input as Numpy array
            path (str): image input as file path (valid formats are those managed by OpenCV)
            url (str): valid URL to image file (valid formats are those managed by OpenCV)
            index (int): zero-based input index, if -1 a new input is added
            datatype (:py:class:`~ikomia.core.pycore.IODataType`): image type
        """
        if array is not None:
            img_input = dataprocess.CImageIO(datatype, array, "")
        elif path:
            img_input = dataprocess.CImageIO(datatype, "", path)
        elif url:
            parsed = urlparse(url)
            img_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, img_path)
            img_input = dataprocess.CImageIO(datatype, "Image", img_path)

        if index == -1 or index >= self.get_input_count():
            self.add_input(img_input)
        else:
            self.set_input(img_input, index, True)

    def set_video_input(self, path="", url="", index=-1, datatype=core.IODataType.VIDEO):
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
            video_input = dataprocess.CVideoIO(datatype, "", path)
        elif url:
            parsed = urlparse(url)
            video_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, video_path)
            video_input = dataprocess.CVideoIO(datatype, "Video", video_path)

        if index == -1 or index >= self.get_input_count():
            self.add_input(video_input)
        else:
            self.set_input(video_input, index, True)

    def set_directory_input(self, folder="", index=-1):
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

        dir_input = dataprocess.CPathIO(core.IODataType.FOLDER_PATH, folder)
        if index == -1 or index >= self.get_input_count():
            self.add_input(dir_input)
        else:
            self.set_input(dir_input, index, True)

    def set_parameters(self, params: dict, task_obj=None, task_name="", index=-1):
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
            raise RuntimeError("Unable to set task parameters: parameters must include either a valid name or task instance.")

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

    def get_time_metrics(self):
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

    def get_task_output(self, task_obj=None, task_name="", task_index=0, types=[IODataType.IMAGE], output_index=-1):
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
                return task.get_output(task_obj[0], types, output_index)
            else:
                for t in task_obj:
                    outs = task.get_output(t, types, output_index)
                    if isinstance(outs, list):
                        outputs.extend(outs)
                    else:
                        outputs.append(outs)

                return outputs
        else:
            return task.get_output(task_obj, types, output_index)

    def get_tasks(self):
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

    def get_task_id(self, task):
        """
        Get task unique identifier from the task instance/

        Args:
            task (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): task instance

        Returns:
            int: task unique identifier
        """
        return self.task_to_id[task.uuid]

    def add_task(self, task=None, name:str="", params:dict={}, auto_connect:bool=False):
        """
        Add task identified by its unique name in the workflow. If the given task is not yet in the registry, it will be
        firstly downloaded and installed from Ikomia HUB. Task unique identifier can then be retrieved with
        :py:meth:`get_task_id`.

        Args:
            name (str): algorithm unique name
            param (:py:class:`~ikomia.core.pycore.CWorkflowTaskParam` based object): algorithm parameters

        Returns:
            :py:class:`~ikomia.core.pycore.CWorkflowTask` based object: task instance
        """
        if task is None and not name:
            raise RuntimeError("Unable to add task to workflow: parameters must include either a valid name or task instance.")

        if task is None:
            task = self.registry.create_algorithm(name, None)
            if task is None:
                raise RuntimeError(f"Algorithm {name} can't be created.")

        if len(params) > 0:
            task.set_parameters(params)

        parent_id = self.get_last_task_id()
        task_id = super().add_task(task)
        self.task_to_id[task.uuid] = task_id

        if (auto_connect):
            parent_task = self.get_task(parent_id)
            if parent_task is not None:
                self.connect_tasks(parent_task, task)
            else:
                self.connect_tasks(self.root(), task)

        return task

    def remove_task(self, task=None, name: str="", index: int=0):
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

    def find_task(self, name: str, index=-1):
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
        else:
            return tasks

    def connect_tasks(self, src, target, edges=None):
        """
        Connect two tasks of the workflow. Depending of the inputs/outputs configuration, multiple connections between
        the two tasks can be set. A connection is a pair (ie tuple) composed by the output index of the source task
        and the input index of the target task.

        Args:
            src (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): source task or None (connect to root)
            target (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): target task
            edges (list of pair): connections. If *None* is passed, auto-connection is enabled so that the system will try to find the best connections automatically with respect to inputs and outputs data types.
        """
        if src.uuid == self.root_uuid:
            src_id = self.get_root_id()
        else:
            src_id = self.get_task_id(src)

        target_id = self.get_task_id(target)

        if edges is None:
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
            logger.info(f"Workflow output data are saved in {self.get_last_run_folder()}")

        logger.info(f"Workflow {self.name} run successfully in {total_time} ms.")

    def run_on(self, array=None, path="", url="", folder=""):
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

    def prepare_runtime_env(self, path):
        tasks = self.get_required_tasks(path)
        available_tasks = ikomia.ik_registry.get_algorithms()

        try:
            online_tasks = ikomia.ik_registry.get_online_algorithms()
        except:
            online_tasks = None

        for t in tasks:
            if t not in available_tasks:
                try:
                    ikomia.ik_registry.create_algorithm(t)
                except Exception as e:
                    msg = f"Workflow preparation failed: task {t} cannot be found."
                    logger.error(msg)
                    logger.error(e)
                    return False

        return True

    def load(self, path):
        if self.prepare_runtime_env(path):
            super().load(path)

            # Update map task -> id
            self.task_to_id.clear()
            ids = self.get_task_ids()

            for task_id in ids:
                task = self.get_task(task_id)
                self.task_to_id[task.uuid] = task_id

    def _run_directory(self):
        for i in range(self.get_input_count()):
            input_type = self.get_input_data_type(i)
            if input_type == core.IODataType.FOLDER_PATH:
                dir_input = self.get_input(i)

                for root, subdirs, files in os.walk(dir_input.get_path(), topdown=True):
                    for file in files:
                        file_path = os.path.join(root, file)
                        if self._is_image_input(path=file_path):
                            self.set_image_input(path=file_path, index=i)
                            self.set_cfg_entry("WholeVideo", str(int(False)))
                        elif self._is_video_input(path=file_path):
                            self.set_video_input(path=file_path, index=i)
                            self.set_cfg_entry("WholeVideo", str(int(True)))
                        else:
                            logger.warning(f"Skipping file {file} as it is neither a supported image or video.")
                            continue

                        try:
                            super().run()
                        except Exception as e:
                            msg = f"Error occurred while processing {file}: {e.__str__()}"
                            logger.error(msg)

                self.set_input(dir_input, i, False)

    def _get_run_mode(self):
        input_types = []
        for i in range(self.get_input_count()):
            input_types.append(self.get_input_data_type(i))

        target_types = self.get_root_target_types()

        if core.IODataType.FOLDER_PATH in input_types and core.IODataType.FOLDER_PATH not in target_types:
            return Workflow.RunMode.DIRECTORY
        else:
            return Workflow.RunMode.SINGLE

    @staticmethod
    def _check_run_input(array, path, url, folder):
        return array is not None or path or url or folder

    @staticmethod
    def _is_image_input(array=None, path="", url=""):
        if array is not None:
            return True
        elif path:
            filename, ext = os.path.splitext(path)
            return dataio.CDataImageIO.is_image_format(ext)
        elif url:
            parsed_url = urlparse(url)
            filename, ext = os.path.splitext(parsed_url.path)
            return dataio.CDataImageIO.is_image_format(ext)
        else:
            return False

    @staticmethod
    def _is_video_input(path="", url=""):
        if path:
            filename, ext = os.path.splitext(path)
            return dataio.CDataVideoIO.is_video_format(ext, True)
        elif url:
            parsed_url = urlparse(url)
            filename, ext = os.path.splitext(parsed_url.path)
            return dataio.CDataVideoIO.is_video_format(ext, True)
        else:
            return False


def create(name="untitled"):
    """
    Create new empty workflow.
    See also :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`,
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks` and
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.run`.

    Args:
        name (str): workflow name.
    """
    return Workflow(name, ikomia.ik_registry)


def load(path):
    """
    Load Ikomia workflow from the given path.

    Args:
        path (str)

    Returns:
        :py:class:`~ikomia.dataprocess.workflow.Workflow`: loaded workflow.
    """
    wf = Workflow("untitled", ikomia.ik_registry)
    wf.load(path)
    return wf


def install_requirements(path):
    """
    Install needed requirements from the given workflow path.

    Args:
        path (str)

    Returns:
        True if all installations succeeded else False
    """
    wf = Workflow("untitled", ikomia.ik_registry)
    tasks = wf.get_required_tasks(path)
    available_tasks = ikomia.ik_registry.get_algorithms()
    plugins_directory = ikomia.ik_registry.get_plugins_directory()
    for t in tasks:
        if t not in available_tasks:
            plugin_dir = os.path.join(plugins_directory, "Python", t)
            if os.path.isdir(plugin_dir):
                utils.plugintools.install_requirements(plugin_dir)
            else:
                msg = f"Workflow preparation failed: task {t} cannot be found."
                logger.error(msg)
                return False
    return True

