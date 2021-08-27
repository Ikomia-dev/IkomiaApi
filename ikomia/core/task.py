"""
Module dedicated to high-level features around task management.
"""
from ikomia import core


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


def set_parameters(task, params: dict):
    """
    Set parameters of the given task from a dict structure.

    Args:
        task (int): unique task identifier
        params (dict): pair of key, value for each parameter. The key will be treated as str.
    """
    if task is None:
        return

    param_map = task.getParamValues()
    for key in params:
        param_map[str(key)] = str(params[key])

    param = task.getParam()
    param.setParamMap(param_map)


def _get_outputs(task, types, index=1):
    if task is None:
        raise RuntimeError("Cannot get outputs from None task.")

    outputs = []
    all_outputs = task.getOutputs()

    for output in all_outputs:
        if output.dataType in types:
            outputs.append(output)

    if 0 <= index < len(outputs):
        return outputs[index]
    elif len(outputs) == 1:
        return outputs[0]
    else:
        return outputs


def get_image_output(task, index=-1):
    # data_types = [core.IODataType.IMAGE, core.IODataType.IMAGE_BINARY, core.IODataType.IMAGE_LABEL]
    data_types = [0, 1, 2]
    return _get_outputs(task, data_types, index)


def get_graphics_output(task, index=-1):
    data_types = [core.IODataType.OUTPUT_GRAPHICS]
    return _get_outputs(task, data_types, index)


def get_numeric_output(task, index=-1):
    data_types = [core.IODataType.NUMERIC_VALUES]
    return _get_outputs(task, data_types, index)


def get_blob_measure_output(task, index=-1):
    data_types = [core.IODataType.BLOB_VALUES]
    return _get_outputs(task, data_types, index)


def get_dataset_output(task, index=-1):
    data_types = [core.IODataType.DNN_DATASET]
    return _get_outputs(task, data_types, index)


def get_array_output(task, index=-1):
    data_types = [core.IODataType.ARRAY]
    return _get_outputs(task, data_types, index)


def get_path_output(task, index=-1):
    data_types = [core.IODataType.FOLDER_PATH, core.IODataType.FILE_PATH]
    return _get_outputs(task, data_types, index)
