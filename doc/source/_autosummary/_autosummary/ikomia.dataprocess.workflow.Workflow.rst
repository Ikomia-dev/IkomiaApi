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
        ~Workflow.get_task_id
        ~Workflow.get_task_output
        ~Workflow.get_tasks
        ~Workflow.get_time_metrics
        ~Workflow.load
        ~Workflow.remove_task
        ~Workflow.root
        ~Workflow.run
        ~Workflow.run_on
        ~Workflow.set_directory_input
        ~Workflow.set_image_input
        ~Workflow.set_video_input
        ~Workflow.set_parameters


    .. rubric:: Inherited methods
    .. autosummary::

        ~Workflow.add_input
        ~Workflow.clear
        ~Workflow.clear_inputs
        ~Workflow.clear_output_data
        ~Workflow.delete_edge
        ~Workflow.delete_task
        ~Workflow.export_graphviz
        ~Workflow.get_children
        ~Workflow.get_edge_info
        ~Workflow.get_edge_source
        ~Workflow.get_edge_target
        ~Workflow.get_elapsed_time_to
        ~Workflow.get_final_tasks
        ~Workflow.get_in_edges
        ~Workflow.get_out_edges
        ~Workflow.get_parents
        ~Workflow.get_required_tasks
        ~Workflow.get_root_id
        ~Workflow.get_root_target_types
        ~Workflow.get_task
        ~Workflow.get_task_count
        ~Workflow.get_task_ids
        ~Workflow.get_total_elapsed_time
        ~Workflow.load
        ~Workflow.remove_input
        ~Workflow.set_auto_save
        ~Workflow.set_input
        ~Workflow.set_output_folder
        ~Workflow.save
        ~Workflow.stop
        ~Workflow.update_start_time


    .. rubric:: Details

    .. automethod:: __init__
    .. automethod:: add_input
    .. automethod:: add_task
    .. automethod:: clear
    .. automethod:: clear_inputs
    .. automethod:: clear_output_data
    .. automethod:: connect_tasks
    .. automethod:: delete_edge
    .. automethod:: delete_task
    .. automethod:: export_graphviz
    .. automethod:: find_task
    .. automethod:: get_children
    .. automethod:: get_edge_info
    .. automethod:: get_edge_source
    .. automethod:: get_edge_target
    .. automethod:: get_elapsed_time_to
    .. automethod:: get_final_tasks
    .. automethod:: get_in_edges
    .. automethod:: get_out_edges
    .. automethod:: get_parents
    .. automethod:: get_required_tasks
    .. automethod:: get_root_id
    .. automethod:: get_root_target_types
    .. automethod:: get_task
    .. automethod:: get_task_id
    .. automethod:: get_task_count
    .. automethod:: get_task_ids
    .. automethod:: get_task_output
    .. automethod:: get_tasks
    .. automethod:: get_time_metrics
    .. automethod:: get_total_elapsed_time
    .. automethod:: load
    .. automethod:: remove_input
    .. automethod:: remove_task
    .. automethod:: root
    .. automethod:: run
    .. automethod:: run_on
    .. automethod:: set_auto_save
    .. automethod:: set_directory_input
    .. automethod:: set_image_input
    .. automethod:: set_video_input
    .. automethod:: set_input
    .. automethod:: set_output_folder
    .. automethod:: set_parameters
    .. automethod:: save
    .. automethod:: stop
    .. automethod:: update_start_time
