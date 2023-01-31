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

import os
import re
import site
import keyword
import logging
import shutil
import importlib
import sys
import ikomia

try:
    from ikomia.utils import ik
    _ik_auto_complete = True
except:
    _ik_auto_complete = False


logger = logging.getLogger(__name__)


def _write_auto_complete_str(f, name, skip_params=False):
    forbid_char = "\ |\-|\[|\]"

    if skip_params:
        # create global variable declaration
        var_name = re.sub(forbid_char, "", name)
        declaration = var_name + " = " + "\"" + name + "\"\n\n"
        f.write(declaration)
    else:
        # create class inherited from str
        algo = ikomia.ik_registry.create_algorithm(name)
        if algo is None:
            return

        class_name = re.sub(forbid_char, "", name)
        declaration = "class " + class_name + "(str):\n"
        f.write(declaration)
        f.write("    def __new__(cls):\n")
        ctor_line = "        obj = str.__new__(cls, " + "\"" + name + "\")\n"
        f.write(ctor_line)

        parameters = algo.get_parameters()
        for param in parameters:
            if keyword.iskeyword(param):
                param += "_"

            var_name = re.sub(forbid_char, "", param)
            declaration = "        obj." + var_name + " = " + "\"" + param + "\"\n"
            f.write(declaration)

        f.write("        return obj\n\n")
        create_line = name + " = " + class_name + "()\n\n"
        f.write(create_line)


def _write_auto_complete(f, task_name="", task=None, skip_params=False):
    if not task_name and task is None:
        raise RuntimeError("Auto-completion: parameters must include either a valid name or task instance.")

    forbid_char = "\ |\-|\[|\]"

    if task is not None:
        task_name = task.name

    # create global variable declaration
    var_name = re.sub(forbid_char, "", task_name)
    declaration = var_name + " = " + "\"" + task_name + "\"\n"
    f.write(declaration)

    if not skip_params:
        if task is None:
            # create class with attributes
            task = ikomia.ik_registry.create_algorithm(task_name)
            if task is None:
                raise RuntimeError(f"Auto-completion: unable to create algorithm {task_name}.")

        parameters = task.get_parameters()
        if len(parameters) == 0:
            f.write("\n")
            return

        class_name = re.sub(forbid_char, "", task_name)
        declaration = "class " + class_name + "_param:\n"
        f.write(declaration)

        for param in parameters:
            if keyword.iskeyword(param):
                param_key += "_"

            var_name = re.sub(forbid_char, "", param)
            declaration = "    " + var_name + " = " + "\"" + param + "\"\n"
            f.write(declaration)

        f.write("\n")


def _generate_python_file(folder):
    ik_file_path = os.path.join(folder, "ik.py")
    with open(ik_file_path, 'w+') as f:
        cache_file_path = os.path.join(folder, "autocomplete_local.cache")
        if os.path.isfile(cache_file_path):
            with open(cache_file_path, "r") as f_cache:
                shutil.copyfileobj(f_cache, f)

        cache_file_path = os.path.join(folder, "autocomplete_online.cache")
        if os.path.isfile(cache_file_path):
            with open(cache_file_path, "r") as f_cache:
                shutil.copyfileobj(f_cache, f)

    # reload module
    if "ikomia.utils.ik" in sys.modules:
        importlib.reload(ikomia.utils.ik)


def _check_sync():
    if not _ik_auto_complete:
        return False

    ik_names = dir(ik)
    names = ikomia.ik_registry.get_algorithms()

    for name in names:
        if name not in ik_names:
            return False

    return True


def _has_local_cache():
    filename = "autocomplete_local.cache"
    cache_file_path1 = os.path.join(os.path.dirname(__file__), filename)
    local_site = os.path.join(site.getusersitepackages(), "ikomia", "utils")
    cache_file_path2 = os.path.join(local_site, filename)
    return os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2)


def _has_online_cache():
    filename = "autocomplete_online.cache"
    cache_file_path1 = os.path.join(os.path.dirname(__file__), filename)
    local_site = os.path.join(site.getusersitepackages(), "ikomia", "utils")
    cache_file_path2 = os.path.join(local_site, filename)
    return os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2)


def make_local_plugins(force=False):
    if not force:
        if _check_sync() and _has_local_cache():
            return

    cache_name = "autocomplete_local.cache"

    if not ikomia.ik_registry.is_all_loaded():
        ikomia.ik_registry.load_algorithms()

    try:
        folder = os.path.dirname(__file__)
        cache_file_path1 = os.path.join(folder, cache_name)
        f = open(cache_file_path1, "w+")
    except Exception:
        try:
            folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
            cache_file_path2 = os.path.join(folder, cache_name)
            os.makedirs(folder, exist_ok=True)
            f = open(cache_file_path2, "w+")
        except Exception:
            logger.warning("Ikomia auto-completion is disable")
            return

    names = ikomia.ik_registry.get_algorithms()
    for name in names:
        try:
            _write_auto_complete(f, task_name=name, skip_params=False)
        except Exception as e:
            # SKip failing plugin
            logger.debug(e)

    try:
        f.close()
        _generate_python_file(folder)
        logger.info("Ikomia auto-completion updated for installed plugins.")
    except Exception as e:
        logger.info("Ikomia auto-completion cannot be generated.")
        logger.debug(e)


def update_local_plugin(task):
    cache_name = "autocomplete_local.cache"

    try:
        folder = os.path.dirname(__file__)
        cache_file_path1 = os.path.join(folder, cache_name)
        f = open(cache_file_path1, "a+")
    except Exception:
        try:
            folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
            cache_file_path2 = os.path.join(local_site, cache_name)
            f = open(cache_file_path2, "a+")
        except Exception:
            logger.warning("Ikomia auto-completion is disable: no update.")

    try:
        _write_auto_complete(f, task=task, skip_params=False)
        f.close()
        _generate_python_file(folder)
        logger.info("Ikomia auto-completion updated for installed plugin.")
    except Exception as e:
        logger.info("Ikomia auto-completion cannot be updated.")
        logger.debug(e)


def make_online_plugins(force=False):
    if not force and _has_online_cache():
        return

    cache_name = "autocomplete_online.cache"

    try:
        folder = os.path.dirname(__file__)
        cache_file_path1 = os.path.join(folder, cache_name)
        f = open(cache_file_path1, "w+")
    except Exception:
        try:
            folder = os.path.join(site.getusersitepackages(), "ikomia", "utils")
            cache_file_path2 = os.path.join(folder, cache_name)
            os.makedirs(folder, exist_ok=True)
            f = open(cache_file_path2, "w+")
        except Exception:
            logger.warning("Ikomia auto-completion is disable")

    online_algos = ikomia.ik_registry.get_online_algorithms()
    if online_algos is None:
        return

    local_names = ikomia.ik_registry.get_algorithms()
    for algo in online_algos:
        if algo["name"] not in local_names:
            try:
                _write_auto_complete(f, task_name=algo["name"], skip_params=True)
            except Exception as e:
                # SKip failing plugin
                logger.debug(e)

    try:
        f.close()
        _generate_python_file(folder)
        logger.info("Ikomia auto-completion updated for Ikomia HUB algorithms.")
    except Exception as e:
        logger.info("Ikomia auto-completion cannot be generated.")
        logger.debug(e)

def make(force=False):
    make_local_plugins(force)
    make_online_plugins(force)