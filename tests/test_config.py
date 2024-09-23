import os
import logging
from ikomia.core import config

logger = logging.getLogger(__name__)


def test_main_config_load():
    assert config.main_cfg is not None
    assert len(config.main_cfg) > 0


def test_main_config_save():
    config.main_cfg["registry"]["auto_completion"] = False
    config.save_main_config()
    config_path = os.path.join(os.path.dirname(os.path.abspath(config.__file__)), "config.yaml")
    cfg = config.load(config_path)
    assert cfg["registry"]["auto_completion"] == False
