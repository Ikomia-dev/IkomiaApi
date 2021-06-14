from ikomia import utils, dataprocess
from ikomia.core import config, auth
import os
import sys
import importlib
import requests
import logging

logger = logging.getLogger(__name__)


class IkomiaRegistry(dataprocess.CIkomiaRegistry):

    def __init__(self):
        dataprocess.CIkomiaRegistry.__init__(self)
        self._load_plugins()

    def _load_plugins(self):
        root_dir = self.getPluginsDirectory() + os.sep + "Python"

        if root_dir not in sys.path:
            sys.path.insert(0, root_dir)

        for root, subdirs, files in os.walk(root_dir, topdown=True):
            subdirs[:] = [d for d in subdirs if not d[0] == '.']
            for directory in subdirs:
                main_module_name = directory + "." + directory
                main_module = importlib.import_module(main_module_name)
                main_class = getattr(main_module, directory)
                main_obj = main_class()
                task_factory = main_obj.getProcessFactory()

                plugin_version = task_factory.info.ikomia_version
                compatibility_state = utils.getCompatibilityState(plugin_version)

                if compatibility_state == utils.PluginState.DEPRECATED:
                    logger.error("Plugin " + directory + "is deprecated: based on Ikomia " + str(plugin_version) +
                                 "while the current version is " + str(utils.getApiVersion()))
                    continue
                elif compatibility_state == utils.PluginState.UPDATED:
                    logger.warning("Plugin " + directory + "is based on Ikomia " + str(plugin_version) +
                                   "while the current version is " + str(utils.getApiVersion()) +
                                   ". You should consider updating Ikomia API.")

                self.registerTask(task_factory)
            break

    @auth.http_except
    def get_online_algorithms(self):
        if auth.api_token is None:
            logger.error("Online algorithms retrieval failed, authentification required.")
            return

        url = config.main_cfg["marketplace"]["url"] + "/api/plugin/"
        header = {"Authorization": "Token " + str(auth.api_token)}
        r = requests.get(url, headers=header)
        r.raise_for_status()
        all_plugins = r.json()
        platform_plugins = []
        current_os = None

        if sys.platform == "win32":
            current_os = utils.OSType.WIN
        elif sys.platform == "darwin":
            current_os = utils.OSType.OSX
        else:
            current_os = utils.OSType.LINUX

        for plugin in all_plugins:
            plugin_os = plugin["os"]
            if  plugin_os == utils.OSType.ALL or plugin_os == current_os:
                platform_plugins.append(plugin)

        return platform_plugins

    def install_plugin(self, name):
        pass

