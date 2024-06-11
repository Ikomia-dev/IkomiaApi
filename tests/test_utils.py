import logging
import os
import argparse
import time
from ikomia import utils
from ikomia.utils.plugintools import get_plugin_dependencies, get_installed_modules
from ikomia.dataprocess.registry import ik_registry

logger = logging.getLogger(__name__)


def test_plugin_dependencies():
    logger.warning("===== Test::get plugin dependencies =====")
    directory = ik_registry.get_plugins_directory()
    ik_registry.install_algorithm(utils.ik.infer_torchvision_resnet().name)
    algo_dir = os.path.join(directory, "Python", utils.ik.infer_torchvision_resnet().name)
    modules = get_plugin_dependencies(algo_dir)
    logger.warning(modules)


def test_installed_modules():
    logger.warning("===== Test::get installed modules =====")
    mods = get_installed_modules()
    logger.warning(mods)


def test_cpp_bindings():
    # Plugin state enum
    state = utils.PluginState.VALID
    state = utils.PluginState.DEPRECATED
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
    logger.warning(f"API version: {utils.get_api_version()}")
    logger.warning(f"Compatibility check: {utils.get_compatibility_state('0.8.1', '0.10.0', utils.ApiLanguage.CPP)}")
    logger.warning(f"CPU architecture: {utils.get_cpu_arch()}")
    logger.warning(f"CPU architecture name: {utils.get_cpu_arch_name(utils.get_cpu_arch())}")
    logger.warning(f"CUDA version: {utils.get_cuda_version()}")
    logger.warning(f"App started: {utils.is_app_started()}")
    logger.warning(f"Model HUB url: {utils.get_model_hub_url()}")

    # Exception
    ex = utils.CException()
    logger.warning(ex.message())
    ex = utils.CException(0, "Exception error message", "test_cpp_bindings", __file__, 47)
    logger.warning(ex.message())

    # Memory info
    mem = utils.CMemoryInfo()
    logger.warning(f"Total memory: {mem.get_total_memory()}")
    logger.warning(f"Available memory: {mem.get_available_memory()}")
    logger.warning(f"Memory load: {mem.get_memory_load()}")

    # Timer
    timer = utils.CTimer()
    timer.start()
    time.sleep(1)
    timer.print_elapsed_time_ms("Step1")
    logger.warning(f"Elapsed time (ms, µs, ns): {timer.get_elapsed_ms()} {timer.get_elapsed_us()} {timer.get_elapsed_ns()}")
    time.sleep(1)
    timer.print_elapsed_time_ms("Step2")
    logger.warning(f"Elapsed time (ms, µs, ns): {timer.get_elapsed_ms()} {timer.get_elapsed_us()} {timer.get_elapsed_ns()}")
    timer.print_total_elapsed_time_ms("Step2")
    logger.warning(f"Total elapsed time (ms, µs, ns): {timer.get_total_elapsed_ms()} {timer.get_total_elapsed_us()} {timer.get_total_elapsed_ns()}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'dependencies' in running_tests:
        test_plugin_dependencies()

    if 'all' in running_tests or 'installed_mods' in running_tests:
        test_installed_modules()

    if "all" in running_tests or "cpp_bindings" in running_tests:
        test_cpp_bindings()
