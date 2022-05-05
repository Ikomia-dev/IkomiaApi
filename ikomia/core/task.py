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
import ikomia
from ikomia import core, dataprocess


class TaskParam(core.CWorkflowTaskParam):
    """
    Base class to manage task parameters.
    It includes a dict structure to store parameter values.

    :ivar cfg: parameters dict
    """
    def __init__(self):
        """
        Constructor. Initialize an empty dict structure.
        """
        core.CWorkflowTaskParam.__init__(self)
        self.cfg = {}

    def getParamMap(self):
        """
        Return parameters for saving in Ikomia Studio.
        """
        param_map = core.ParamMap()
        for key, value in self.cfg.items():
            param_map[key] = str(value)

        return param_map

    def setParamMap(self, param_map):
        """
        Generic way to set parameters of CWorkflowTask-based object. Must be reimplemented.
        """
        raise NotImplementedError


def create(name=""):
    """
    Create task instance (ie algorithm) from the given name.
    See :py:class:`~ikomia.dataprocess.registry.IkomiaRegistry` for details.

    Args:
        name (str): unique algorithm name

    Returns:
        :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived: algorithm instance
    """
    if not name:
        return None

    return ikomia.ik_registry.create_algorithm(name)


def set_parameters(task, params: dict):
    """
    Set parameters of the given task from a dict structure.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        params (dict): pair of key, value for each parameter. The key will be treated as str.
    """
    if task is None:
        return

    param_map = task.getParamValues()
    for key in params:
        param_map[str(key)] = str(params[key])

    param = task.getParam()
    param.setParamMap(param_map)


def get_parameters(task):
    """
    Get parameters as a dict of key-value pairs for the given task.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.

    Returns:
        parameters (dict): pairs of key, value for each parameter.
    """
    if task is None:
        raise RuntimeError("Parameter can't be retrieved from None object")

    params = {}
    param_map = task.getParamValues()
    for param in param_map:
        params[param.key()] = param_map[param.key()]

    return params


def _get_outputs(task, types, target_class=None, index=-1):
    if task is None:
        raise RuntimeError("Cannot get outputs from None task.")

    outputs = []
    all_outputs = task.getOutputs()

    for output in all_outputs:
        if output.dataType in types:
            if target_class is None:
                outputs.append(output)
            elif type(output) == target_class:
                outputs.append(output)

    if 0 <= index < len(outputs):
        return outputs[index]
    elif len(outputs) == 1:
        return outputs[0]
    else:
        return outputs


def get_image_output(task, index=-1):
    """
    Get IMAGE output(s) of the given task. A task can have multiple IMAGE outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CImageIO`: output or list of outputs.
    """
    data_types = [core.IODataType.IMAGE, core.IODataType.IMAGE_BINARY, core.IODataType.IMAGE_LABEL]
    return _get_outputs(task, data_types, index=index)


def get_image_with_graphics(task, image_index=0, graphics_index=0):
    """
    Get image (numpy array) of a specific IMAGE output with graphics items from a specific GRAPHICS output burnt
    into it. Index of IMAGE and GRAPHICS outputs must be given.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        image_index (int): zero-based index of the IMAGE output.
        graphics_index (int): zero-based index of the GRAPHICS output.

    Returns:
        Numpy array: result image.
    """
    if image_index < 0 or graphics_index < 0:
        raise RuntimeError("Output index must be equal or greater than 0.")

    img_data_types = [core.IODataType.IMAGE, core.IODataType.IMAGE_BINARY, core.IODataType.IMAGE_LABEL]
    img_output = _get_outputs(task, img_data_types, index=image_index)
    graphics_output = _get_outputs(task, [core.IODataType.OUTPUT_GRAPHICS], index=graphics_index)
    return img_output.getImageWithGraphics(graphics_output)


def get_graphics_output(task, index=-1):
    """
    Get GRAPHICS output(s) of the given task. A task can have multiple GRAPHICS outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput`: output or list of outputs.
    """
    data_types = [core.IODataType.OUTPUT_GRAPHICS]
    return _get_outputs(task, data_types, index=index)


def get_numeric_output(task, index=-1):
    """
    Get NUMERIC output(s) of the given task. A task can have multiple NUMERIC outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CNumericIO`: output or list of outputs.
    """
    data_types = [core.IODataType.NUMERIC_VALUES]
    return _get_outputs(task, data_types, target_class=dataprocess.CNumericIO, index=index)


def get_data_string_output(task, index=-1):
    """
    Get data string output(s) of the given task. A task can have multiple NUMERIC outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CDataStringIO`: output or list of outputs.
    """
    data_types = [core.IODataType.NUMERIC_VALUES]
    return _get_outputs(task, data_types, target_class=dataprocess.CDataStringIO, index=index)


def get_blob_measure_output(task, index=-1):
    """
    Get BLOB MEASURE output(s) of the given task. A task can have multiple BLOB MEASURE outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CBlobMeasureIO`: output or list of outputs.
    """
    data_types = [core.IODataType.BLOB_VALUES]
    return _get_outputs(task, data_types, index=index)


def get_dataset_output(task, index=-1):
    """
    Get DATASET output(s) of the given task. A task can have multiple DATASET outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CDatasetIO`: output or list of outputs.
    """
    data_types = [core.IODataType.DNN_DATASET]
    return _get_outputs(task, data_types, index=index)


def get_array_output(task, index=-1):
    """
    Get ARRAY output(s) of the given task. A task can have multiple ARRAY outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CArrayIO`: output or list of outputs.
    """
    data_types = [core.IODataType.ARRAY]
    return _get_outputs(task, data_types, index=index)


def get_path_output(task, index=-1):
    """
    Get ARRAY output(s) of the given task. A task can have multiple ARRAY outputs so the index
    argument can be set to specify the wanted output.

    Args:
        task (:py:class:`~ikomia.core.pycore.CWorkflowTask` or derived): object instance.
        index (int): zero-based index of the output in case of multiple matches. With default -1, all outputs are returned.

    Returns:
        :py:class:`~ikomia.dataprocess.pydataprocess.CPathIO`: output or list of outputs.
    """
    data_types = [core.IODataType.FOLDER_PATH, core.IODataType.FILE_PATH]
    return _get_outputs(task, data_types, index=index)
