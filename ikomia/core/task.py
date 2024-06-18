# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Module dedicated to high-level features around task management.

See also :py:class:`~ikomia.core.pycore.CWorkflowTask` for all available methods from task object instance.
"""
import logging
from ikomia.core import CWorkflowTaskParam, CWorkflowTask  # pylint: disable=E0611
from ikomia.dataprocess import CWorkflowTaskIO  # pylint: disable=E0611
from ikomia.dataprocess.registry import ik_registry

logger = logging.getLogger(__name__)


class TaskParam(CWorkflowTaskParam):
    """
    Base class to manage task parameters. Inherit :py:class:`~ikomia.core.pycore.CWorkflowTaskParam`.
    It includes a dict structure to store parameter values.

    :ivar cfg: parameters dict
    """
    def __init__(self):
        """
        Constructor. Initialize an empty dict structure.
        """
        CWorkflowTaskParam.__init__(self)
        self.cfg = {}

    def get_values(self) -> dict:
        """
        Return parameters for saving in Ikomia Studio.

        Returns:
            dict: parameters as key-value pairs
        """
        param_map = {}
        for key, value in self.cfg.items():
            param_map[key] = str(value)

        return param_map

    def set_values(self, params: dict):
        """
        Generic way to set parameters of CWorkflowTask-based object. Must be reimplemented.

        Args:
            params (dict): parameters as key-value pairs
        """
        raise NotImplementedError


def create(name: str = "", public_hub: bool = True, private_hub: bool = False) -> CWorkflowTask:
    """
    Create task instance (ie algorithm) from the given name.
    See :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry` for details.

    Args:
        name (str): unique algorithm name
        public_hub (bool): if True, try to install from public HUB if not installed locally
        private_hub (bool): if True, try to install from private HUB if not installed locally

    Returns:
        :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived: algorithm instance
    """
    if not name:
        return None

    return ik_registry.create_algorithm(name=name, public_hub=public_hub, private_hub=private_hub)


def get_output(task_obj: CWorkflowTask, types: list, index: int = -1) -> CWorkflowTaskIO:
    """
    Get specific output(s) of a task from the given types (:py:class:`~ikomia.core.pycore.IODataType`).

    Args:
        task_obj (:py:class:`~ikomia.core.pycore.CWorkflowTask` based object): task instance
        types (list of :py:class:`~ikomia.core.pycore.IODataType`): output data types
        index (int): zero-based index in the candidates output list. If -1, all candidates are returned.

    Returns:
        instance or list of :py:class:`~ikomia.dataprocess.pydataprocess.CWorkflowTaskIO` based object: outputs
    """
    if task_obj is None:
        raise RuntimeError("Cannot get outputs from None object.")

    outputs = []
    all_outputs = task_obj.get_outputs()

    for output in all_outputs:
        if output.data_type in types:
            outputs.append(output)

    if index == -1:
        return outputs

    if 0 <= index < len(outputs):
        return outputs[index]

    raise RuntimeError(f"No output at index {index}: only {len(outputs)} outputs available")


def conform_parameters(params: dict) -> dict:
    """
    Conform parameters dict structure to ensure that all values are string.

    Args:
        params (dict): parameters as key-value pairs

    Returns:
        dict: conformed parameters with string values
    """
    valid_params = {}
    for key in params:
        if not isinstance(key, str):
            raise TypeError("Parameter key must be a string.")

        value = params[key]
        if not isinstance(params[key], str):
            valid_params[key] = str(value)
        else:
            valid_params[key] = value

    return valid_params
