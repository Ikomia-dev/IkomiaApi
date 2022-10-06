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
Module dedicated to algorithms management from the Ikomia platform.
It implements IkomiaRegistry class that offers features to install, update and instanciate
algorithms from the built-in environment or Ikomia HUB.
"""
import ikomia
from ikomia import utils, dataprocess
from ikomia.utils import autocomplete
from ikomia.core import config
import os
import sys
import logging
import zipfile
import shutil

logger = logging.getLogger(__name__)


class IkomiaRegistry(dataprocess.CIkomiaRegistry):
    """
    Registry for all Ikomia algorithms (built-in and Ikomia HUB). It stores all algorithms references and allows to
    install, update and instanciate any of these algorithms.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CIkomiaRegistry`.
    """
    def __init__(self):
        dataprocess.CIkomiaRegistry.__init__(self)
        self.load_python_algorithms()

    @utils.http.http_except
    def get_online_algorithms(self):
        """
        Get the list of available algorithms from Ikomia HUB.
        Each algorithm is identified by a unique name.
        Each algorithm can then be instanciated from this name with the function
        :py:meth:`~ikomia.dataprocess.registry.IkomiaRegistry.create_algorithm`.

        Returns:
             list of dict: list of algorithms information
        """
        s = ikomia.ik_api_session
        if s is None or s.token is None:
            logger.error("Failed to get online algorithms from Ikomia HUB, authentication required.")
            return None

        url = config.main_cfg["hub"]["url"] + "/api/plugin/"
        r = s.session.get(url)
        r.raise_for_status()
        all_plugins = r.json()
        platform_plugins = []

        for plugin in all_plugins:
            if self._check_compatibility(plugin):
                platform_plugins.append(plugin)

        return platform_plugins

    def create_algorithm(self, name, parameters=None):
        """
        Instanciate algorithm from its unique name. See :py:meth:`~ikomia.dataprocess.IkomiaRegistry.getAlgorithms` or
        :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_online_algorithms` to get valid names.
        If algorithm is already in the registry, an object instance is directly returned. Otherwise,
        the function tries to install it from Ikomia HUB and add it to the registry if installation success.
        Finally the object instance is returned.

        Args:
            name (str): unique algorithm name
            parameters (~ikomia.core.CWorkflowTaskParam): initial parameters values

        Returns:
            :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived: algorithm instance
        """
        algo = None
        available_algos = self.getAlgorithms()

        if name in available_algos:
            algo = self.createInstance(name, parameters)
        else:
            try:
                self.install_algorithm(name)
                algo = self.createInstance(name, parameters)

                if config.main_cfg["registry"]["auto_completion"]:
                    autocomplete.update_local_plugin(algo)
            except ValueError as val_e:
                logger.error(val_e)
            except ConnectionRefusedError as conn_e:
                logger.error(conn_e)

        return algo

    def update_algorithms(self):
        """
        Launch automatic update of all algorithms in the registry. It only concerns algorithms of Ikomia HUB.
        The function checks version compatibility.
        """
        local_algos = self.getAlgorithms()
        for algo in local_algos:
            info = self.getAlgorithmInfo(algo)
            if not info.internal:
                self.update_algorithm(algo)

    def update_algorithm(self, name):
        """
        Launch update of the given algorithm. It only concerns algorithms of Ikomia HUB.
        The function checks version compatibility.

        Args:
             name (str): algorithm unique name
        """
        local_algos = self.getAlgorithms()
        if local_algos is None:
            logger.error("Ikomia algorithm registry is empty.")

        if name not in local_algos:
            logger.error("Algorithm " + name + " can't be updated as it is not installed.")
            return

        online_algos = self.get_online_algorithms()
        if online_algos is None:
            return

        online_algo = None
        for algo in online_algos:
            if algo["name"] == name:
                online_algo = algo
                break

        if online_algo is None:
            logger.error("Algorithm " + name + " does not exist in Ikomia HUB")
            return

        info = self.getAlgorithmInfo(name)
        if info.version >= online_algo["version"] and info.ikomiaVersion >= online_algo["ikomiaVersion"]:
            logger.info("Algorithm " + name + " is already up to date")
            return

        self.install_algorithm(name, force=True)

    def install_algorithm(self, name, force=False):
        """
        Launch algorithm installation from Ikomia HUB given its unique name.

        Args:
            name (str): algorithm unique name
            force (bool): force package installation even if the algorithm is already installed
        """
        available_algos = self.getAlgorithms()
        if name in available_algos and not force:
            logger.info(f"Skip installation of {name} as it is already installed.")
            return

        # Download package
        plugin, language, plugin_dir = self._download_algorithm(name)

        # Install requirements
        logger.info(f"Installing {name} requirements. This may take a while, please be patient...")
        utils.plugintools.install_requirements(plugin_dir)
        self._check_installed_modules(plugin_dir)

        # Load it
        if language == utils.ApiLanguage.PYTHON:
            self._load_python_algorithm(plugin_dir, plugin["name"])
        else:
            self.loadCppPlugin(plugin_dir)

    def load_python_algorithms(self):
        root_dir = self.getPluginsDirectory() + os.sep + "Python"

        if root_dir not in sys.path:
            sys.path.insert(0, root_dir)

        for root, subdirs, files in os.walk(root_dir, topdown=True):
            subdirs[:] = [d for d in subdirs if not d[0] == '.']
            for directory in subdirs:
                try:
                    self._load_python_algorithm(root + os.sep + directory, directory)
                except Exception as e:
                    logger.warning(f"Algorithm {directory} not loaded: {e}")
            break

    def _load_python_algorithm(self, directory, name):
        main_module_name = name + "." + name
        main_module = utils.import_plugin_module(directory, main_module_name)
        main_class = getattr(main_module, "IkomiaPlugin")
        main_obj = main_class()
        task_factory = main_obj.getProcessFactory()
        task_factory.info.language = utils.ApiLanguage.PYTHON
        task_factory.info.os = utils.OSType.ALL
        task_factory.info.internal = False

        plugin_version = task_factory.info.ikomiaVersion
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

    def _download_algorithm(self, name):
        available_plugins = self.get_online_algorithms()
        if available_plugins is None:
            raise RuntimeError(f"Download algorithm {name} failed. Please check connection and authentication.")

        plugin_info = None

        for plugin in available_plugins:
            if plugin["name"] == name:
                plugin_info = plugin
                break

        if plugin_info is None:
            error_msg = "Algorithm " + name + " does not exist in the Ikomia HUB"
            raise ValueError(error_msg)

        language = utils.ApiLanguage.CPP if plugin["language"] == 0 else utils.ApiLanguage.PYTHON
        state = utils.getCompatibilityState(plugin["ikomiaVersion"], language)

        if state != utils.PluginState.VALID:
            error_msg = "Plugin " + plugin["name"] + " can't be installed due to version incompatibility.\n"
            error_msg += "Based on Ikomia " + plugin["ikomiaVersion"] + " while the current version is " + utils.getApiVersion()
            raise ValueError(error_msg)

        s = ikomia.ik_api_session
        if s.token is None:
            error_msg = "Online algorithms retrieval failed, authentication required."
            raise ConnectionRefusedError(error_msg)

        # Get plugin package url
        url = config.main_cfg["hub"]["url"] + "/api/plugin/" + str(plugin_info["id"]) + "/package/"
        r = s.session.get(url)
        r.raise_for_status()
        package_info = r.json()
        package_url = package_info["packageFile"]

        # Download package
        url = config.main_cfg["hub"]["url"] + package_url
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

    @staticmethod
    def _check_compatibility(plugin):
        current_os = None
        if sys.platform == "win32":
            current_os = utils.OSType.WIN
        elif sys.platform == "darwin":
            current_os = utils.OSType.OSX
        else:
            current_os = utils.OSType.LINUX

        plugin_os = plugin["os"]
        if plugin_os != utils.OSType.ALL and plugin_os != current_os:
            return False

        language = utils.ApiLanguage.CPP if plugin["language"] == 0 else utils.ApiLanguage.PYTHON
        if language == utils.ApiLanguage.CPP:
            return utils.checkArchitectureKeywords(plugin["keywords"])

        return True

    def _check_installed_modules(self, plugin_dir):
        modules = utils.plugintools.get_installed_modules()

        # Uninstall blacklisted packages (conflicting with already bundle packages in Ikomia API)
        to_remove = self.getBlackListedPackages()
        for package in to_remove:
            module_installed = next((mod for mod in modules if mod["name"] == package), None)
            if module_installed:
                if package == "tb-nightly":
                    utils.plugintools.uninstall_package(package)
                    tb_installed = next((mod for mod in modules if mod["name"] == "tensorboard"), None)

                    if tb_installed:
                        utils.plugintools.uninstall_package("tensorboard")
                        utils.plugintools.install_package("tensorboard", tb_installed["version"])
                else:
                    utils.plugintools.uninstall_package(package)

        # Remove plugin specific blacklisted packages
        needless_path = os.path.join(plugin_dir, "needless.txt")
        if os.path.exists(needless_path):
            with open(needless_path, "r") as f:
                for line in f:
                    utils.plugintools.uninstall_package(line.rstrip())
