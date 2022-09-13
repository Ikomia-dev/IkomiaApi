First steps with workflows
==========================

Workflows are acyclic directed graph composed by nodes (algorithms) and connections from node outputs to adjacent node inputs. Output and inputs must have compatible data types to be connected, otherwise connection 
will fail and raise exception. The API provides all features needed to handle such graph:

- create nodes (algorithms) identified by their unique name
- connect them through their I/O
- set the workflow global inputs
- set algorithm parameters
- run workflows
- save workflows to file
- load workflows from file


Get the list of available algorithms
------------------------------------

In Ikomia, algorithms are identified by their names. So one of the most important thing when using this API is to know the list of possible algorithms. 
We can differentiate 2 kinds of algorithms: those available on the host system (built-in + installed plugins) and those available on Ikomia HUB.
As introduced in the previous section, algorithms management is done by the global Ikomia registry.

Get names of built-in and installed algorithms:

.. code-block:: python

    import ikomia

    names = ikomia.ik_registry.getAlgorithms()

Get information (name, authors, description...) of Ikomia HUB algorithms:

.. code-block:: python

    import ikomia

    algos = ikomia.ik_registry.get_online_algorithms()
    for algo in algos:
        name = algo["name"]

.. note::
    We provide an auto-completion mechanism proposing available algorithms while coding (for built-in and installed algorithms only). Is is available under the **ik** namespace.

    .. figure:: _static/auto_completion.jpg

        Auto completion for algorithms name


Create workflow from scratch
------------------------------

The sample below show how to instanciate algorithms and connect them in a runnable workflow.

.. code-block:: python

    from ikomia.utils import ik
    from ikomia.dataprocess import workflow

    wf = workflow.create("FromScratch")

    # Instanciate Box Filter algorithm
    box_filter_id, box_filter = wf.add_task("ocv_box_filter")
    # Or
    box_filter_id, box_filter = wf.add_task(ik.ocv_box_filter)
    # Connection to the root node (mandatory)
    wf.connect_tasks(wf.getRootID(), box_filter_id)
    
    # Instanciate CLAHE algorithm
    clahe_id, clahe = wf.add_task(ik.ocv_clahe)
    # Connection with Box filter
    wf.connect_tasks(box_filter_id, clahe_id)
    
    # Set image input of the workflow
    img_path = "my_image_folder/my_image.png"
    wf.set_image_input(path=img_path)
    # Run workflow
    wf.run()

See :py:meth:`~ikomia.dataprocess.workflow.create`, :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`, :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks`  and 
:py:meth:`~ikomia.dataprocess.workflow.Workflow.run` for details.


Save workflow to JSON
---------------------

Workflows can be exported in human-readable format (JSON).

.. code-block:: python

    from ikomia.dataprocess import workflow

    wf = workflow.create("FromScratch")
    # Build your workflow here
    wf.save("path/to/workflow.json")

See :py:meth:`~ikomia.dataprocess.workflow.Workflow.save` for details.


Load workflow from JSON
-----------------------

Workflows can be loaded from JSON file. A classic use case is to build your workflow with Ikomia Studio (no code) and export it as JSON. 
Then you can simply execute this workflow everywhere with the following code: 

.. code-block:: python

    from ikomia.dataprocess import workflow

    # Load workflow
    wf = workflow.load("path/to/workflow.json")
    # Set image input of the workflow
    img_path = "my_image_folder/my_image.png"
    wf.set_image_input(path=img_path)
    # Run workflow
    wf.run()

See :py:meth:`~ikomia.dataprocess.workflow.load` for details.


Run workflow on common inputs
-----------------------------

Ikomia API provides features to run workflow on classic image-based inputs.

.. code-block:: python

    import numpy as np
    from ikomia.dataprocess import workflow

    wf = workflow.load("path/to/workflow.json")

    # Run on image from array
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    wf.run_on(array=img_array)

    # Run on image from file path
    img_path = "path/to/image.png"
    wf.run_on(path=img_path)

    # Run on image from URL
    img_url = "http://url/to/image.jpg"
    wf.run_on(url=img_url)

    # Run on images from folder
    folder_path = "path/to/folder"
    wf.run_on(folder=folder_path)

See :py:meth:`~ikomia.dataprocess.workflow.Workflow.run_on` for details.


Set parameters of workflow algorithms
-------------------------------------

Each algorithm comes with its own set of parameters. From a workflow object instance, you have access to each algorithm with :py:meth:`~ikomia.dataprocess.workflow.Workflow.find_task`.
It returns the task object instance and id from which you can then set parameters. 

.. code-block:: python

    from ikomia.utils import ik
    from ikomia.dataprocess import workflow    

    # Load workflow
    wf = workflow.load("path/to/workflow.json")
    # Get specific algorithm of the workflow
    box_filter_id, box_filter = wf.find_task(name=ik.ocv_box_filter)
    # Set new parameters
    new_params = { ik.ocv_box_filter_param.kSizeHeight: 11, ik.ocv_box_filter_param.kSizeWidth: 11 }
    wf.set_parameters(new_params, task_id=box_filter_id)

With name only:

.. code-block:: python

    from ikomia.utils import ik
    from ikomia.dataprocess import workflow    

    # Load workflow
    wf = workflow.load("path/to/workflow.json")
    # Set new parameters
    new_params = { ik.ocv_box_filter_param.kSizeHeight: 11, ik.ocv_box_filter_param.kSizeWidth: 11 }
    wf.set_parameters(new_params, task_name=ik.ocv_box_filter)

.. note::
    Like for algorithm names, you can use the **ik** namespace to have auto-completion for parameter names. In the **ik** namespace,
    you will find a sub-namespace for each algorithm: *name_of_algorithm_param*.
    
.. important::
    The example above assumes that there is only one Box Filter algorithm in the workflow. If it is not the case, you can set a keywords argument **index** in
    both :py:meth:`~ikomia.dataprocess.workflow.Workflow.find_task` and :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_parameters`.