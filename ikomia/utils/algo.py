import logging
from typing import Union
from ikomia.utils import OSType
from ikomia.core import AlgoType, CWorkflowTask
from ikomia.dataprocess.workflow import Workflow
from ikomia.dataprocess.registry import ik_registry


logger = logging.getLogger(__name__)


def _check_algo_compatibility(algo_name: str) -> bool:
    info = ik_registry.get_algorithm_info(algo_name)

    # Check algorithm OS, Windows only algorithm not supported
    if info.os == OSType.WIN:
        logger.error("Windows-only algorithms are not supported for auto demo workflow")
        return False

    # Check algorithm type
    if info.algo_type == AlgoType.TRAIN or info.algo_type == AlgoType.DATASET:
        logger.error("Algorithms type %s is not supported for auto demo workflow", str(info.algo_type))
        return False

    return True


def create_demo_workflow(algo_name: str, depends_on: Union[list, None] = None) -> Workflow:
    """
    Automatically generate demo workflow for the given algorithm.
    """
    algo = ik_registry.create_algorithm(algo_name)
    if not _check_algo_compatibility(algo_name):
        raise RuntimeError(f"Algorithm {algo_name} is not supported for automatic demo workflow")

    wf = Workflow(name=algo_name)

    # Sequential deps only
    if depends_on is not None:
        for task_name in depends_on:
            task = ik_registry.create_algorithm(task_name)
            if not _check_algo_compatibility(task):
                raise RuntimeError(f"Algorithm {task_name} is not supported for automatic demo workflow")

            wf.add_task(task=task, auto_connect=True)

    # Add main algorithm
    wf.add_task(task=algo, auto_connect=True)
    return wf
