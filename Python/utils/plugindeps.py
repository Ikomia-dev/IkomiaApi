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

"""
Internal use only
"""

import os
import modulefinder

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

    def import_hook(self, name, caller, *arg, **kwarg):
        if caller is not None and caller.__file__ == self.name:
            # Only call the parent at the top level.
            return modulefinder.ModuleFinder.import_hook(self, name, caller, *arg, **kwarg)

    def __call__(self, node):
        self.name = str(node)
        self.run_script(self.name)


def getPluginDependencies(pluginFolder):
    good_modules = []
    bad_modules = []
    file_paths = []
    module_names = []

    # Get all python files of the plugin (including sub-folders)
    for r, d, f in os.walk(pluginFolder):
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


if __name__ == "__main__":
    folder = '/home/yom/Ikomia/Plugins/Python/Yolact'
    goodModules, badModules = getPluginDependencies(folder)
    print('Dependencies:\n')
    print(goodModules)
    print('\n\nMissing:\n')
    print(badModules)

