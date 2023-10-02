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
import platform
import semver
import re
import time
from datetime import datetime

logger = logging.getLogger(__name__)


class IkomiaRegistry(dataprocess.CIkomiaRegistry):
    """
    Registry for all Ikomia algorithms (built-in and Ikomia HUB). It stores all algorithms references and allows to
    install, update and instanciate any of these algorithms.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CIkomiaRegistry`.
    """
    def __init__(self, lazy_load:bool=True):
        dataprocess.CIkomiaRegistry.__init__(self)
        self.public_online_algos = None
        self.private_online_algos = None

        if not lazy_load:
            self.load_algorithms()

    def __repr__(self):
        return f"IkomiaRegistry()"

    def get_public_hub_algorithms(self, force:bool=False):
        """
        Get the list of available algorithms from public Ikomia HUB.
        Each algorithm is identified by a unique name.
        Each algorithm can then be instanciated from this name with the function
        :py:meth:`~ikomia.dataprocess.registry.IkomiaRegistry.create_algorithm`.

        Args:
            force (bool): IkomiaRegistry class use cache for online algorithms. Set this parameter to True to force online list update.

        Returns:
             list of dict: list of algorithms information
        """
        if ikomia.ik_api_session is None:
            raise ConnectionError("Failed to get online algorithms from Ikomia HUB.")

        if self.public_online_algos is None or force:
            url = config.main_cfg["hub"]["url"] + "/v1/hub/"
            self.public_online_algos = self._get_online_algos(url)

        return self.public_online_algos

    def get_private_hub_algorithms(self, force:bool=False):
        """
        Get the list of available algorithms from private Ikomia HUB (authentication required).
        Each algorithm is identified by a unique name.
        Each algorithm can then be instanciated from this name with the function
        :py:meth:`~ikomia.dataprocess.registry.IkomiaRegistry.create_algorithm`.

        Args:
            force (bool): IkomiaRegistry class use cache for online algorithms. Set this parameter to True to force online list update.

        Returns:
             list of dict: list of algorithms information
        """
        s = ikomia.ik_api_session
        if s.token is None:
            raise PermissionError("You must be authenticated to get private algorithms")

        if self.private_online_algos is None or force:
            url = config.main_cfg["hub"]["url"] + "/v1/algos/"
            self.private_online_algos = self._get_online_algos(url)

        return self.private_online_algos

    def _get_online_algos(self, url: str):
        s = ikomia.ik_api_session
        algos = self._get_all_from_pagination(url)
        valid_algos = []

        for algo in algos:
            # Get algorithm details
            r = s.session.get(algo["url"])
            r.raise_for_status()
            algo_detail = r.json()

            if self._check_compatibility(algo_detail):
                valid_algos.append(algo_detail)

        return valid_algos

    def _get_all_from_pagination(self, url: str):
        s = ikomia.ik_api_session
        r = s.session.get(url)
        r.raise_for_status()
        pagination_data = r.json()

        if pagination_data["next"] is not None:
            url = f"{url}?page_size={pagination_data['count']}"
            r = s.session.get(url)
            r.raise_for_status()
            pagination_data = r.json()

        return pagination_data["results"]

    def create_algorithm(self, name:str, parameters=None, public_hub:bool=True, private_hub=False):
        """
        Instanciate algorithm from its unique name. See :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_algorithms` or
        :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_public_hub_algorithms` or
        :py:meth:`~ikomia.dataprocess.IkomiaRegistry.get_private_hub_algorithms` to get valid names.
        If algorithm is already in the registry, an object instance is directly returned. Otherwise,
        the function tries to install it from public Ikomia HUB first, and private Ikomia HUB at the end. It finally
        adds it to the registry if installation succeed and returns the object instance.

        Args:
            name (str): unique algorithm name
            parameters (~ikomia.core.CWorkflowTaskParam): initial parameters values
            public_hub (bool): try to install algorithm from public Ikomia Hub if True
            private_hub (bool): try to install algorithm from private Ikomia Hub if True

        Returns:
            :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived: algorithm instance
        """
        if not isinstance(name, str):
            raise TypeError("Algorithm name required: name parameter must be a string")

        algo = None
        available_algos = self.get_algorithms()

        if name in available_algos:
            # Algorithm is loaded, instanciate it
            algo = self.create_instance(name, parameters)
        else:
            try:
                # Algorithm is not loaded (lazy load), check if folder is found locally and try to load it
                algo_dir, language = self._get_algorithm_directory(name)
                self._load_algorithm(name, algo_dir, language)
                algo = self.create_instance(name, parameters)
            except Exception as e:
                logger.debug(e)

                if (public_hub or private_hub) and not algo_dir:
                    # Algorithm is not installed, so try to install it from HUB
                    logger.warning(f"Try installing {name} from Ikomia HUB...")
                    self.install_algorithm(name, public_hub, private_hub)
                    algo = self.create_instance(name, parameters)
                else:
                    # If algorithm is installed locally but not functional (algo_dir is not empty), it may be a plugin
                    # in developpement and we should not overwrite it with the Ikomia Hub version
                    raise RuntimeError(f"Algorithm {name} is installed locally but not functional. "
                                       f"Check your code or your Python environment please.")

        return algo

    def update_algorithms(self, public_hub:bool=True, private_hub:bool=False):
        """
        Launch automatic update of all algorithms in the registry. It only concerns algorithms of Ikomia HUB.
        The function checks version compatibility.

        Args:
            public_hub (bool): update public algorithms from Ikomia HUB if True
            private_hub (bool): update private algorithms from Ikomia HUB if True
        """
        local_algos = self.get_algorithms()
        algo_count = 0

        for algo in local_algos:
            info = self.get_algorithm_info(algo)
            if not info.internal:
                self.update_algorithm(algo, public_hub, private_hub)
                algo_count += 1

        logger.info(f"{algo_count} algorithms have been updated successfully")

    def update_algorithm(self, name:str, public_hub:bool=True, private_hub:bool=False):
        """
        Launch update of the given algorithm. It only concerns algorithms from Ikomia HUB. In case where algorithm name
        is both available in public and private HUBs, you should set parameters public_hub and private_hub to
        explicitly set the source. If both are set to True, the function will update local version from public HUB.

        Args:
             name (str): algorithm unique name
             public_hub (bool): update public algorithm from Ikomia HUB if True
             private_hub (bool): update private algorithm from Ikomia HUB if True
        """
        local_algos = self.get_algorithms()
        if local_algos is None:
            logger.error("Ikomia algorithm registry is empty.")

        if name not in local_algos:
            try:
                # Lazy loading
                algo_dir, language = ikomia.ik_registry._get_algorithm_directory(name)
                self._load_algorithm(name, algo_dir, language)
            except:
                logger.error(f"Algorithm {name} can't be updated as it is not installed.")
                return

        if public_hub:
            hub_algo = self._find_hub_algo(name, self.get_public_hub_algorithms())
            if hub_algo:
                if self._has_to_be_updated(name, hub_algo):
                    self.install_algorithm(name, force=True, public_hub=True, private_hub=False)
                else:
                    logger.info(f"Algorithm {name} is already up to date")
                return

        if private_hub:
            hub_algo = self._find_hub_algo(name, self.get_private_hub_algorithms())
            if hub_algo:
                if self._has_to_be_updated(name, hub_algo):
                    self.install_algorithm(name, force=True, public_hub=False, private_hub=True)
                else:
                    logger.info(f"Algorithm {name} is already up to date")
                return

        raise RuntimeError(f"Algorithm {name} does not exist in Ikomia HUB")

    def _has_to_be_updated(self, name:str, hub_info: dict):
        local_info = self.get_algorithm_info(name)
        current_version = semver.Version.parse(local_info.version)

        if "version" in hub_info:
            # Public algorithm
            hub_version = semver.Version.parse(hub_info["version"])
            return current_version < hub_version
        else:
            # Private algorithm
            hub_modif_date = datetime.strptime(hub_info["updated_at"], "%Y-%m-%dT%H:%M:%S.%fZ")
            algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(name)
            local_time = time.localtime(os.path.getmtime(os.path.join(algo_dir, f"{name}_process.py")))
            local_format_time = time.strftime("%Y-%m-%dT%H:%M:%S", local_time)
            local_modif_date = datetime.strptime(local_format_time, "%Y-%m-%dT%H:%M:%S")
            return hub_modif_date > local_modif_date

    def _find_hub_algo(self, name: str, hub_list: list):
        for algo in hub_list:
            if algo["name"] == name:
                return algo

        return None

    def install_algorithm(self, name:str, public_hub: bool = True, private_hub: bool = False, force:bool=False):
        """
        Launch algorithm installation from Ikomia HUB given its unique name.

        Args:
            name (str): algorithm unique name
            public_hub (bool): search algorithm in public Ikomia HUB if True
            private_hub (bool): search algorithm in private Ikomia HUB if True
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
        plugin, language, algo_dir = self._download_algorithm(name, public_hub, private_hub)

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

    def _download_algorithm(self, name:str, public_hub: bool = True, private_hub: bool = False):
        algo_info = None

        if public_hub:
            public_algos = self.get_public_hub_algorithms()
            algo_info = self._find_hub_algo(name, public_algos)

        if private_hub and algo_info is None:
            try:
                private_algos = self.get_private_hub_algorithms()
                algo_info = self._find_hub_algo(name, private_algos)
            except Exception as e:
                raise RuntimeError(f"Failed to search for {name} algorithm in private Ikomia HUB for the following reason: {e}")

        if algo_info is None:
            error_msg = f"Algorithm {name} does not exist in Ikomia HUB or is not compatible with your environment."
            raise ValueError(error_msg)

        # Get algorithm package url
        package_url = self._find_best_package_url(algo_info["packages"])
        if package_url is None:
            raise RuntimeError(f"Failed to get algorithm package for {name}.")

        # Download package
        file_path = os.path.join(self.get_plugins_directory(), "Transfer", f"{name}.zip")
        try:
            utils.http.download_file(f"{package_url}download/", file_path, ikomia.ik_api_session)
        except Exception as e:
            raise RuntimeError(f"Failed to download algorithm {name} for the following reason: {e}")

        # Unzip
        language = utils.ApiLanguage.PYTHON if algo_info["language"] == "PYTHON" else utils.ApiLanguage.CPP
        language_folder = "C++" if language == utils.ApiLanguage.CPP else "Python"
        target_dir = os.path.join(self.get_plugins_directory(), language_folder, algo_info["name"])

        if os.path.isdir(target_dir):
            shutil.rmtree(target_dir)

        with zipfile.ZipFile(file_path, "r") as zip_tool:
            zip_tool.extractall(target_dir)

        if os.path.isfile(file_path):
            os.remove(file_path)

        return algo_info, language, target_dir

    def _find_best_package_url(self, packages:list):
        """
        At this point, plugins are already filtered to ensure compatibility.
        So for now, we just want to pick the latest version
        """
        url = None
        last_version = semver.Version.parse("0.0.0")

        for package in packages:
            if "version" in package:
                # Public hub algorithm
                package_version = semver.Version.parse(package["version"])
                if package_version > last_version:
                    url = package["url"]
                    last_version = package_version
            elif "tag" in package:
                # Private hub algorithm
                url = package["url"]

        return url

    @staticmethod
    def _check_compatibility(algo):
        language = utils.ApiLanguage.PYTHON if algo["language"] == "PYTHON" else utils.ApiLanguage.CPP
        packages = algo["packages"]

        for package in packages:
            if IkomiaRegistry._check_package_compatibility(package, language):
                return True

        return False

    @staticmethod
    def _check_package_compatibility(package, language):
        if (
                IkomiaRegistry._check_os_compatibility(package) and
                IkomiaRegistry._check_ikomia_compatibility(package, language) and
                IkomiaRegistry._check_python_compatibility(package) and
                IkomiaRegistry._check_architecture(package, language)
        ):
            return True
        else:
            return False

    @staticmethod
    def _check_os_compatibility(package):
        current_os = None
        if sys.platform == "win32":
            current_os = "WINDOWS"
        elif sys.platform == "darwin":
            current_os = "OSX"
        else:
            current_os = "LINUX"

        os_list = package["platform"]["os"]
        return current_os in os_list

    @staticmethod
    def _check_ikomia_compatibility(package, language):
        ikomia_condition = package["platform"]["ikomia"]
        min_version, max_version = IkomiaRegistry._split_min_max_version(ikomia_condition)
        state = utils.get_compatibility_state(min_version, max_version, language)
        return state == utils.PluginState.VALID

    @staticmethod
    def _check_python_compatibility(package):
        current_version = platform.python_version()
        min_version, max_version = IkomiaRegistry._split_min_max_version(package["platform"]["python"])
        current_sem_ver = semver.Version.parse(current_version)

        if min_version:
            min_sem_ver = semver.Version.parse(min_version, optional_minor_and_patch=True)
            if current_sem_ver < min_sem_ver:
                return False

        if max_version:
            max_sem_ver = semver.Version.parse(max_version, optional_minor_and_patch=True)
            return current_sem_ver < max_sem_ver

        return True

    @staticmethod
    def _check_architecture(package, language):
        if language == utils.ApiLanguage.CPP:
            # TODO: test it!
            # Check CPU architecture
            current_arch = utils.get_cpu_arch_name(utils.get_cpu_arch())
            compatible_arch_list = package["platform"]["architecture"]

            if current_arch not in compatible_arch_list:
                return False

            # Check features: CUDA version
            current_features = utils.get_cuda_version()
            compatible_features = package["platform"]["features"]

            for feat in current_features:
                if feat not in compatible_features:
                    return False

            return True
        else:
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

    @staticmethod
    def _split_min_max_version(version: str):
        match = re.search(r">=(\d+\.\d+\.?\d*),?<?(\d*\.?\d*\.?\d*)", version)
        if match:
            return match.group(1), match.group(2)
        else:
            raise RuntimeError("Invalid version condition string")
