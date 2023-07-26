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
    def __init__(self, lazy_load:bool = True):
        dataprocess.CIkomiaRegistry.__init__(self)

        if not lazy_load:
            self.load_algorithms()

    def __repr__(self):
        return f"IkomiaRegistry()"

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
        if s is None:
            raise ConnectionError("Failed to get online algorithms from Ikomia HUB.")

        url = config.main_cfg["hub"]["url"] + "/api/plugin/"
        r = s.session.get(url)
        r.raise_for_status()
        all_plugins = r.json()
        platform_plugins = []

        for plugin in all_plugins:
            if self._check_compatibility(plugin):
                platform_plugins.append(plugin)

        return platform_plugins

    def create_algorithm(self, name:str, parameters=None, hub:bool=True):
        """
        Instanciate algorithm from its unique name. See :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_algorithms` or
        :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_online_algorithms` to get valid names.
        If algorithm is already in the registry, an object instance is directly returned. Otherwise,
        the function tries to install it from Ikomia HUB and add it to the registry if installation success.
        Finally the object instance is returned.

        Args:
            name (str): unique algorithm name
            parameters (~ikomia.core.CWorkflowTaskParam): initial parameters values
            hub (bool): if False, we don't try to install algorithm from Ikomia Hub

        Returns:
            :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived: algorithm instance
        """
        if not isinstance(name, str):
            raise TypeError("Algorithm name required: name parameter must be a string")

        algo = None
        available_algos = self.get_algorithms()

        if name in available_algos:
            algo = self.create_instance(name, parameters)
        else:
            try:
                algo_dir, language = self._get_algorithm_directory(name)
                self._load_algorithm(name, algo_dir, language)
                algo = self.create_instance(name, parameters)
            except Exception as e:
                logger.warning(e)

                # If algorithm is installed but not functional (algo_dir is not empty), it may be a plugin
                # in developpement and we should not overwrite it with the Ikomia Hub version
                if hub and not algo_dir:
                    try:
                        logger.warning(f"Try installing {name} from Ikomia HUB...")
                        self.install_algorithm(name)
                        algo = self.create_instance(name, parameters)
                    except Exception as e:
                        logger.error(e)

        return algo

    def update_algorithms(self):
        """
        Launch automatic update of all algorithms in the registry. It only concerns algorithms of Ikomia HUB.
        The function checks version compatibility.
        """
        local_algos = self.get_algorithms()
        for algo in local_algos:
            info = self.get_algorithm_info(algo)
            if not info.internal:
                self.update_algorithm(algo)

    def update_algorithm(self, name:str):
        """
        Launch update of the given algorithm. It only concerns algorithms of Ikomia HUB.
        The function checks version compatibility.

        Args:
             name (str): algorithm unique name
        """
        local_algos = self.get_algorithms()
        if local_algos is None:
            logger.error("Ikomia algorithm registry is empty.")

        if name not in local_algos:
            logger.error(f"Algorithm {name} can't be updated as it is not installed.")
            return

        try:
            online_algos = self.get_online_algorithms()
        except Exception as e:
            logger.error(e)
            return

        online_algo = None
        for algo in online_algos:
            if algo["name"] == name:
                online_algo = algo
                break

        if online_algo is None:
            logger.error(f"Algorithm {name} does not exist in Ikomia HUB")
            return

        info = self.get_algorithm_info(name)
        if info.version >= online_algo["version"] and info.ikomia_version >= online_algo["ikomiaVersion"]:
            logger.info(f"Algorithm {name} is already up to date")
        else:
            self.install_algorithm(name, force=True)

    def install_algorithm(self, name:str, force:bool=False):
        """
        Launch algorithm installation from Ikomia HUB given its unique name.

        Args:
            name (str): algorithm unique name
            force (bool): force package installation even if the algorithm is already installed
        """
        available_algos = self.get_algorithms()
        update = False

        if name in available_algos:
            if not force:
                logger.info(f"Skip installation of {name} as it is already installed.")
                return
            else:
                update = True

        # Download package
        try:
            plugin, language, algo_dir = self._download_algorithm(name)
        except Exception as e:
            logger.error(f"Failed to install algorithm {name} for the following reason:")
            logger.error(e)
            return

        # Install requirements
        logger.info(f"Installing {name} requirements. This may take a while, please be patient...")
        utils.plugintools.install_requirements(algo_dir)
        self._check_installed_modules(algo_dir)

        # Load it
        self._load_algorithm(name, algo_dir, language)
        if language == utils.ApiLanguage.CPP and update:
            logger.warning(f"C++ algorithm {plugin['name']} can't be reloaded at runtime. "
                           f"It will be updated on next start.")

        if config.main_cfg["registry"]["auto_completion"]:
            autocomplete.update_local_plugin(name)

    def _get_algorithm_directory(self, name:str):
        # C++ or Python algorithm?
        cpp_algo_dir = os.path.join(self.get_plugins_directory(), "C++", name)
        python_algo_dir = os.path.join(self.get_plugins_directory(), "Python", name)

        if os.path.isdir(cpp_algo_dir):
            return cpp_algo_dir, utils.ApiLanguage.CPP
        elif os.path.isdir(python_algo_dir):
            return python_algo_dir, utils.ApiLanguage.PYTHON
        else:
            return "", None

    def _load_algorithm(self, name:str, directory:str, language:utils.ApiLanguage):
        if not os.path.isdir(directory):
            raise RuntimeError(f"Algorithm {name} is not installed.")

        if language == utils.ApiLanguage.PYTHON:
            self.load_python_algorithm(directory)
        elif language == utils.ApiLanguage.CPP:
            self.load_cpp_algorithm(directory)
        else:
            raise RuntimeError(f"Unsupported language for algorithm {name}.")

    def _download_algorithm(self, name:str):
        available_plugins = self.get_online_algorithms()

        plugin_info = None
        for plugin in available_plugins:
            if plugin["name"] == name:
                plugin_info = plugin
                break

        if plugin_info is None:
            error_msg = f"Algorithm {name} does not exist in Ikomia HUB."
            raise ValueError(error_msg)

        language = utils.ApiLanguage.CPP if plugin["language"] == 0 else utils.ApiLanguage.PYTHON
        state = utils.get_compatibility_state(plugin["ikomiaVersion"], language)

        if state != utils.PluginState.VALID:
            error_msg = f"Plugin {plugin['name']} can't be installed due to version incompatibility.\n"\
                        f"Based on Ikomia {plugin['ikomiaVersion']} " \
                        f"while the current version is {utils.get_api_version()}."
            raise ValueError(error_msg)

        # Get plugin package url
        s = ikomia.ik_api_session
        url = config.main_cfg["hub"]["url"] + "/api/plugin/" + str(plugin_info["id"]) + "/package/"
        r = s.session.get(url)
        r.raise_for_status()
        package_info = r.json()
        package_url = package_info["packageFile"]

        if package_url is None:
            raise RuntimeError(f"Failed to get algorithm package for {name}.")

        # Download package
        url = config.main_cfg["hub"]["url"] + package_url
        file_path = os.path.join(self.get_plugins_directory(), "Transfer", os.path.basename(package_url))
        utils.http.download_file(url, file_path, public=True)

        # Unzip
        language_folder = "C++" if language == utils.ApiLanguage.CPP else "Python"
        target_dir = os.path.join(self.get_plugins_directory(), language_folder, plugin["name"])

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
            return utils.check_architecture_keywords(plugin["keywords"])

        return True

    def _check_installed_modules(self, algo_dir:str):
        modules = utils.plugintools.get_installed_modules()

        # Uninstall blacklisted packages (conflicting with already bundle packages in Ikomia API)
        to_remove = self.get_black_listed_packages()
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
        needless_path = os.path.join(algo_dir, "needless.txt")
        if os.path.exists(needless_path):
            with open(needless_path, "r") as f:
                for line in f:
                    utils.plugintools.uninstall_package(line.rstrip())
