Working with tasks
==================

The recommended way to apply algorithms with Ikomia API is through the :py:mod:`~ikomia.dataprocess.workflow` module. 
However, you have also access to algorithm at task level with the possibility to run it independently.


Run task without workflow
-------------------------

The example below illustrates the steps to run a single algorithm.

.. code-block:: python

    from ikomia.core import task
    from ikomia.utils import ik

    # Instanciate algorithm
    box_filter = task.create(ik.ocv_box_filter)

    # Set input(s)
    img = cv2.imread("path/to/image.png")
    img_input = box_filter.getInput(0)
    img_input.setImage(img)

    # Run
    box_filter.run()

Consult :py:meth:`~ikomia.core.task.create`, :py:meth:`~ikomia.core.pycore.CWorkflowTask.getInput` and 
:py:meth:`~ikomia.core.pycore.CWorkflowTask.run` for details.


Manage parameters
-----------------

Algorithm behavior can be adjusted by setting task parameters (:py:meth:`~ikomia.core.task.set_parameters`).

.. code-block:: python

    from ikomia.core import task
    from ikomia.utils import ik

    # Instanciate algorithm
    box_filter = task.create(ik.ocv_box_filter)

    # ...

    # Set parameters
    box_filter_params = {
        ik.ocv_box_filter_param.kSizeHeight: 11,
        ik.ocv_box_filter_param.kSizeWidth: 11
    }
    task.set_parameters(box_filter, box_filter_params)

    # Run
    box_filter.run()


Manage inputs/outputs
---------------------

Inputs
^^^^^^

From a task object instance, you access inputs with :py:meth:`~ikomia.core.pycore.CWorkflowTask.getInput`. 
Depending on algorithm, inputs can be of various type. Please consult this :doc:`page<plugin_io>` to have more 
information about possible input types.

Outputs
^^^^^^^

Like for workflows, the :py:mod:`~ikomia.core.task` module provides useful functions to get common output types:

- :py:meth:`~ikomia.core.task.get_image_output`
- :py:meth:`~ikomia.core.task.get_graphics_output`
- :py:meth:`~ikomia.core.task.get_numeric_output`
- :py:meth:`~ikomia.core.task.get_data_string_output`
- :py:meth:`~ikomia.core.task.get_blob_measure_output`
- :py:meth:`~ikomia.core.task.get_dataset_output`
- :py:meth:`~ikomia.core.task.get_array_output`
- :py:meth:`~ikomia.core.task.get_path_output`

Here is a example with image output:

.. code-block:: python

    from ikomia.core import task
    from ikomia.utils import ik

    # Instanciate algorithm
    box_filter = task.create(ik.ocv_box_filter)

    # Set input(s)
    img = cv2.imread("path/to/image.png")
    img_input = box_filter.getInput(0)
    img_input.setImage(img)

    # Run
    box_filter.run()

    # Get output
    img_out = task.get_image_output(box_filter)
    cv2.imshow("Box Filter result", img_out.getImage())

.. note::
    We provide high-level function to handle image and graphics outputs. Thus you can compute the merge image (for visualization) between graphics and image 
    outputs (:py:meth:`~ikomia.core.task.get_image_with_graphics`).