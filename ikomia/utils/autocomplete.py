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
The module autocomplete provides auto-completion capabilities to Ikomia API.
It generates automatically a Python module named ik.
The aim is to help users to discover algorithm names and parameters.
"""
import os
import re
import site
import keyword
import logging
import shutil
import importlib
import sys
import time
from ikomia import utils
from ikomia.core import CWorkflowTask  # pylint: disable=E0611
from ikomia.dataprocess.registry import ik_registry

try:
    from ikomia.utils import ik
    _ik_auto_complete = True
except ImportError:
    _ik_auto_complete = False

logger = logging.getLogger(__name__)


class AutoComplete:
    """
    Generation of auto-completion module.
    """
    def __init__(self):
        self.registry = ik_registry
        self.registry.register_event_callback("algorithm_changed", self.update_local_plugin)

    def _write_auto_complete(self, fp, task_name: str = "", task: CWorkflowTask = None, local: bool = True):
        if not task_name and task is None:
            raise RuntimeError("Auto-completion: parameters must include either a valid name or task instance.")

        forbid_char = r"\ |\-|\[|\]"
        parameters = {}

        if task is not None:
            task_name = task.name
        elif local:
            task = self.registry.create_algorithm(task_name, public_hub=False, private_hub=False)

        # Class definition
        class_name = re.sub(forbid_char, "", task_name)
        fp.write(f"class {class_name}:\n")

        if local:
            # Algorithm already installed
            parameters = task.get_parameters()
            if len(parameters) == 0:
                # __new__() return task object instance
                fp.write("    def __new__(cls):\n")
                fp.write(f"        algo = ik_registry.create_algorithm(\"{task_name}\", None)\n")
                fp.write("        return algo\n\n")
            else:
                function_params = ""
                params_dict = "{\n"

                for param in parameters:
                    if keyword.iskeyword(param):
                        param_var = f"{param}_"
                    else:
                        param_var = param

                    if function_params:
                        function_params += ", "

                    # Compute _new_() parameters
                    param_var = re.sub(forbid_char, "", param_var)
                    param_value = str(parameters[param]).replace("\\", "/")
                    function_params += f"{param_var}: str=\"{param_value}\""
                    # Compute parameters dict
                    params_dict += f"            \"{param}\": {param_var},\n"
                    # Static class variable for names
                    fp.write(f"    {param_var} = \"{param}\"\n")

                # __new__() return task object instance
                params_dict += "        }"
                fp.write(f"\n    def __new__(cls, {function_params}):\n")
                fp.write(f"        algo = ik_registry.create_algorithm(\"{task_name}\", None)\n")
                fp.write(f"        algo.set_parameters({params_dict})\n")
                fp.write("        return algo\n\n")
        else:
            # Algorithm from Ikomia Hub
            # __new__() return task object instance
            fp.write("    def __new__(cls, **kwargs):\n")
            fp.write(f"        algo = ik_registry.create_algorithm(\"{task_name}\", None)\n")
            fp.write("        if algo is not None:\n")
            fp.write("            params = {}\n")
            fp.write("            for arg in kwargs:\n")
            fp.write("                if isinstance(kwargs[arg], str):\n")
            fp.write("                    params[arg] = kwargs[arg]\n")
            fp.write("                else:\n")
            fp.write("                    params[arg] = str(kwargs[arg])\n\n")
            fp.write("            algo.set_parameters(params)\n\n")
            fp.write("        return algo\n\n")

        # name() return task name
        fp.write("    @staticmethod\n")
        fp.write("    def name():\n")
        fp.write(f"        return \"{task_name}\"\n\n")

    def _generate_python_file(self, folder: str):
        ik_file_path = os.path.join(folder, "ik.py")
        with open(ik_file_path, 'w+', encoding="utf-8") as f:
            # Imports
            f.write("from ikomia.dataprocess.registry import ik_registry\n\n")

            # Class definitions from cache files
            cache_file_path = os.path.join(folder, "autocomplete_local.cache")
            if os.path.isfile(cache_file_path):
                with open(cache_file_path, "r", encoding="utf-8") as f_cache:
                    shutil.copyfileobj(f_cache, f)

            cache_file_path = os.path.join(folder, "autocomplete_online.cache")
            if os.path.isfile(cache_file_path):
                with open(cache_file_path, "r", encoding="utf-8") as f_cache:
                    shutil.copyfileobj(f_cache, f)

        # Reload module
        if "ikomia.utils.ik" in sys.modules:
            importlib.reload(utils.ik)

    def _is_valid_python_plugin_folder(self, name: str, folder_path: str) -> bool:
        main_python_file = os.path.join(folder_path, f"{name}.py")
        return os.path.exists(main_python_file)

    def _is_valid_cpp_plugin_folder(self, name: str, folder_path: str) -> bool:
        del name
        for filename in os.listdir(folder_path):
            # Just check if at least one shared library exists -> maybe not sufficient...
            _, ext = os.path.splitext(filename)
            if ".so" in ext or ".dll" in ext:
                return True

        return False

    def _check_local_sync(self) -> bool:
        if not _ik_auto_complete:
            return False

        # Get local algorithm names from plugin directories
        local_folder_names = set()
        python_plugin_folder = os.path.join(self.registry.get_plugins_directory(), "Python")
        cpp_plugin_folder = os.path.join(self.registry.get_plugins_directory(), "C++")

        for name in os.listdir(python_plugin_folder):
            folder_path = os.path.join(python_plugin_folder, name)
            if os.path.isdir(folder_path) and self._is_valid_python_plugin_folder(name, folder_path):
                local_folder_names.add(name)

        for name in os.listdir(cpp_plugin_folder):
            folder_path = os.path.join(cpp_plugin_folder, name)
            if os.path.isdir(folder_path) and self._is_valid_cpp_plugin_folder(name, folder_path):
                local_folder_names.add(name)

        # Get auto-completion local list
        ik_names_set = set(ik.local_names)

        # Check changes
        if ik_names_set == local_folder_names:
            return True

        # Local plugins has changed
        diff_local = local_folder_names - ik_names_set

        # Try to load them -> auto-completion has to be updated if at least one is valid
        for name in diff_local:
            try:
                algo = self.registry.create_algorithm(name=name, public_hub=False, private_hub=False)
                if algo is not None:
                    return False
            except Exception as e:
                logger.debug(e)

        return True

    def _check_online_sync(self) -> bool:
        if not _ik_auto_complete:
            return False

        # Update every 4 hours because call to get_public_hub_algorithms() is time consuming
        ik_last_update = os.path.getmtime(ik.__file__)
        now = time.time()

        if now - ik_last_update < 14400:
            return True

        # Update modified time
        os.utime(ik.__file__)

        try:
            algos = self.registry.get_public_hub_algorithms()
        except Exception:
            # Connection to Ikomia HUB failed
            return True

        try:
            ik_names_set = set(ik.online_names)
            online_names_set = {algo["name"] for algo in algos}
            return ik_names_set == online_names_set
        except Exception:
            # Auto-completion cache is in not complete
            return False

    def _check_task_params(self, task: CWorkflowTask) -> bool:
        if not _ik_auto_complete:
            return False

        params = task.get_parameters()

        try:
            ik_class = getattr(ik, task.name)
        except Exception:
            return False

        ik_class_vars = vars(ik_class)

        for param_key in params:
            if param_key not in ik_class_vars:
                return False

        return True

    def _has_local_cache(self) -> bool:
        filename = "autocomplete_local.cache"
        cache_file_path1 = os.path.join(os.path.dirname(__file__), filename)
        local_site = os.path.join(site.getusersitepackages(), "ikomia", "utils")
        cache_file_path2 = os.path.join(local_site, filename)
        return os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2)

    def _has_online_cache(self) -> bool:
        filename = "autocomplete_online.cache"
        cache_file_path1 = os.path.join(os.path.dirname(__file__), filename)
        local_site = os.path.join(site.getusersitepackages(), "ikomia", "utils")
        cache_file_path2 = os.path.join(local_site, filename)
        return os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2)

    def make_local_plugins(self, force: bool = False):
        """
        Generate auto-completion data for local algorithms.

        Args:
            force (bool): bypass cache system
        """
        if not force:
            if self._check_local_sync() and self._has_local_cache():
                return

        cache_name = "autocomplete_local.cache"

        if not self.registry.is_all_loaded():
            self.registry.load_algorithms()

        try:
            folder = os.path.dirname(__file__)
            cache_file_path1 = os.path.join(folder, cache_name)
            f = open(cache_file_path1, "w+", encoding="utf-8")
        except Exception:
            try:
                folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
                cache_file_path2 = os.path.join(folder, cache_name)
                os.makedirs(folder, exist_ok=True)
                f = open(cache_file_path2, "w+", encoding="utf-8")
            except Exception:
                logger.warning("Ikomia auto-completion is disable")
                return

        names = self.registry.get_algorithms()

        # Write local names list declaration
        f.write("local_names = [\n")
        for name in names:
            f.write(f"    '{name}',\n")
        f.write("]\n\n")

        # Write instanciation functions
        for name in names:
            try:
                self._write_auto_complete(f, task_name=name, local=True)
            except Exception as e:
                # SKip failing plugin
                logger.debug(e)

        try:
            f.close()
            self._generate_python_file(folder)
            logger.info("Ikomia auto-completion updated for installed plugins.")
        except Exception as e:
            logger.info("Ikomia auto-completion cannot be generated.")
            logger.debug(e)

    def update_local_plugin(self, name: str):
        """
        Update completion data for the given algorithm.

        Args:
            name (str): name of the algorithm
        """
        task = self.registry.create_instance(name)
        if task is None:
            return

        info = self.registry.get_algorithm_info(name)
        if info.internal:
            return

        if not self._check_task_params(task):
            # TODO change only function definition instead of rebuild all?
            self.make_local_plugins(force=True)
            # TODO remove function definition instead of rebuild all?
            self.make_online_plugins(force=True)
        elif not self._check_local_sync():
            cache_name = "autocomplete_local.cache"

            try:
                folder = os.path.dirname(__file__)
                cache_file_path1 = os.path.join(folder, cache_name)
                f = open(cache_file_path1, "a+", encoding="utf-8")
            except Exception:
                try:
                    folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
                    cache_file_path2 = os.path.join(folder, cache_name)
                    f = open(cache_file_path2, "a+", encoding="utf-8")
                except Exception:
                    logger.warning("Ikomia auto-completion is disable: no update.")

            try:
                self._write_auto_complete(f, task=task, local=True)
                ik.local_names.append(task.name)
                # TODO remove function definition instead of rebuild all?
                self.make_online_plugins(force=True)
                f.close()
                self._generate_python_file(folder)
                logger.info("Ikomia auto-completion updated for installed plugin.")
            except Exception as e:
                logger.info("Ikomia auto-completion cannot be updated.")
                logger.debug(e)

    def make_online_plugins(self, force: bool = False):
        """
        Generate auto-completion data for Ikomia HUB algorithms.

        Args:
            force (bool): bypass caching
        """
        if not force and self._has_online_cache() and self._check_online_sync():
            return

        if not self.registry.is_all_loaded():
            self.registry.load_algorithms()

        cache_name = "autocomplete_online.cache"

        try:
            folder = os.path.dirname(__file__)
            cache_file_path1 = os.path.join(folder, cache_name)
            f = open(cache_file_path1, "w+", encoding="utf-8")
        except Exception:
            try:
                folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
                cache_file_path2 = os.path.join(folder, cache_name)
                os.makedirs(folder, exist_ok=True)
                f = open(cache_file_path2, "w+", encoding="utf-8")
            except Exception:
                logger.warning("Ikomia auto-completion is disable")

        try:
            online_algos = self.registry.get_public_hub_algorithms()
        except Exception as e:
            logger.debug("Auto-completion: skip generating online cache (see error message below).")
            logger.debug(e)
            return

        # Write online names list declaration
        f.write("online_names = [\n")
        for algo in online_algos:
            f.write(f"    '{algo['name']}',\n")
        f.write("]\n\n")

        # Write instanciation functions
        local_names = self.registry.get_algorithms()
        for algo in online_algos:
            if algo["name"] not in local_names:
                try:
                    self._write_auto_complete(f, task_name=algo["name"], local=False)
                except Exception as e:
                    # SKip failing plugin
                    logger.debug(e)

        try:
            f.close()
            self._generate_python_file(folder)
            logger.info("Ikomia auto-completion updated for Ikomia HUB algorithms.")
        except Exception as e:
            logger.info("Ikomia auto-completion cannot be generated.")
            logger.debug(e)

    def make(self, force: bool = False):
        """
        Generate full auto-completion data: local and Ikomia HUB algorithms.

        Args:
            force (bool): bypass caching.
        """
        self.make_local_plugins(force)
        self.make_online_plugins(force)


# -----------------------------------------
# ----- Global auto-completion object -----
# -----------------------------------------
ik_autocomplete = AutoComplete()
# -----------------------------------------
