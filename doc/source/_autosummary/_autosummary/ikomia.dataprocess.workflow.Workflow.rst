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
        ~Workflow.get_time_metrics
        ~Workflow.run
        ~Workflow.set_directory_input
        ~Workflow.set_image_input


    .. rubric:: Inherited methods
    .. autosummary::

        ~Workflow.addInput
        ~Workflow.clear
        ~Workflow.clearInputs
        ~Workflow.clearOutputData
        ~Workflow.deleteEdge
        ~Workflow.deleteTask
        ~Workflow.exportGraphviz
        ~Workflow.getChilds
        ~Workflow.getEdgeInfo
        ~Workflow.getEdgeSource
        ~Workflow.getEdgeTarget
        ~Workflow.getElapsedTimeTo
        ~Workflow.getFinalTasks
        ~Workflow.getInEdges
        ~Workflow.getOutEdges
        ~Workflow.getParents
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
    .. automethod:: getChilds
    .. automethod:: getEdgeInfo
    .. automethod:: getEdgeSource
    .. automethod:: getEdgeTarget
    .. automethod:: getElapsedTimeTo
    .. automethod:: getFinalTasks
    .. automethod:: getInEdges
    .. automethod:: getOutEdges
    .. automethod:: getParents
    .. automethod:: getRootID
    .. automethod:: getRootTargetTypes
    .. automethod:: getTask
    .. automethod:: getTaskCount
    .. automethod:: getTaskIDs
    .. automethod:: get_time_metrics
    .. automethod:: getTotalElapsedTime
    .. automethod:: load
    .. automethod:: removeInput
    .. automethod:: run
    .. automethod:: setAutoSave
    .. automethod:: set_directory_input
    .. automethod:: set_image_input
    .. automethod:: setInput
    .. automethod:: setOutputFolder
    .. automethod:: save
    .. automethod:: stop
    .. automethod:: updateStartTime
