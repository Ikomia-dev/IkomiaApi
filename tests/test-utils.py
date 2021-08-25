import logging
import argparse
from ikomia import utils

logger = logging.getLogger(__name__)


def test_plugin_dependencies(plugin_folder):
    logger.info("===== Test::get plugin dependencies =====")
    modules = utils.get_plugin_dependencies(plugin_folder)
    logger.info(modules)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--plugin_folder", type=str, default="/home/ludo/Ikomia/Plugins/Python/ResNet",
                        help="Plugin folder to checl")
    opt = parser.parse_args()
    test_plugin_dependencies(opt.plugin_folder)