from ikomia import dataprocess
import os
import sys
import importlib


class IkomiaRegistry(dataprocess.CIkomiaRegistry):

    def __init__(self):
        dataprocess.CIkomiaRegistry.__init__(self)
        self._load_plugins()

    def _load_plugins(self):
        root_dir = self.getPluginsDirectory() + os.sep + "Python"

        if root_dir not in sys.path:
            sys.path.insert(0, root_dir)

        for root, subdirs, files in os.walk(root_dir):
            for dir in subdirs:
                main_module_name = dir + "." + dir
                main_module = importlib.import_module(main_module_name)
                main_class = getattr(main_module, dir)
                main_obj = main_class()
                task_factory = main_obj.getProcessFactory()
                self.registerTask(task_factory)


