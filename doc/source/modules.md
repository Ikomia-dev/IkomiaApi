# API Reference

Ikomia API is divided into 4 main modules, each one may contain sub-modules. A large part of this API is based on
C++ bindings, those classes are easily identifiable since we apply a naming convention where all C++ classes begin with
the C capital letter.

```{eval-rst}
.. rubric:: Utils module
.. autosummary::
    :toctree: _autosummary

    ikomia.utils.pyutils
    ikomia.utils.pyqtutils
    ikomia.utils.qtconversion
    ikomia.utils.displayIO


.. rubric:: Core module
.. autosummary::
    :toctree: _autosummary

    ikomia.core.pycore
    ikomia.core.task


.. rubric:: DataProcess module
.. autosummary::
    :toctree: _autosummary

    ikomia.dataprocess.pydataprocess
    ikomia.dataprocess.registry
    ikomia.dataprocess.workflow
    ikomia.dataprocess.datadictIO


.. rubric:: DNN module
.. autosummary::
    :toctree: _autosummary

    ikomia.dnn.dnntrain
    ikomia.dnn.datasetio
    ikomia.dnn.dataset
    ikomia.dnn.torch.models
    ikomia.dnn.torch.datasetmapper
```