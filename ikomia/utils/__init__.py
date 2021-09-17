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

import ikomia
from ikomia.utils.pyutils import *
from ikomia.utils.plugintools import *
from ikomia.utils.data import *
import sys
import logging
import re
import site

logger = logging.getLogger()


def init_logging(rank=-1):
    if rank in [-1, 0]:
        logger.handlers = []
        logger.setLevel(logging.INFO)

        # log to stdout and stderr
        formatter = logging.Formatter("%(message)s")
        info = logging.StreamHandler(sys.stdout)
        info.setLevel(logging.INFO)
        info.setFormatter(formatter)
        logger.addHandler(info)

        err = logging.StreamHandler(sys.stderr)
        err.setLevel(logging.ERROR)
        err.setFormatter(formatter)
        logger.addHandler(err)

        # log to file
        log_path = getIkomiaFolder() + "/log.txt"
        file_formatter = logging.Formatter("%(asctime)s;%(levelname)s;%(message)s", "%Y-%m-%d %H:%M:%S")
        file_handler = logging.FileHandler(log_path, 'w')
        file_handler.setLevel(logging.DEBUG)
        file_handler.setFormatter(file_formatter)
        logger.addHandler(file_handler)
    else:
        logging.basicConfig(format="%(message)s", level=logging.WARN)


def is_colab():
    # Is environment a Google Colab instance?
    try:
        import google.colab
        return True
    except Exception as e:
        return False


def make_auto_complete():
    current_folder = os.path.dirname(__file__)
    names_file_path = current_folder + os.sep + "ik.py"

    try:
        f = open(names_file_path, "w+")
    except Exception:
        local_site = site.getusersitepackages() + os.sep + "ikomia" + os.sep + "utils" + os.sep
        os.makedirs(local_site, exist_ok=True)
        names_file_path = local_site + "iknames.py"

        try:
            f = open(names_file_path, "w+")
        except Exception:
            logger.warning("Ikomia auto-completion is disable")

    local_names = ikomia.ik_registry.getAlgorithms()
    forbid_char = "\ |\-|\[|\]"

    for name in local_names:
        variable_name = re.sub(forbid_char, "", name)
        declaration = variable_name + " = " + "\"" + name + "\"\n\n"
        f.write(declaration)

    online_algos = ikomia.ik_registry.get_online_algorithms()
    if online_algos is None:
        return

    for algo in online_algos:
        if algo["name"] not in local_names:
            variable_name = re.sub(forbid_char, "", algo["name"])
            declaration = variable_name + " = " + "\"" + algo["name"] + "\"\n\n"
            f.write(declaration)
