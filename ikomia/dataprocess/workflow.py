#!/usr/bin/env python
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

# -*- coding: utf-8 -*-
"""
Module dedicated to workflow management. It implements the Workflow class that offers high-level features
based on a C++ implementation. You will be able to create, modify, load and run workflows composed by built-in
Ikomia algorithms or any of those available in the Marketplace.
"""
import os
import logging
import enum
from ikomia import utils, core, dataprocess
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
        output_folder = config.main_cfg["workflow"]["path"]
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
            img_input = dataprocess.CImageIO(datatype, array, "Image")
        elif path:
            img_input = dataprocess.CImageIO(datatype, "Image", path)
        elif url:
            parsed = urlparse(url)
            img_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, img_path)
            img_input = dataprocess.CImageIO(datatype, "Image", img_path)

        if index == -1:
            self.addInput(img_input)
        else:
            self.setInput(img_input, index, True)

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
            raise RuntimeError("Algorithm " + name + "can't be created.")

        return self.addTask(algo), algo

    def find_task(self, name: str, index=-1):
        """
        Get identifiers of tasks with the given name in the workflow.

        Args:
             name (str): algorithm name
             index (int): zero-based index of the wanted task. If -1, the function returns all candidates.

        Returns:
            list of pairs (tuple): 1- task identifier 2- task object
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

    def _run_directory(self):
        for i in range(self.getInputCount()):
            input_type = self.getInputDataType(i)
            if input_type == core.IODataType.FOLDER_PATH:
                dir_input = self.getInput(i)

                for root, subdirs, files in os.walk(dir_input.getPath(), topdown=True):
                    for file in files:
                        self.set_image_input(path=os.path.join(root, file), index=i)
                        super().run()

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
