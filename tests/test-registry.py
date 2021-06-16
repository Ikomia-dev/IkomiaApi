import logging
import ikomia
from ikomia.dataprocess import store
import random

logger = logging.getLogger(__name__)


def test_get_local_algorithms():
    try:
        registry = store.IkomiaRegistry()
        algos = registry.getAlgorithms()
        logger.info("Number of algorithms:" + str(len(algos)))
        logger.info(algos)
    except Exception as e:
        logger.error(e)


def test_local_instantiation():
    try:
        registry = store.IkomiaRegistry()
        algo_names = registry.getAlgorithms()
        algos = []

        for name in algo_names:
            algo = registry.createInstance(name)
            logger.info(algo.name)
            algos.append(algo)

        logger.info("Try running algorithm (error expected):" + algos[0].name)
        algos[0].run()

    except Exception as e:
        logger.error(e)


def test_get_online_algorithms():
    registry = store.IkomiaRegistry()
    algos = registry.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


def test_download_plugin():
    registry = store.IkomiaRegistry()
    logger.info("Downloading RAFTOpticalFlow...")
    registry._download_plugin("RAFTOpticalFlow")


def test_install_plugin():
    registry = store.IkomiaRegistry()
    logger.info("Installing RAFTOpticalFlow...")
    registry.install_plugin("RAFTOpticalFlow")


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    #test_get_local_algorithms()
    #test_local_instantiation()
    #test_get_online_algorithms()
    #test_download_plugin()
    test_install_plugin()
