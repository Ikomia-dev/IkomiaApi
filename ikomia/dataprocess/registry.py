import ikomia
from ikomia import utils, dataprocess
from ikomia.core import config
import os
import sys
import logging
import zipfile
import shutil

logger = logging.getLogger(__name__)


class IkomiaRegistry(dataprocess.CIkomiaRegistry):

    def __init__(self):
        dataprocess.CIkomiaRegistry.__init__(self)
        plugin_folder = config.main_cfg["registry"]["path"]
        self.setPluginsDirectory(plugin_folder)
        self._load_python_plugins()

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

    def create_algorithm(self, name, parameters=None):
        algo = None
        available_algos = self.getAlgorithms()

        if name in available_algos:
            algo = self.createInstance(name, parameters)
        else:
            try:
                self._install_plugin(name)
                algo = self.createInstance(name, parameters)
            except ValueError as val_e:
                logger.error(val_e)
            except ConnectionRefusedError as conn_e:
                logger.error(conn_e)

        return algo

    def update_algorithms(self):
        local_algos = self.getAlgorithms()
        for algo in local_algos:
            info = self.getAlgorithmInfo(algo)
            if not info.internal:
                self.update_algorithm(algo)

    def update_algorithm(self, name):
        local_algos = self.getAlgorithms()
        if name not in local_algos:
            logger.error("Plugin " + name + " can't be updated as it is not installed.")
            return

        online_algos = self.get_online_algorithms()
        online_algo = None

        for algo in online_algos:
            if algo["name"] == name:
                online_algo = algo
                break

        if online_algo is None:
            logger.error("Plugin " + name + " does not exist in the Ikomia Marketplace")
            return

        info = self.getAlgorithmInfo(name)
        if info.version >= online_algo["version"] and info.ikomia_version >= online_algo["ikomiaVersion"]:
            logger.info("Plugin " + name + " is already up to date")
            return

        self._install_plugin(name)

    def _install_plugin(self, name):
        # Download package
        plugin, language, plugin_dir = self._download_plugin(name)

        # Install requirements
        utils.plugintools.install_requirements(plugin_dir)

        # Load it
        if language == utils.ApiLanguage.PYTHON:
            self._load_python_plugin(plugin_dir, plugin["name"])
        else:
            self.loadCppPlugin(plugin_dir)

    def _load_python_plugins(self):
        root_dir = self.getPluginsDirectory() + os.sep + "Python"

        if root_dir not in sys.path:
            sys.path.insert(0, root_dir)

        for root, subdirs, files in os.walk(root_dir, topdown=True):
            subdirs[:] = [d for d in subdirs if not d[0] == '.']
            for directory in subdirs:
                try:
                    self._load_python_plugin(root + os.sep + directory, directory)
                except Exception as e:
                    logger.error(e)
            break

    def _load_python_plugin(self, directory, name):
        main_module_name = name + "." + name
        main_module = utils.import_plugin_module(directory, main_module_name)
        main_class = getattr(main_module, name)
        main_obj = main_class()
        task_factory = main_obj.getProcessFactory()

        plugin_version = task_factory.info.ikomia_version
        compatibility_state = utils.getCompatibilityState(plugin_version, utils.ApiLanguage.PYTHON)

        if compatibility_state == utils.PluginState.DEPRECATED:
            error_msg = "Plugin " + name + "is deprecated: based on Ikomia " + str(plugin_version) + \
                        "while the current version is " + str(utils.getApiVersion())
            raise ValueError(error_msg)
        elif compatibility_state == utils.PluginState.UPDATED:
            logger.warning("Plugin " + name + "is based on Ikomia " + str(plugin_version) +
                           " while the current version is " + str(utils.getApiVersion()) +
                           ". You should consider updating Ikomia API.")

        self.registerTask(task_factory)

    def _download_plugin(self, name):
        available_plugins = self.get_online_algorithms()
        plugin_info = None

        for plugin in available_plugins:
            if plugin["name"] == name:
                plugin_info = plugin
                break

        if plugin_info is None:
            error_msg = "Plugin " + name + " does not exist in the Ikomia Marketplace"
            raise ValueError(error_msg)

        language = utils.ApiLanguage.CPP if plugin["language"] == 0 else utils.ApiLanguage.PYTHON
        state = utils.getCompatibilityState(plugin["ikomiaVersion"], language)

        if state != utils.PluginState.VALID:
            error_msg = "Plugin " + plugin["name"] + " can't be installed due to version incompatibility.\n"
            error_msg += "Based on Ikomia " + plugin["version"] + " while the current version is " + utils.getApiVersion()
            raise ValueError(error_msg)

        s = ikomia.api_session
        if s.token is None:
            error_msg = "Online algorithms retrieval failed, authentication required."
            raise ConnectionRefusedError(error_msg)

        # Get plugin package url
        url = config.main_cfg["marketplace"]["url"] + "/api/plugin/" + str(plugin_info["id"]) + "/package/"
        r = s.session.get(url)
        r.raise_for_status()
        package_info = r.json()
        package_url = package_info["packageFile"]

        # Download package
        url = config.main_cfg["marketplace"]["url"] + package_url
        file_path = self.getPluginsDirectory() + "/Transfer/" + os.path.basename(package_url)
        utils.http.download_file(url, file_path, public=False)

        # Unzip
        language_folder = "C++" if language == utils.ApiLanguage.CPP else "Python"
        target_dir = self.getPluginsDirectory() + os.sep + language_folder + os.sep + plugin["name"]

        if os.path.isdir(target_dir):
            shutil.rmtree(target_dir)

        with zipfile.ZipFile(file_path, "r") as zip_tool:
            zip_tool.extractall(target_dir)

        if os.path.isfile(file_path):
            os.remove(file_path)

        target_dir = utils.conform_plugin_directory(target_dir, plugin)
        return plugin, language, target_dir