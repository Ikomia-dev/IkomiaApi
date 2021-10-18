Going deeper with workflows
===========================


Set workflow inputs
-------------------

In the previous section, we use :py:meth:`~ikomia.dataprocess.workflow.Workflow.run_on` to execute a workflow on common inputs. 
But this function can only be used for a workflow with a single input. In fact, a workflow object is also a task 
(:py:mod:`~ikomia.dataprocess.workflow.Workflow` is derived from :py:mod:`~ikomia.core.CWorkflowTask`). Please find below 
the generic way to set workflow inputs.

.. code-block:: python

    from ikomia.core import IODataType
    from ikomia.dataprocess import workflow, CImageIO

    wf = workflow.create("MyWorkflow")
    # create image inputs
    first_input = CImageIO(IODataType.IMAGE, "1st", "path/to/image1")
    second_input = CImageIO(IODataType.IMAGE, "2nd", "path/to/image2")
    # add 2 inputs
    wf.addInput(first_input)
    wf.addInput(second_input)
    # Or
    wf.setInput(first_input, 0)
    wf.setInput(second_input, 1)

See :py:meth:`~ikomia.dataprocess.workflow.Workflow.addInput` and :py:meth:`~ikomia.dataprocess.workflow.Workflow.setInput` for details.

The :py:mod:`~ikomia.dataprocess.workflow.Workflow` class also provides helper functions to ease the management of image inputs. You can use 
:py:meth:`~ikomia.dataprocess.workflow.Workflow.set_image_input` and :py:meth:`~ikomia.dataprocess.workflow.Workflow.set_directory_input` 
for this specific data type.


Get a specific workflow task
----------------------------

In some cases, it may be necessary to access the task object instance directly. The :py:mod:`~ikomia.dataprocess.workflow.Workflow` class provides 
a single method to get a task from its name: :py:meth:`~ikomia.dataprocess.workflow.Workflow.find_task`. When a workflow has several tasks with the 
same name, the function returns the list of candidates. Please note that the candidates are sorted according to their order of insertion in the workflow, 
from the oldest to the most recent. You can also specify a zero-based index (insertion order) to get the wanted task only.

Considering the following workflow:

.. image:: _static/workflow_example1.jpg

.. code-block:: python

    from ikomia.dataprocess import workflow

    wf = workflow.load("path/to/workflow")
    # Get the bilateral filter task
    bilateral_id, bilateral_obj = wf.find_task("ocv_bilateral_filter")
    # Get the candidates for Canny filter
    # canny_candidates is a list of pairs (id, obj) here
    canny_candidates = wf.find_task("ocv_canny")
    # Get the first inserted Canny filter task
    first_canny_id, first_canny_obj = wf.find_task("ocv_canny", 0)
    # Get the second inserted Canny filter task
    second_canny_id, second_canny_obj = wf.find_task("ocv_canny", 1)

.. note::
    When you create a workflow from scratch, the function :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task` returns a pair (id, obj) that could be used 
    to get the task later. In this case, you should call :py:meth:`~ikomia.dataprocess.workflow.Workflow.getTask` with the corresponding id instead of 
    :py:meth:`~ikomia.dataprocess.workflow.Workflow.find_task` (faster).


Get workflow outputs
--------------------

The generic way to get workflow outputs is to firstly get the task object and then get its outputs.

.. code-block:: python

    from ikomia.dataprocess import workflow

    wf = workflow.create("ObjectDetection")
    # Store id and object instance while adding task to the workflow
    yolov4_id, yolov4_obj = wf.add_task("infer_yolo_v4")
    wf.connect_tasks(wf.getRootID(), yolov4_id)

    wf.run_on(path="path/to/image.png")

    # Optional use find_task
    yolov4_id, yolov4_obj = wf.find_task("infer_yolo_v4")

    # Get outputs
    # 1 - Forwarded source image: CImageIO object
    img_output = yolov4_obj.getOutput(0)
    # 2 - Graphics objects (box): CGraphicsOutput object
    graphics_output = yolov4_obj.getOutput(1)
    # 3- Box information: CBlobMeasureIO object
    box_output = yolov4_obj.getOutput(2)

The :py:mod:`~ikomia.dataprocess.workflow.Workflow` class provides convenient methods to get outputs by type. For all these methods, 
you could get outputs from the task name or the task id. When using task name, the methods will return a list of outputs for all maching 
tasks. When a task has several outputs of the same type, you can specify the output index (zero-based index of all outputs).

- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_image_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_graphics_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_numeric_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_blob_measure_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_dataset_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_array_output`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.get_path_output`

.. code-block:: python

    from ikomia.dataprocess import workflow

    wf = workflow.create("ObjectDetection")
    # Store id and object instance while adding task to the workflow
    yolov4_id, yolov4_obj = wf.add_task("infer_yolo_v4")
    wf.connect_tasks(wf.getRootID(), yolov4_id)

    wf.run_on(path="path/to/image.png")

    img_output = wf.get_image_output(yolov4_id)
    graphics_output = wf.get_graphics_output(yolov4_id)
    box_output = wf.get_blob_measure_output(yolov4_id)

.. note::
    We provide high-level methods to handle image outputs. Thus from a workflow object, you can access image array directly  
    (:py:meth:`~ikomia.dataprocess.workflow.Workflow.get_image`) or compute the merge image (for visualization) between graphics and image 
    outputs (:py:meth:`~ikomia.dataprocess.workflow.Workflow.get_image_with_graphics`).


Handle the workflow graph structure
-----------------------------------

A workflow is a graph where the nodes are the runnable tasks (algorithms) and the links between nodes are connections from task outputs to task inputs.
Basically, you need two methods to build your own workflow:

- :py:meth:`~ikomia.dataprocess.workflow.Workflow.add_task`
- :py:meth:`~ikomia.dataprocess.workflow.Workflow.connect_tasks`


Create a Deep Learning training workflow
----------------------------------------


Working with video
------------------


Get workflow time metrics
-------------------------

