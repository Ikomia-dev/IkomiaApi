"""
Module dedicated to workflow management.

It implements the Workflow class that offers high-level features
based on a C++ implementation. You will be able to create, modify, load and run workflows composed by built-in
Ikomia algorithms or any of those available in Ikomia HUB.
"""

import json
import logging
import os
from typing import Literal

from ikomia import utils
from ikomia.core.enums import WorkflowType
from ikomia.dataprocess import CHardwareConfig
from ikomia.dataprocess.registry import ik_registry
from ikomia.dataprocess.workflow.base import Workflow
from ikomia.dataprocess.workflow.llm import LlmWorkflow

logger = logging.getLogger(__name__)


def create(name: str = "untitled") -> Workflow:
    """
    Create new empty generic workflow.

    See also :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`,
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks` and
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.run`.

    Args:
        name (str): workflow name.

    Returns:
        Workflow: workflow instance
    """
    return Workflow(name=name, registry=ik_registry)


def create_llm(
    name: str = "untitled",
    backend: Literal["vllm", "sglang"] = "vllm",
    model_name: str = "",
) -> LlmWorkflow:
    """
    Create new empty LLM workflow.

    See also :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`,
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks` and
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.run`.

    Args:
        name (str): workflow name.
        backend (str): inference backend, can be vllm or sglang.
        model_name (str): llm model.

    Returns:
        LlmWorkflow: workflow instance
    """
    return LlmWorkflow(
        name=name, backend=backend, model_name=model_name, registry=ik_registry
    )


def load(path: str) -> Workflow:
    """
    Load Ikomia workflow from the given path.

    Args:
        path (str): path to workflow definition file (.json)

    Returns:
        :py:class:`~ikomia.dataprocess.workflow.Workflow`: loaded workflow.
    """
    with open(path, "r", encoding="utf-8") as f:
        json_wf = json.load(f)
        if "type" in json_wf and json_wf["type"] == WorkflowType.LLM:
            wf = LlmWorkflow(
                name="untitled", backend=json_wf["llm"]["backend"], registry=ik_registry
            )
        else:
            wf = Workflow(name="untitled", registry=ik_registry)

        wf.load(path)
        return wf


def install_requirements(path: str) -> bool:
    """
    Install Python requirements from all algorithms of the workflow stored at the given path.

    Algorithms must be installed locally before calling this function.

    Args:
        path (str): path to workflow definition file (.json)

    Returns:
        True if all installations succeeded else False
    """
    tasks = Workflow.get_required_tasks(path)
    available_tasks = ik_registry.get_algorithms()
    plugins_directory = ik_registry.get_plugins_directory()

    for t in tasks:
        if t not in available_tasks:
            plugin_dir = os.path.join(plugins_directory, "Python", t)
            if os.path.isdir(plugin_dir):
                utils.plugintools.install_requirements(plugin_dir)
            else:
                msg = f"Requirements installation failed: algorithm folder {t} cannot be found."
                logger.error(msg)
                return False
    return True


def get_min_hardware_config(path: str) -> CHardwareConfig:
    """
    Get minimum hardware configuration for the given workflow file.

    The configuration is determined from the minimum hardware configuration of workflow tasks.

    Args:
         path (str): path to workflow definition file (.json)

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CHardwareConfig`: hardware configuration
    """
    Workflow.prepare_runtime_env(path)
    min_hw_config = CHardwareConfig()
    task_names = Workflow.get_required_tasks(path)

    for name in task_names:
        info = ik_registry.get_algorithm_info(name)
        min_hw_config.min_cpu = max(min_hw_config.min_cpu, info.hardware_config.min_cpu)
        min_hw_config.min_ram = max(min_hw_config.min_ram, info.hardware_config.min_ram)
        min_hw_config.gpu_required = (
            min_hw_config.gpu_required or info.hardware_config.gpu_required
        )
        min_hw_config.min_vram = max(
            min_hw_config.min_vram, info.hardware_config.min_vram
        )

    return min_hw_config
