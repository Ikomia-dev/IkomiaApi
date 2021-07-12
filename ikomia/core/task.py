from ikomia import core


def set_parameter(task, params: dict):
    if task is None:
        return

    param_map = core.ParamMap()

    for key in params:
        param_map[str(key)] = str(params[key])

    param = task.getParam()
    param.setParamMap(param_map)
