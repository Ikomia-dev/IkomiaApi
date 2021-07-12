import os
import logging
import enum
from ikomia import utils, core, dataprocess
from ikomia.core import config
from urllib.parse import urlparse

logger = logging.getLogger(__name__)


class Workflow(dataprocess.CWorkflow):

    @enum.unique
    class RunMode(enum.Enum):
        SINGLE = 1
        DIRECTORY = 2

    def __init__(self, name="Untitled", registry=None):
        if registry is None:
            dataprocess.CWorkflow.__init__(self, name)
        else:
            dataprocess.CWorkflow.__init__(self, name, registry)

        self.registry = registry
        output_folder = config.main_cfg["workflow"]["path"]
        self.setOutputFolder(output_folder)

    def set_image_input(self, array=None, path="", url="", index=-1, datatype=core.IODataType.IMAGE):
        if array is not None:
            img_input = dataprocess.CImageIO(datatype, array)
        elif path:
            img_input = dataprocess.CImageIO(datatype, path)
        elif url:
            parsed = urlparse(url)
            img_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, img_path)
            img_input = dataprocess.CImageIO(datatype, img_path)

        if index == -1:
            self.addInput(img_input)
        else:
            self.setInput(img_input, index, True)

    def set_directory_input(self, folder="", index=-1):
        if not os.path.isdir(folder):
            logger.error("Directory input not set: you must pass an existing directory.")
            return

        dir_input = dataprocess.CPathIO(core.IODataType.FOLDER_PATH, folder)
        if index == -1:
            self.addInput(dir_input)
        else:
            self.setInput(dir_input, index, True)

    def get_time_metrics(self):
        metrics = {"total_time": self.getTotalElapsedTime()}
        ids = self.getTaskIDs()

        for task_id in ids:
            task = self.getTask(task_id)
            task_metrics = {"task_time": task.getElapsedTime(), "time_from_start": self.getElapsedTimeTo(task_id)}
            metrics[task.name] = task_metrics

        return metrics

    def add_task(self, name, param=None):
        algo = self.registry.create_algorithm(name, param)
        return self.addTask(algo)

    def find_task(self, name: str):
        tasks = []
        ids = self.getTaskIDs()

        for task_id in ids:
            task = self.getTask(task_id)
            if task.name == name:
                tasks.append((task_id, task))

        return tasks

    def connect_tasks(self, src, target, edges=None):
        if edges is None:
            self.connect(src, target, -1, -1)
        else:
            for edge in edges:
                self.connect(src, target, edge[0], edge[1])

    def run(self):
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
