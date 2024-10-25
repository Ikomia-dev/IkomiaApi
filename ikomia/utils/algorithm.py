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

    Args:
        algo_name (str): HUB algorithm name
        depends_on (list): list of sequential algorithms on which the given algorithm should be connected

    Returns:
        auto-generated workflow (Workflow)
    """
    algo = ik_registry.create_algorithm(algo_name)
    if not _check_algo_compatibility(algo_name):
        raise RuntimeError(f"Algorithm {algo_name} is not supported for automatic demo workflow")

    wf = Workflow(name=algo_name)
    wf.description = (
        ":star2: Auto-generated workflow for HUB algorithm deployment with Ikomia Scale.\n\n"
        "## How to deploy :rocket:\n"
        "1. Pick your provider\n"
        "2. Choose the compute infrastructure: serverless, CPU or GPU dedicated instances\n"
        "3. Select your region\n"
        "4. Choose the compute size\n"
        "5. Click 'Add deployment'\n\n"
        "Then, patiently await its execution and effortlessly test your algorithm with a single click!"
    )
    wf.keywords = "HUB,algorithm,deployment"

    # Sequential deps only
    if depends_on is not None:
        for task_name in depends_on:
            task = ik_registry.create_algorithm(task_name)
            if not _check_algo_compatibility(task):
                raise RuntimeError(f"Algorithm {task_name} is not supported for automatic demo workflow")

            wf.add_task(task=task, auto_connect=True)

    # Add main algorithm
    auto_connect = algo.get_input_count() > 0
    wf.add_task(task=algo, auto_connect=auto_connect)
    return wf
