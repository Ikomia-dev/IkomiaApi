The aim of this API is to offer a simple interface to add user-defined plugin into the Ikomia platform. In the Ikomia environment, a plugin is the implementation of an algorithm processing image based data.

The software gives the possibility to build custom image processing pipelines through the use of a graph called *Protocol*. A protocol is thus composed of several connected tasks that get inputs, process it and gives results through outputs. When you create your own task (or plugin), there are some few essential components to deal with: 
* the task, where the algorithm is implemented
* the parameters, to setup the algorithm
* the metadata, information we want to attach to the task (description, icon, authors, articles...)
* the inputs, what the task needs to run
* the outputs, what the task produces
* the widget (or window), to allow user interaction (to adjust parameters for example)

Each of this components is implemented by a specific class. We use inheritance and factory design pattern to build the interface between software and plugins. Once implemented, you will be able to handle your plugin like all others internal process proposed by the software. It means that you will be able to add it to a protocol and visualize your outputs directly.

The API is composed of three main modules:
* @ref groupUtils : module offering various tools
* @ref groupCore : module offering essential base classes to implement input/ouput, task, task parameters and widget.
* @ref groupDataProcess : module offering the main interface structure and several class specializations to facilitate the implementation of common process.

The API is available in C++ and Python.