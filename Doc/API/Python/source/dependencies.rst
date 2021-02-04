Dependencies management
=======================

As of any Python project, an Ikomia plugin may have some Python module dependencies. 
The philosophy behind Ikomia is to offer ready to use algorithms to users.
From the plugin author perspective, it means that the plugin must include directives
to install these dependencies automatically. 

To achieve this goal, Ikomia software includes a system based on pip requirements files.
During the plugin installation, Ikomia software will parse the plugin directory to find requirements files
and install dependencies automatically.

**Before publishing plugins, authors must include the necessary requirements files.**


Requirements file structure
---------------------------

Ikomia dependency management is fully based on pip. Please consult the pip official documentation
to know about all features:

- `pip user guide <https://pip.pypa.io/en/stable/user_guide/>`_
- `requirements file format <https://pip.pypa.io/en/stable/reference/pip_install/#requirements-file-format>`_


Multi-steps installation
------------------------

In some cases, it could be necessary to split the installation process to force installation of dependencies 
prior to others. As such, Ikomia dependency system handles multiple requirements files 
and apply alphanumeric sort to define installation order. Authors can then insert several requirements file into the 
plugin directory to solve dependencies issues. 

Example of plugin structure:

- MyPlugin

    - __init__.py
    - MyPlugin.py
    - MyPlugin_process.py
    - MyPlugin_widget.py
    - requirements1.txt
    - requirements2.txt

In this configuration, Ikomia software will first install Python packages listed *requirements1.txt* and 
then those listed in the file *requirements2.txt*.


Limitations
-----------

The standard way to manage dependencies in Python and its pip package installer is to provide a valid *setup.py* script.
Then, Python package can either be distributed from Wheel or from source. For the moment, Ikomia software can only install
dependencies available in the official Python Package Index (PyPI) or via a git repository. Support for manual installation
through direct call to *setup.py* script (setuptools) inside the plugin is in our roadmap and will be available soon.
