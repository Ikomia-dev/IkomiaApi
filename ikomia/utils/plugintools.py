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
Internal use only
"""

import os
import modulefinder
import re
import subprocess
import sys
import types
import importlib
import shutil
import json
import logging


logger = logging.getLogger(__name__)


SYSMODULES = [
    "__main__", "__future__", "string", "re",
    "difflib", "textwrap", "unicodedata", "stringprep", "readline",
    "rlcompleter", "struct", "codecs", "datetime", "calendar", "collections",
    "heapq", "bisect", "array", "weakref", "types", "copy", "pprint",
    "reprlib", "enum", "numbers", "math", "cmath", "decimal", "fractions",
    "random", "statistics", "itertools", "functools", "operator", "pathlib",
    "fileinput", "stat", "filecmp", "tempfile", "glob", "fnmatch", "linecache",
    "shutil", "macpath", "pickle", "copyreg", "shelve", "marshal", "dbm",
    "sqlite3", "zlib", "gzip", "bz2", "lzma", "zipfile", "tarfile", "csv",
    "configparser", "netrc", "xdrlib", "plistlib", "hashlib", "hmac", "secrets",
    "os", "io", "time", "argparse", "getopt", "logging", "getpass", "curses",
    "platform", "errno", "ctypes", "threading", "multiprocessing", "concurrent",
    "sched", "queue", "_thread", "_dummy_thread", "dummy_threading",
    "asyncio", "socket", "ssl", "select", "selectors", "asyncore", "asynchat",
    "signal", "mmap", "email", "json", "mailcap", "mailbox", "mimetypes",
    "base64", "binhex", "binascii", "quopri", "uu", "html", "xml",
    "webbrowser", "cgi", "cgitb", "wsgiref", "urllib", "ftplib", "poplib",
    "imaplib", "nntplib", "smtplib", "smtpd", "telnetlib", "uuid", "socketserver",
    "xmlrpc", "ipadress", "audioop", "aifc", "sunau", "wave", "chunk", "colorsys",
    "imghdr", "sndhdr", "ossaudiodev", "gettext", "locale", "turtle", "cmd",
    "shlex", "tkinter", "typing", "pydoc", "doctest", "unittest", "2to3",
    "test", "bdb", "faulthandler", "pdb", "timeit", "trace", "tracemalloc",
    "distutils", "ensurepip", "venv", "zipapp", "sys", "sysconfig", "builtins",
    "warnings", "dataclasses", "contextlib", "abc", "atexit", "traceback",
    "gc", "inspect", "site", "code", "codeop", "zipimport", "pkgutil",
    "modulefinder", "runpy", "importlib", "parser", "ast", "symtable",
    "symbol", "token", "keyword", "tokenize", "tabnanny", "pyclbr",
    "py_compile", "compileall", "dis", "pickletools", "formatter", "msilib",
    "msvcrt", "winreg", "winsound", "posix", "pwd", "spwd", "grp", "crypt",
    "termios", "tty", "pty", "fcntl", "pipes", "resource", "nis", "syslog"
    "optparse", "imp"
]


class SingleFileModuleFinder(modulefinder.ModuleFinder):
    """
    Internal use only.
    """
    def import_hook(self, name: str, caller, *arg, **kwarg):
        if caller is not None and caller.__file__ == self.name:
            # Only call the parent at the top level.
            return modulefinder.ModuleFinder.import_hook(self, name, caller, *arg, **kwarg)

        return None

    def __call__(self, node):
        self.name = str(node)
        self.run_script(self.name)


def get_installed_modules() -> dict:
    """
    Return the list of installed package in the current Python environment.

    Returns:
        dict: list with package name and version
    """
    modules = {}
    result = subprocess.run([sys.executable, "-m", "pip", "list", "--format", "json"],
                            capture_output=True,
                            text=True)
    if result.stdout:
        modules = json.loads(result.stdout)
    else:
        logger.error(result.stderr)

    return modules


def get_plugin_dependencies(plugin_folder: str) -> list:
    """
    Return the list of Python dependencies for a given Ikomia plugin.

    Args:
        plugin_folder (str): path to the plugin folder

    Returns:
        list: list of Python package names
    """
    good_modules = []
    bad_modules = []
    file_paths = []
    module_names = []

    # Get all python files of the plugin (including sub-folders)
    for r, _, f in os.walk(plugin_folder):
        for file in f:
            if file.endswith(".py"):
                module_names.append(os.path.splitext(file)[0])
                file_paths.append(os.path.normpath(os.path.join(r, file)))

                # When a file __init__.py exists, consider folder as a module
                if file == "__init__.py":
                    module_name = os.path.basename(os.path.dirname(file_paths[-1]))
                    if module_name not in module_names:
                        module_names.append(module_name)

    # Retrieve dependencies from all python files into the plugin folder
    for file in file_paths:
        mf = SingleFileModuleFinder()
        mf(file)

        for mod in mf.modules.keys():
            # Keep only root modules (ex: keep matplotlib when we have import matplotlib.pyplot as plt)
            root_mod = mod.replace(mod, mod.split('.')[0])

            if root_mod not in SYSMODULES and root_mod not in module_names:
                good_modules.append(root_mod)

        for mod in mf.badmodules.keys():
            # Keep only root modules (ex: keep matplotlib when we have import matplotlib.pyplot as plt)
            root_mod = mod.replace(mod, mod.split('.')[0])

            if root_mod not in SYSMODULES and root_mod not in good_modules and root_mod not in module_names:
                bad_modules.append(root_mod)

    # Remove duplicates
    return list(set(good_modules)), list(set(bad_modules))


def install_requirements(directory: str):
    """
    Install Python dependencies for the given Ikomia Plugin.
    The function searches for requirements files and iterate over them in the lexicographic order.

    Args:
        directory (str): path to the plugin directory
    """
    req_files = []
    for root, _, files in os.walk(directory, topdown=True):
        for file in files:
            if re.search("[rR]equirements[0-9]*.txt", file):
                req_files.append(os.path.normpath(os.path.join(root, file)))
        break

    req_files.sort()

    for file in req_files:
        try:
            subprocess.run([sys.executable, "-m", "pip", "install", "-r", file], check=True)
        except Exception as e:
            logger.warning(e)


def install_package(name: str, version: str):
    """
    Install a specific Python package according to the given package name and version.

    Args:
        name (str): name of the package to install
        version (str): version of the package to install
    """
    if name:
        try:
            subprocess.run([sys.executable, "-m", "pip", "install", f'{name}=={version}'], check=True)
        except Exception as e:
            logger.warning(e)


def uninstall_package(name: str):
    """
    Uninstall a specific Python package.

    Args:
        name (str): name of the package to uninstall
    """
    if name:
        try:
            subprocess.run([sys.executable, "-m", "pip", "uninstall", "-y", name], check=True)
        except Exception as e:
            logger.warning(e)


def import_plugin_module(directory: str, name: str):
    """
    Import Ikomia plugin programmatically.

    Args:
        directory (str): path to the plugin directory
        name (str): name of the plugin
    """
    if is_module_imported(name):
        for _, _, files in os.walk(directory, topdown=True):
            for file in files:
                filename, file_extension = os.path.splitext(file)
                if file_extension == ".py":
                    unload_plugin_module(filename)

    return importlib.import_module(name)


def is_module_imported(name: str) -> bool:
    """
    Check whether a Python module is already imported.

    Args:
        name (str): module name

    Returns:
        bool: True if module is already imported, False otherwise
    """
    for _, val in globals().items():
        if isinstance(val, types.ModuleType) and name == val.__name__:
            return True

    return False


def unload_plugin_module(name: str):
    """
    Unload the given Python module.

    Args:
        name (str): module name to unload
    """
    modules_to_delete = [m for m in sys.modules.keys() if name == m]
    for m in modules_to_delete:
        del sys.modules[m]


def conform_plugin_directory(directory: str, plugin: dict) -> str:
    """
    Conform plugin directory name to fit the Ikomia naming convention.
    This function must be call after downloading the plugin package from
    Ikomia HUB to ensure name validity.

    Args:
        directory (str): path to the plugin directory
        plugin (dict): plugin information retrieved from Ikomia HUB
    """
    good_dir_name = directory
    base_name = os.path.basename(directory)

    if plugin["language"] == 1:
        # PYTHON
        for _, _, files in os.walk(directory, topdown=True):
            for file in files:
                match = re.search(r"([a-zA-Z0-9\-\._#@=]+)_process", file)
                if match is not None:
                    good_dir_name = directory.replace(base_name, match.group(1))
                    break
            break
    else:
        # C++
        good_name = base_name.replace(" ", "")
        good_name = good_name.replace(":", "-")
        good_dir_name = directory.replace(base_name, good_name)

    if good_dir_name != directory:
        if os.path.isdir(good_dir_name):
            shutil.rmtree(good_dir_name)

        os.rename(directory, good_dir_name)

    return good_dir_name


if __name__ == "__main__":
    folder = 'plugin_folder'
    goodModules, badModules = get_plugin_dependencies(folder)
    print('Dependencies:\n')
    print(goodModules)
    print('\n\nMissing:\n')
    print(badModules)
