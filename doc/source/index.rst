Welcome to Ikomia's documentation
=================================

:Current version: |version|

The aim of this API is to offer a simple interface to add user-defined plugin into the Ikomia platform. A plugin is a Python implementation of an algorithm executed on image based data.

The software gives the possibility to build custom image processing pipelines through the use of a graph-based editor called **workflow**. A workflow is thus composed of several connected tasks that get inputs, process them and give results through outputs. When you create your own task (i.e. plugin), there are some few essential components to deal with:

    - :Task: algorithm implementation
    - :Parameters: to setup the algorithm
    - :Metadata: information attached to the task (description, icon, authors, articles...)
    - :Inputs: required input data
    - :Outputs: produced output data
    - :Widget: user interaction interface (to adjust parameters for example)

Each of these components is implemented by a specific class. We use inheritance and factory design pattern to build the interface between Ikomia software and plugins. Once implemented, you will be able to handle your plugin like all others internal processes. It means that you will be able to add it to a workflow and visualize your outputs directly.

The API is divided into modules addressing specific features:
    - **Utils**: module offering various helper tools organized in different submodules
        - :py:mod:`~ikomia.utils.pyutils`: helper classes which are bindings from C++ libraries
        - :py:mod:`~ikomia.utils.pyqtutils`: helper functions simplifying widgets creation
        - :py:mod:`~ikomia.utils.qtconversion`: helper functions providing conversion from Python Qt framework (PyQt or PySide2) to C++ Qt
    - **Core**: module offering essential base classes to implement input/ouput, task, parameters and widget.
        - :py:mod:`~ikomia.core.pycore`: bindings from C++ libraries
    - **DataProcess**:module offering the main interface structure to plug user-defined task and several class specializations to facilitate common process implementation.
        - :py:mod:`~ikomia.dataprocess.pydataprocess`: bindings from C++ libraries
    - **DNN**: module providing features for Deep Learning tasks. More details :doc:`here<modules>`


Getting started
===============
This section will help you understand the API structure and will guide you through the creation of your first plugins.

.. toctree::
    :maxdepth: 1

    python_env
    dependencies


Tutorials

.. toctree::
    :maxdepth: 1

    hello_world_plugin
    opencv_plugin
    interactive_plugin


API Reference
=============
This section provides the auto-generated documentation from the Python code. You will find all the implementation details and
the comprehensive list of available functions.

.. toctree::
   :maxdepth: 2

   modules

Examples
========
Here are source codes associated with the tutorials.

.. toctree::
    :maxdepth: 2

    examples


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
