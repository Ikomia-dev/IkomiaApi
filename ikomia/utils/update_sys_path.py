# For Windows users who want to use the standalone API in "dev mode"
# The user environment variable IKOMIA_DEV = 1 must be set

import os
import sys

if os.environ["IKOMIA_DEV"] == "1" and sys.platform == "win32":
    try:
        dll_list = os.environ["PATH"]
        dlls = dll_list.split(";")
        for dll in dlls:
            if dll:
                os.add_dll_directory(dll)
    except:
        pass
