# Copyright (C) 2021 Ikomia SAS
# Contact: https://www.ikomia.com
#
# This file is part of the Ikomia API libraries.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import os
import re
import site
import keyword
import logging
import shutil
import ikomia

logger = logging.getLogger()


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

        parameters = algo.getParamValues()
        for param in parameters:
            param_key = param.key()
            if keyword.iskeyword(param_key):
                param_key += "_"

            var_name = re.sub(forbid_char, "", param_key)
            declaration = "        obj." + var_name + " = " + "\"" + param_key + "\"\n"
            f.write(declaration)

        f.write("        return obj\n\n")
        create_line = name + " = " + class_name + "()\n\n"
        f.write(create_line)


def _write_auto_complete(f, name, skip_params=False):
    forbid_char = "\ |\-|\[|\]"

    # create global variable declaration
    var_name = re.sub(forbid_char, "", name)
    declaration = var_name + " = " + "\"" + name + "\"\n"
    f.write(declaration)

    if not skip_params:
        # create class with attributes
        algo = ikomia.ik_registry.create_algorithm(name)
        if algo is None:
            return

        parameters = algo.getParamValues()
        if len(parameters) == 0:
            f.write("\n")
            return

        class_name = re.sub(forbid_char, "", name)
        declaration = "class " + class_name + "_param:\n"
        f.write(declaration)

        for param in parameters:
            param_key = param.key()
            if keyword.iskeyword(param_key):
                param_key += "_"

            var_name = re.sub(forbid_char, "", param_key)
            declaration = "    " + var_name + " = " + "\"" + param_key + "\"\n"
            f.write(declaration)

        f.write("\n")


def _generate_python_file(folder):
    ik_file_path = folder + "ik.py"
    with open(ik_file_path, 'w+') as f:
        cache_file_path = folder + "autocomplete_local.cache"
        if os.path.isfile(cache_file_path):
            with open(cache_file_path, "r") as f_cache:
                shutil.copyfileobj(f_cache, f)

        cache_file_path = folder + "autocomplete_online.cache"
        if os.path.isfile(cache_file_path):
            with open(cache_file_path, "r") as f_cache:
                shutil.copyfileobj(f_cache, f)


def make_local_plugins(force=False):
    current_folder = os.path.dirname(__file__) + os.sep
    cache_file_path1 = current_folder + "autocomplete_local.cache"
    local_site = site.getusersitepackages() + os.sep + "ikomia" + os.sep + "utils" + os.sep
    cache_file_path2 = local_site + "autocomplete_local.cache"

    if not force:
        if os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2):
            return

    try:
        f = open(cache_file_path1, "w+")
        folder = current_folder
    except Exception:
        try:
            os.makedirs(local_site, exist_ok=True)
            f = open(cache_file_path2, "w+")
            folder = local_site
        except Exception:
            logger.warning("Ikomia auto-completion is disable")

    names = ikomia.ik_registry.getAlgorithms()
    for name in names:
        _write_auto_complete(f, name, skip_params=False)

    f.close()

    try:
        _generate_python_file(folder)
        logger.info("Ikomia auto-completion updated for installed plugins.")
    except Exception:
        logger.info("Ikomia auto-completion cannot be generated.")


def make_online_plugins(force=False):
    current_folder = os.path.dirname(__file__) + os.sep
    cache_file_path1 = current_folder + "autocomplete_online.cache"
    local_site = site.getusersitepackages() + os.sep + "ikomia" + os.sep + "utils" + os.sep
    cache_file_path2 = local_site + "autocomplete_online.cache"

    if not force:
        if os.path.isfile(cache_file_path1) or os.path.isfile(cache_file_path2):
            return

    try:
        f = open(cache_file_path1, "w+")
        folder = current_folder
    except Exception:
        try:
            os.makedirs(local_site, exist_ok=True)
            f = open(cache_file_path2, "w+")
            folder = local_site
        except Exception:
            logger.warning("Ikomia auto-completion is disable")

    online_algos = ikomia.ik_registry.get_online_algorithms()
    if online_algos is None:
        return

    local_names = ikomia.ik_registry.getAlgorithms()
    for algo in online_algos:
        if algo["name"] not in local_names:
            _write_auto_complete(f, algo["name"], skip_params=True)

    f.close()
    _generate_python_file(folder)
    logger.info("Ikomia auto-completion updated for marketplace plugins.")
