import logging
from ikomia import utils, dataprocess

logger = logging.getLogger(__name__)


def test_names():
    try:
        registry = dataprocess.CIkomiaRegistry()
        algos = registry.getAlgorithms()
        logger.info("Number of algorithms:" + str(len(algos)))
        logger.info(algos)
    except Exception as e:
        logger.error(e)


def test_instantiation():
    try:
        registry = dataprocess.CIkomiaRegistry()
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


if __name__ == "__main__":
    test_names()
    test_instantiation()
