from ikomia import core


class TaskParam(core.CWorkflowTaskParam):
    """
    Base class to manage Deep Learning training parameters.
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


def set_parameter(task, params: dict):
    if task is None:
        return

    param_map = core.ParamMap()

    for key in params:
        param_map[str(key)] = str(params[key])

    param = task.getParam()
    param.setParamMap(param_map)
