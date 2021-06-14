import logging
from ikomia.dataprocess import store
from ikomia.core import auth

logger = logging.getLogger(__name__)


def test_get_local_algorithms():
    try:
        registry = store.IkomiaRegistry()
        algos = registry.getAlgorithms()
        logger.info("Number of algorithms:" + str(len(algos)))
        logger.info(algos)
    except Exception as e:
        logger.error(e)


def test_get_online_algorithms():
    registry = store.IkomiaRegistry()
    algos = registry.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


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


if __name__ == "__main__":
    auth.authenticate("Ludo", "ludo?imageez")
    test_get_local_algorithms()
    test_get_online_algorithms()
    test_local_instantiation()