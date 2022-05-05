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
Ikomia algorithms or any of those available in the Marketplace.
"""
import os
import logging
import enum

import ikomia
from ikomia import utils, core, dataprocess, dataio
from ikomia.core import config, task
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

    def __init__(self, name="Untitled", registry=None):
        """
        Construct Workflow object with the given name and an :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry`
        object. The latter is used to instanciate algorithm from their unique name when added to the workflow. Thus,
        you are able to use any Ikomia algorithms (built-in and Marketplace) in your workflow.
        """
        if registry is None:
            dataprocess.CWorkflow.__init__(self, name)
        else:
            dataprocess.CWorkflow.__init__(self, name, registry)

        self.registry = registry
        output_folder = os.path.join(config.main_cfg["workflow"]["path"], self.name) + os.sep
        self.setOutputFolder(output_folder)

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

        if index == -1 or index >= self.getInputCount():
            self.addInput(img_input)
        else:
            self.setInput(img_input, index, True)

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

        if index == -1 or index >= self.getInputCount():
            self.addInput(video_input)
        else:
            self.setInput(video_input, index, True)

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
        if index == -1:
            self.addInput(dir_input)
        else:
            self.setInput(dir_input, index, True)

    def set_parameters(self, params: dict, task_id=None, task_name="", index=-1):
        """
        Set task parameters as a simple key-value dict.
        You can get parameters keys for each by calling:

        .. code-block:: python

            print(task_obj.getParam())

        Args:
            params (dict): key-value pairs of parameters to modify.
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the wanted task. If -1, the function modifies all candidates parameters.
        """
        if task_id is not None:
            wf_task = self.getTask(task_id)
            if wf_task is not None:
                task.set_parameters(wf_task, params)
        elif task_name:
            wf_task = self.find_task(task_name)
            if wf_task is None:
                return

            if not isinstance(wf_task, list):
                task.set_parameters(wf_task[1], params)
            else:
                if index == -1:
                    for t in wf_task:
                        task.set_parameters(t[1], params)
                elif 0 <= index < len(wf_task):
                    task.set_parameters(wf_task[index][1], params)

    def get_time_metrics(self):
        """
        Get metrics around workflow execution time. This includes the total execution time of the workflow, and for
        each task, the execution time and the execution time from the start.

        Returns:
             dict: metrics
        """
        metrics = {"total_time": self.getTotalElapsedTime()}
        ids = self.getTaskIDs()

        for task_id in ids:
            t = self.getTask(task_id)
            task_metrics = {"task_time": t.getElapsedTime(), "time_from_start": self.getElapsedTimeTo(task_id)}
            metrics[t.name] = task_metrics

        return metrics

    def get_image_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow IMAGE output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple IMAGE outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CImageIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_image_output, task_id, task_name, index)

    def get_image(self, task_id=None, task_name="", index=-1):
        """
        Get image (numpy array) from a specific IMAGE output (by index) of the given task (by name or id).
        If task_name argument is given, the function returns images from all matching tasks. If index = -1,
        the function returns also images from all IMAGE outputs available.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the IMAGE output.

        Returns:
            Numpy array or list: result image(s).
        """
        outputs = self.get_image_output(task_id=task_id, task_name=task_name, index=index)
        if isinstance(outputs, list):
            images = []
            for output in outputs:
                images.append(output.getImage())
            return images
        else:
            return outputs.getImage()

    def get_image_with_graphics(self, task_id=None, task_name="", image_index=0, graphics_index=0):
        """
        Get image (numpy array) from a specific IMAGE output of the given task with graphics items burnt into it.
        Graphics items came from a specific GRAPHICS output of the same task. If task_name argument is given, the
        function returns images from all matching tasks.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            image_index (int): zero-based index of the IMAGE output.
            graphics_index (int): zero-based index of the GRAPHICS output.

        Returns:
            Numpy array or list: result image(s).
        """
        if task_id is not None:
            obj = self.getTask(task_id)
            return task.get_image_with_graphics(obj, image_index, graphics_index)
        elif task_name:
            wf_task = self.find_task(task_name)
            if not isinstance(wf_task, list):
                return task.get_image_with_graphics(wf_task[1], image_index, graphics_index)
            else:
                images = []
                for t in wf_task:
                    images.append(task.get_image_with_graphics(t[1], image_index, graphics_index))

                return images

    def get_graphics_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow GRAPHICS output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple GRAPHICS outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput`: output or list of outputs.
        """
        return self._get_task_output(task.get_graphics_output, task_id, task_name, index)

    def get_numeric_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow NUMERIC output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple NUMERIC outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CNumericIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_numeric_output, task_id, task_name, index)

    def get_data_string_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow data string output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple string outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CDataStringIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_data_string_output, task_id, task_name, index)

    def get_blob_measure_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow BLOB MEASURE output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple BLOB MEASURE outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CBlobMeasureIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_blob_measure_output, task_id, task_name, index)

    def get_dataset_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow DATASET output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple DATASET outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CDatasetIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_dataset_output, task_id, task_name, index)

    def get_array_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow ARRAY output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple ARRAY outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CArrayIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_array_output, task_id, task_name, index)

    def get_path_output(self, task_id=None, task_name="", index=-1):
        """
        Get workflow PATH output(s) for the given task(s). You can either pass a unique task ID or a task name.
        In the latter case, several tasks of the workflow can match the name. The function will then return all
        corresponding outputs in a list. Moreover, a task can have multiple PATH outputs so the index (zero-based)
        argument can be set to specify the wanted output.

        Args:
            task_id (int): unique identifier of the task. See also :py:meth:`~ikomia.dataprocess.workflow.add_task` and :py:meth:`~ikomia.dataprocess.workflow.find_task`.
            task_name (str): method :py:meth:`~ikomia.dataprocess.workflow.find_task` is used to retrieve corresponding task(s).
            index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.CPathIO`: output or list of outputs.
        """
        return self._get_task_output(task.get_path_output, task_id, task_name, index)

    def get_tasks(self):
        """
        Get unique identifier and name for all tasks composing the workflow.

        Returns:
             list of tuple (id (int), name (str)
        """
        tasks = []
        ids = self.getTaskIDs()
        for task_id in ids:
            t = self.getTask(task_id)
            if t is not None:
                tasks.append((task_id, t.name))

        return tasks

    def add_task(self, name, param=None):
        """
        Add task identified by its unique name in the workflow. If the given task is not yet in the registry, it will be
        firstly downloaded and installed from the Marketplace.

        Args:
            name (str): algorithm unique name
            param (:py:class:`~ikomia.core.pycore.CWorkflowTaskParam` or derived): algorithm parameters

        Returns:
            pair (tuple): 1- unique task identifier 2- task object
        """
        algo = self.registry.create_algorithm(name, param)
        if algo is None:
            raise RuntimeError(f"Algorithm {name} can't be created.")

        return self.addTask(algo), algo

    def find_task(self, name: str, index=-1):
        """
        Get identifiers and instance of tasks with the given name in the workflow.

        Args:
             name (str): algorithm name
             index (int): zero-based index of the wanted task. If -1, the function returns all candidates.

        Returns:
            list of pairs (tuple): 1- task identifier 2- task object instance
        """
        tasks = []
        ids = self.getTaskIDs()

        for task_id in ids:
            task = self.getTask(task_id)
            if task.name == name:
                tasks.append((task_id, task))

        if 0 <= index < len(tasks):
            return tasks[index]
        elif len(tasks) == 1:
            return tasks[0]
        else:
            return tasks

    def connect_tasks(self, src, target, edges=None):
        """
        Connect two tasks of the workflow. Depending of the inputs/outputs configuration, multiple connections between
        the two tasks can be set. A connection is a pair (ie tuple) composed by the output index of the source task
        and the input index of the target task.

        Args:
            src (int): source task identifier
            target (int): target task identifier
            edges (list of pair): connections. If *None* is passed, auto-connection is enabled so that the system will try to find the best connections automatically with respect to inputs and outputs data types.
        """
        if edges is None:
            self.connect(src, target, -1, -1)
        else:
            for edge in edges:
                self.connect(src, target, edge[0], edge[1])

    def run(self):
        """
        Start workflow execution on global input. Each :py:class:`~ikomia.core.pycore.CWorkflowTask` object or derived
        must reimplement the *run()* function that will be called in the right order by the workflow. Please note that
        global inputs should be set before calling this function (see
        :py:meth:`~ikomia.dataprocess.pydataprocess.CWorkflow.setInput`,
        :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_image_input`,
        :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_directory_input`).
        """
        self.updateStartTime()
        run_mode = self._get_run_mode()

        if run_mode == Workflow.RunMode.SINGLE:
            super().run()
        else:
            self._run_directory()

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

        self.updateStartTime()

        if folder:
            self.set_directory_input(folder=folder)
            self.run()
        else:
            if self._is_image_input(array, path, url):
                self.set_image_input(array=array, path=path, url=url, index=0)
                self.setCfgEntry("WholeVideo", str(int(False)))
            elif self._is_video_input(path, url):
                self.set_video_input(path=path, url=url, index=0)
                self.setCfgEntry("WholeVideo", str(int(True)))
            else:
                raise RuntimeError("Workflow run failed: unsupported input type.")

            self.run()

    def prepare_runtime_env(self, path):
        tasks = self.getRequiredTasks(path)
        available_tasks = ikomia.ik_registry.getAlgorithms()
        online_tasks = ikomia.ik_registry.get_online_algorithms()

        for t in tasks:
            if t not in available_tasks:
                if online_tasks is not None and any(d["name"] == t for d in online_tasks):
                    # try installation
                    try:
                        ikomia.ik_registry.install_algorithm(t)
                    except Exception:
                        msg = f"Workflow preparation failed: task {t} cannot be installed."
                        logger.error(msg)
                        return False
                else:
                    msg = f"Workflow preparation failed: task {t} cannot be found."
                    logger.error(msg)
                    return False

        return True

    def _run_directory(self):
        for i in range(self.getInputCount()):
            input_type = self.getInputDataType(i)
            if input_type == core.IODataType.FOLDER_PATH:
                dir_input = self.getInput(i)

                for root, subdirs, files in os.walk(dir_input.getPath(), topdown=True):
                    for file in files:
                        file_path = os.path.join(root, file)
                        if self._is_image_input(path=file_path):
                            self.set_image_input(path=file_path, index=i)
                            self.setCfgEntry("WholeVideo", str(int(False)))
                        elif self._is_video_input(path=file_path):
                            self.set_video_input(path=file_path, index=i)
                            self.setCfgEntry("WholeVideo", str(int(True)))
                        else:
                            logger.warning(f"Skipping file {file} as it is neither a supported image or video.")
                            continue

                        try:
                            super().run()
                        except Exception as e:
                            msg = f"Error occurred while processing {file}: {e.__str__()}"
                            logger.error(msg)

                self.setInput(dir_input, i, False)

    def _get_run_mode(self):
        input_types = []
        for i in range(self.getInputCount()):
            input_types.append(self.getInputDataType(i))

        target_types = self.getRootTargetTypes()

        if core.IODataType.FOLDER_PATH in input_types and core.IODataType.FOLDER_PATH not in target_types:
            return Workflow.RunMode.DIRECTORY
        else:
            return Workflow.RunMode.SINGLE

    def _get_task_output(self, get_output_func, task_id=None, task_name="", index=-1):
        wf_task = None
        if task_id is not None:
            wf_task = self.getTask(task_id)
            if wf_task is None:
                return None

            return get_output_func(wf_task, index)
        elif task_name:
            wf_task = self.find_task(task_name)
            if wf_task is None:
                return None

            if not isinstance(wf_task, list):
                return get_output_func(wf_task[1], index)
            else:
                wf_outputs = []
                for t in wf_task:
                    outs = get_output_func(t[1], index)
                    if isinstance(outs, list):
                        wf_outputs.extend(outs)
                    else:
                        wf_outputs.append(outs)

                return wf_outputs

    @staticmethod
    def _check_run_input(array, path, url, folder):
        return array is not None or path or url or folder

    @staticmethod
    def _is_image_input(array=None, path="", url=""):
        if array is not None:
            return True
        elif path:
            filename, ext = os.path.splitext(path)
            return dataio.CDataImageIO.isImageFormat(ext)
        elif url:
            parsed_url = urlparse(url)
            filename, ext = os.path.splitext(parsed_url.path)
            return dataio.CDataImageIO.isImageFormat(ext)
        else:
            return False

    @staticmethod
    def _is_video_input(path="", url=""):
        if path:
            filename, ext = os.path.splitext(path)
            return dataio.CDataVideoIO.isVideoFormat(ext, True)
        elif url:
            parsed_url = urlparse(url)
            filename, ext = os.path.splitext(parsed_url.path)
            return dataio.CDataVideoIO.isVideoFormat(ext, True)
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
    if wf.prepare_runtime_env(path):
        wf.load(path)
        return wf

    return None


def install_requirements(path):
    """
    Install needed requirements from the given workflow path.

    Args:
        path (str)

    Returns:
        True if all installations succeeded else False
    """
    wf = Workflow("untitled", ikomia.ik_registry)
    tasks = wf.getRequiredTasks(path)
    available_tasks = ikomia.ik_registry.getAlgorithms()
    plugins_directory = ikomia.ik_registry.getPluginsDirectory()
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

