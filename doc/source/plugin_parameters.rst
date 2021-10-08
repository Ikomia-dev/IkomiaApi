Parameters management
=====================

Core concept
------------

Algorithms often comes with parameters to adjust produced results. The Ikomia platform handles parameters in standalone class that will be shared across the process implementation and the UI widget.
Basically, this class must inherit :py:mod:`~ikomia.core.pycore.CWorkflowTaskParam` (C++ base class). It should include definition of your parameters variables as member and reimplement 2 methods:

- :py:meth:`~ikomia.core.pycore.CWorkflowTaskParam.setParamMap`: called when loading workflow from file or from Ikomia Studio internal database. It must set member variable values from a dict-like structure (key-value pairs).
- :py:meth:`~ikomia.core.pycore.CWorkflowTaskParam.getParamMap`: called when saving workflow in file or in Ikomia Studio internal database. It returns parameter keys and values as a dict-like structure.

.. code-block:: python

    from ikomia import core

    class MyParam(core.CWorkflowTaskParam):

        def __init__(self):
            core.CWorkflowTaskParam.__init__(self)
            self.kernel_size = (3, 3)
            self.sigma_x = 1.0
            self.sigma_y = 1.0

        def setParam(self, param_map):
            self.kernel_size = (int(param_map["kernel_size_x"]), int(param_map["kernel_size_y"]))
            self.sigma_x = int(param_map["sigma_x"])
            self.sigma_y = int(param_map["sigma_y"])

        def getParam(self):
            param_map = core.ParamMap()
            param_map["kernel_size_x"] = str(self.kernel_size[0])
            param_map["kernel_size_y"] = str(self.kernel_size[1])
            param_map["sigma_x"] = str(self.sigma_x)
            param_map["sigma_y"] = str(self.sigma_y)
            return param_map


.. important:: the dict-like structure handles only **str** type for both key and value. Thus, one should take care to set valid conversion from the original parameter type (int, float, bool...) to str.


Inherit from the pure Python base class
---------------------------------------

As an option, it is possible to inherit your parameter class from a pure Python base class 
(:py:mod:`~ikomia.core.task.TaskParam`) that make it easier. Indeed, parameters are managed directly 
in a dict member instead of defining one variable for each parameter. Another small advantage is that 
the method *getParamMap()* has not to be reimplemented anymore.

.. code-block:: python

    from ikomia.core.task import TaskParam

    class MyParam(TaskParam):

        def __init__(self):
            core.TaskParam.__init__(self)
            self.cfg["kernel_size"] = (3, 3)
            self.cfg["sigma_x"] = 1.0
            self.cfg["sigma_y"] = 1.0

        def setParam(self, param_map):
            self.cfg["kernel_size"] = (int(param_map["kernel_size_x"]), int(param_map["kernel_size_y"]))
            self.cfg["sigma_x"] = int(param_map["sigma_x"])
            self.cfg["sigma_y"] = int(param_map["sigma_y"])
