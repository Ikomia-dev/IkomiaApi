import ikomia
from ikomia import utils, dataprocess
from ikomia.core import config
import os
import sys
import importlib
import logging
import zipfile

logger = logging.getLogger(__name__)


class IkomiaRegistry(dataprocess.CIkomiaRegistry):

    def __init__(self):
        dataprocess.CIkomiaRegistry.__init__(self)
        self._load_plugins()

    @utils.http.http_except
    def get_online_algorithms(self):
        s = ikomia.api_session
        if s.token is None:
            logger.error("Online algorithms retrieval failed, authentication required.")
            return

        url = config.main_cfg["marketplace"]["url"] + "/api/plugin/"
        r = s.session.get(url)
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
            if plugin_os == utils.OSType.ALL or plugin_os == current_os:
                platform_plugins.append(plugin)

        return platform_plugins

    def install_plugin(self, name):
        # Download package
        plugin = self._download_plugin(name)

        # Install requirements
        language_folder = "C++" if plugin["language"] == utils.ApiLanguage.CPP else "Python"
        plugin_dir = self.getPluginsDirectory() + os.sep + language_folder + os.sep + plugin["name"]
        utils.plugindeps.install_requirements(plugin_dir)

        # Load it

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
                compatibility_state = utils.getCompatibilityState(plugin_version, utils.ApiLanguage.PYTHON)

                if compatibility_state == utils.PluginState.DEPRECATED:
                    logger.error("Plugin " + directory + "is deprecated: based on Ikomia " + str(plugin_version) +
                                 "while the current version is " + str(utils.getApiVersion()))
                    continue
                elif compatibility_state == utils.PluginState.UPDATED:
                    logger.warning("Plugin " + directory + "is based on Ikomia " + str(plugin_version) +
                                   " while the current version is " + str(utils.getApiVersion()) +
                                   ". You should consider updating Ikomia API.")

                self.registerTask(task_factory)
            break

    def _download_plugin(self, name):
        available_plugins = self.get_online_algorithms()
        plugin_info = None

        for plugin in available_plugins:
            if plugin["name"] == name:
                plugin_info = plugin
                break

        if plugin_info is None:
            logger.error("Plugin " + name + "does not exist in the Ikomia Marketplace")
            return

        language = utils.ApiLanguage.CPP if plugin["language"] == "0" else utils.ApiLanguage.PYTHON
        state = utils.getCompatibilityState(plugin["ikomiaVersion"], language)

        if state != utils.PluginState.VALID:
            logger.error("Plugin " + plugin["name"] + "can't be installed due to version incompatibility.")
            logger.error("Based on Ikomia " + plugin["version"] + " while the current version is " + utils.getApiVersion())
            return

        s = ikomia.api_session
        if s.token is None:
            logger.error("Online algorithms retrieval failed, authentication required.")
            return

        # Get plugin package url
        url = config.main_cfg["marketplace"]["url"] + "/api/plugin/" + str(plugin_info["id"]) + "/package/"
        r = s.session.get(url)
        r.raise_for_status()
        package_info = r.json()
        package_url = package_info["packageFile"]

        # Download package
        url = config.main_cfg["marketplace"]["url"] + package_url
        file_path = self.getPluginsDirectory() + "/Transfer/" + os.path.basename(package_url)
        utils.http.download_file(url, file_path)

        # Unzip
        language_folder = "C++" if plugin["language"] == utils.ApiLanguage.CPP else "Python"
        target_dir = self.getPluginsDirectory() + os.sep + language_folder + os.sep + plugin["name"]

        with zipfile.ZipFile(file_path, "r") as zip_tool:
            zip_tool.extractall(target_dir)

        if os.path.isfile(file_path):
            os.remove(file_path)

        return plugin
