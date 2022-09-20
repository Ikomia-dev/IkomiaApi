import logging
import argparse
from ikomia import utils

logger = logging.getLogger(__name__)


def test_plugin_dependencies(plugin_folder):
    logger.info("===== Test::get plugin dependencies =====")
    modules = utils.get_plugin_dependencies(plugin_folder)
    logger.info(modules)


def test_installed_modules():
    logger.info("===== Test::get installed modules =====")
    mods = utils.get_installed_modules()
    logger.info(mods)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--plugin_folder", type=str, default="/home/ludo/Ikomia/Plugins/Python/infer_torchvision_resnet",
                        help="Plugin folder to checl")
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'dependencies' in running_tests:
        test_plugin_dependencies(opt.plugin_folder)

    if 'all' in running_tests or 'installed_mods' in running_tests:
        test_installed_modules()