Create your algorithm for Ikomia platform
=========================================

Ikomia platform is an Open Source ecosystem providing plug'n'play algorithms in Computer Vision. Such algorithms can then be tested and evaluated in 
Ikomia Studio for fast prototyping. Ikomia Studio is a desktop software (Qt based) that will help you to access and evaluate state of the art algorithms
in a no-code way. It includes built-in algorithms from well-known libraries like **OpenCV** and give access to Ikomia HUB. Ikomia Studio offers
the possibility to build complex **workflows** thanks to a flexible graph-based editor.

The aim of this API is to offer a simple Python interface to add user-defined plugin into Ikomia platform. A plugin is a Python implementation of an 
algorithm executed on image based data. Once implemented, you will be able to handle your plugin like all other algorithms and use it in workflows.
You will also be able to publish it to the Ikomia HUB and promote your work towards Ikomia community.

The plugin structure
--------------------

In Ikomia platform, each plugin is intended for use in workflows to process image-based data. A workflow is thus composed of several connected tasks that get inputs,
process them and give results through outputs. When you create your own plugin, there are some few essential components to deal with, each one being implemented by a specific class:

    - **Task**: algorithm implementation, inherited from :py:mod:`~ikomia.core.pycore.CWorkflowTask`.
    - **Parameters**: to setup the algorithm, inherited from :py:mod:`~ikomia.core.pycore.CWorkflowTaskParam`.
    - **Inputs**: required input data, inherited from :py:mod:`~ikomia.core.pycore.CWorkflowTaskIO`.
    - **Outputs**: produced output data, inherited from :py:mod:`~ikomia.core.pycore.CWorkflowTaskIO`.
    - **Widget**: user interaction interface (to adjust parameters for example), inherited from :py:mod:`~ikomia.core.pycore.CWorkflowTaskWidget`.


Then, each plugin must follow the connection mechanism of Ikomia Studio by providing three classes:

    - **Task factory**: create task object. It also stores all information relative to the algorithm (unique name, description, icon, authors, articles, keywords...). Inherited from :py:mod:`~ikomia.dataprocess.pydataprocess.CTaskFactory`.
    - **Widget factory**: create widget object. It must set the same unique name as the task factory. Inherited from :py:mod:`~ikomia.dataprocess.pydataprocess.CWidgetFactory`.
    - **Main plugin interface**: allow to get the two factories described above. Inherited from :py:mod:`~ikomia.dataprocess.pydataprocess.CPluginProcessInterface`.


Finally, the minimal file structure of a plugin could be:

    - my_plugin_name (folder)

        - __init__.py
        - my_plugin_name.py: implementation of the main plugin interface.
        - my_plugin_name_process.py: implementation of the algorithm, its parameters and its factory.
        - my_plugin_name_widget.py: implementation of the widget and its factory.


Getting started
---------------

You are almost ready to write your first Ikomia plugin. We just need to introduce how the Python environment is managed in Ikomia Studio.
As a C++/Qt software, Ikomia Studio embeds a specific version of the Python interpreter.
So you need to use this environment while implementing your plugin. More details are given in the following pages:

.. toctree::
    :maxdepth: 1

    python_env
    dependencies
    naming


These tutorial sections will help you understand the API structure and will guide you through the creation of your first plugins:

.. toctree::
    :maxdepth: 1

    hello_world_plugin
    opencv_plugin
    interactive_plugin


Going deeper
------------

.. toctree::
    :maxdepth: 1

    plugin_parameters
    plugin_task
    plugin_io


Examples
--------

Here are source codes associated with the tutorials.

.. toctree::
    :maxdepth: 2

    examples