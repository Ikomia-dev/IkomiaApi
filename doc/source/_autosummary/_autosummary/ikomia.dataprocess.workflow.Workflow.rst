Workflow
========

.. currentmodule:: ikomia.dataprocess.workflow

.. autoclass:: Workflow

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess.workflow import Workflow


    .. rubric:: Methods
    .. autosummary::

        ~Workflow.__init__
        ~Workflow.add_task
        ~Workflow.connect_tasks
        ~Workflow.find_task
        ~Workflow.get_array_output
        ~Workflow.get_blob_measure_output
        ~Workflow.get_dataset_output
        ~Workflow.get_graphics_output
        ~Workflow.get_image
        ~Workflow.get_image_output
        ~Workflow.get_image_with_graphics
        ~Workflow.get_numeric_output
        ~Workflow.get_data_string_output
        ~Workflow.get_path_output
        ~Workflow.get_tasks
        ~Workflow.get_time_metrics
        ~Workflow.run
        ~Workflow.run_on
        ~Workflow.set_directory_input
        ~Workflow.set_image_input
        ~Workflow.set_video_input
        ~Workflow.set_parameters


    .. rubric:: Inherited methods
    .. autosummary::

        ~Workflow.addInput
        ~Workflow.clear
        ~Workflow.clearInputs
        ~Workflow.clearOutputData
        ~Workflow.deleteEdge
        ~Workflow.deleteTask
        ~Workflow.exportGraphviz
        ~Workflow.getChildren
        ~Workflow.getEdgeInfo
        ~Workflow.getEdgeSource
        ~Workflow.getEdgeTarget
        ~Workflow.getElapsedTimeTo
        ~Workflow.getFinalTasks
        ~Workflow.getInEdges
        ~Workflow.getOutEdges
        ~Workflow.getParents
        ~Workflow.getRequiredTasks
        ~Workflow.getRootID
        ~Workflow.getRootTargetTypes
        ~Workflow.getTask
        ~Workflow.getTaskCount
        ~Workflow.getTaskIDs
        ~Workflow.getTotalElapsedTime
        ~Workflow.load
        ~Workflow.removeInput
        ~Workflow.setAutoSave
        ~Workflow.setInput
        ~Workflow.setOutputFolder
        ~Workflow.save
        ~Workflow.stop
        ~Workflow.updateStartTime


    .. rubric:: Details

    .. automethod:: __init__
    .. automethod:: addInput
    .. automethod:: add_task
    .. automethod:: clear
    .. automethod:: clearInputs
    .. automethod:: clearOutputData
    .. automethod:: connect_tasks
    .. automethod:: deleteEdge
    .. automethod:: deleteTask
    .. automethod:: exportGraphviz
    .. automethod:: find_task
    .. automethod:: getChildren
    .. automethod:: get_array_output
    .. automethod:: get_blob_measure_output
    .. automethod:: get_dataset_output
    .. automethod:: getEdgeInfo
    .. automethod:: getEdgeSource
    .. automethod:: getEdgeTarget
    .. automethod:: getElapsedTimeTo
    .. automethod:: getFinalTasks
    .. automethod:: get_graphics_output
    .. automethod:: get_image
    .. automethod:: get_image_output
    .. automethod:: get_image_with_graphics
    .. automethod:: getInEdges
    .. automethod:: get_numeric_output
    .. automethod:: get_data_string_output
    .. automethod:: getOutEdges
    .. automethod:: getParents
    .. automethod:: get_path_output
    .. automethod:: getRequiredTasks
    .. automethod:: getRootID
    .. automethod:: getRootTargetTypes
    .. automethod:: getTask
    .. automethod:: getTaskCount
    .. automethod:: getTaskIDs
    .. automethod:: get_tasks
    .. automethod:: get_time_metrics
    .. automethod:: getTotalElapsedTime
    .. automethod:: load
    .. automethod:: removeInput
    .. automethod:: run
    .. automethod:: run_on
    .. automethod:: setAutoSave
    .. automethod:: set_directory_input
    .. automethod:: set_image_input
    .. automethod:: set_video_input
    .. automethod:: setInput
    .. automethod:: setOutputFolder
    .. automethod:: set_parameters
    .. automethod:: save
    .. automethod:: stop
    .. automethod:: updateStartTime
