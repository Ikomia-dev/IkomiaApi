# Ikomia API

![](https://ikomia.com/static/showcase/img/home/plugin.png)

This project provides core features for [Ikomia Studio software](https://github.com/Ikomia-dev/IkomiaStudio) and Ikomia API to create and plug your own algorithms. It consists in C++ librairies, Python wrappers from C++ and pure Python modules.

## C++ libraries

- **ikUtils**: helper classes implementing design patterns, common widgets, containers...
- **ikCore**: core classes to handle data containers, workflow task, task input/output, task widget and graphics.
- **ikProcess**: workflow structure and base classes for algorithm implementation.
- **ikDataIO**: data readers and writers
- **ikDataManagement**: data management interface
- **ikDataPlot**: specific module for plots management
- **ikVolumeRender**: volume rendering engine based on OpenCL

## Python modules

Boost Python Wrappers from C++:
- **PyUtils**: partial wrapper of ikUtils.
- **PyCore**: partial wrapper of ikCore.
- **PyDataProcess**: partial wrapper of ikDataProcess.
- **PyDataIO**: partial wrapper of ikDataIO.

Pure Python modules (source code in Python subfolder):
- **pyqtutils**: helper functions simplifying widgets creation.
- **qtconversion**: helper functions providing conversion from Python Qt framework (PyQt or PySide2) to C++ Qt.
- **dnn**: module providing features for Deep Learning tasks.

## Documentation

For those interested in plugin integration, we strongly advice to use the Python API. Documentation can be found [here](https://ikomia-dev.github.io/python-api-documentation/). You will find plugin examples in [our main GitHub account](https://github.com/Ikomia-dev).

It is also possible to generate documentation automatically for the C++ part using Doxygen (doxyfiles are location in the *Doc* folder).

## Compilation

Coming soon...
