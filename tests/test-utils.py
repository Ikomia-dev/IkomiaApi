import logging
import argparse
import time
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


def test_cpp_bindings():
    # Plugin state enum
    state = utils.PluginState.VALID
    state = utils.PluginState.DEPRECATED
    state = utils.PluginState.UPDATED
    state = utils.PluginState.INVALID

    # Operating System enum
    os = utils.OSType.ALL
    os = utils.OSType.LINUX
    os = utils.OSType.WIN
    os = utils.OSType.OSX

    # API language enum
    language = utils.ApiLanguage.CPP
    language = utils.ApiLanguage.PYTHON

    # global functions
    logger.info(f"API version: {utils.get_api_version()}")
    logger.info(f"Compatibility check: {utils.get_compatibility_state('0.8.1', utils.ApiLanguage.CPP)}")
    logger.info(f"Check compile architecture: {utils.check_architecture_keywords('python310,cuda11')}")
    logger.info(f"App started: {utils.is_app_started()}")
    logger.info(f"Model HUB url: {utils.get_model_hub_url()}")

    # Exception
    ex = utils.CException()
    logger.info(ex.message())
    ex = utils.CException(0, "Exception error message", "test_cpp_bindings", __file__, 47)
    logger.info(ex.message())

    # Memory info
    mem = utils.CMemoryInfo()
    logger.info(f"Total memory: {mem.get_total_memory()}")
    logger.info(f"Available memory: {mem.get_available_memory()}")
    logger.info(f"Memory load: {mem.get_memory_load()}")

    # Timer
    timer = utils.CTimer()
    timer.start()
    time.sleep(1)
    timer.print_elapsed_time_ms("Step1")
    logger.info(f"Elapsed time (ms, µs, ns): {timer.get_elapsed_ms()} {timer.get_elapsed_us()} {timer.get_elapsed_ns()}")
    time.sleep(1)
    timer.print_elapsed_time_ms("Step2")
    logger.info(f"Elapsed time (ms, µs, ns): {timer.get_elapsed_ms()} {timer.get_elapsed_us()} {timer.get_elapsed_ns()}")
    timer.print_total_elapsed_time_ms("Step2")
    logger.info(f"Total elapsed time (ms, µs, ns): {timer.get_total_elapsed_ms()} {timer.get_total_elapsed_us()} {timer.get_total_elapsed_ns()}")


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

    if "all" in running_tests or "cpp_bindings" in running_tests:
        test_cpp_bindings()
