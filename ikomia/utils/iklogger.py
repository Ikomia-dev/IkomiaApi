"""
Main Ikomia logger.
"""
import sys
import logging
from ikomia.core import config


class _ExcludeStderrFilter(logging.Filter):
    def filter(self, record):
        return record.levelno < logging.WARNING


# ----------------------------------
# ----- Logging initialization -----
# ----------------------------------
logger = logging.getLogger("ikomia")
logger.handlers = []
logger.setLevel(logging.DEBUG)

# log to stdout and stderr
formatter = logging.Formatter("%(message)s")
info = logging.StreamHandler(sys.stdout)
info.setLevel(logging.INFO)
info.setFormatter(formatter)
info.addFilter(_ExcludeStderrFilter())
logger.addHandler(info)

err = logging.StreamHandler(sys.stderr)
err.setLevel(logging.ERROR)
err.setFormatter(formatter)
logger.addHandler(err)

# Log to file
log_path = config.main_cfg["root_folder"] + "/log.txt"
file_formatter = logging.Formatter("%(asctime)s:%(levelname)s:%(message)s", "%Y-%m-%d %H:%M:%S")
file_handler = logging.FileHandler(log_path, 'a')
file_handler.setLevel(logging.DEBUG)
file_handler.setFormatter(file_formatter)
logger.addHandler(file_handler)
logger.propagate = False
