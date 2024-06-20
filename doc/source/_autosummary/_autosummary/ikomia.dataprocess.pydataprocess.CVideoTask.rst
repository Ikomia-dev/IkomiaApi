CVideoTask
==========

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CVideoTask

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CVideoTask

    .. rubric:: Methods
    .. autosummary::
        ~CVideoTask.__init__
        ~CVideoTask.notify_video_end
        ~CVideoTask.notify_video_start

    .. rubric:: Overridden methods
    .. autosummary::
        ~CVideoTask.begin_task_run

    .. rubric:: Inherited methods
    .. autosummary::
        ~CVideoTask.add_input
        ~CVideoTask.add_output
        ~CVideoTask.apply_graphics_mask
        ~CVideoTask.apply_graphics_mask_to_binary
        ~CVideoTask.create_graphics_mask
        ~CVideoTask.emit_add_sub_progress_steps
        ~CVideoTask.emit_graphics_context_changed
        ~CVideoTask.emit_output_changed
        ~CVideoTask.emit_step_progress
        ~CVideoTask.end_task_run
        ~CVideoTask.execute_actions
        ~CVideoTask.forward_input_image
        ~CVideoTask.get_elapsed_time
        ~CVideoTask.get_graphics_mask
        ~CVideoTask.get_input
        ~CVideoTask.get_input_count
        ~CVideoTask.get_input_data_type
        ~CVideoTask.get_inputs
        ~CVideoTask.get_output
        ~CVideoTask.get_output_count
        ~CVideoTask.get_output_data_type
        ~CVideoTask.get_outputs
        ~CVideoTask.get_parameters
        ~CVideoTask.get_progress_steps
        ~CVideoTask.global_input_changed
        ~CVideoTask.graphics_changed
        ~CVideoTask.is_graphics_changed_listening
        ~CVideoTask.is_mask_available
        ~CVideoTask.parameters_modified
        ~CVideoTask.remove_input
        ~CVideoTask.run
        ~CVideoTask.set_action_flag
        ~CVideoTask.set_active
        ~CVideoTask.set_input
        ~CVideoTask.set_input_data_type
        ~CVideoTask.set_inputs
        ~CVideoTask.set_output
        ~CVideoTask.set_output_color_map
        ~CVideoTask.set_output_data_type
        ~CVideoTask.set_outputs
        ~CVideoTask.set_parameters
        ~CVideoTask.stop
        ~CVideoTask.update_static_outputs
   
    .. rubric:: Attributes
    .. autosummary::
        ~CVideoTask.name
        ~CVideoTask.type
   
    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_input
    .. automethod:: add_output
    .. automethod:: apply_graphics_mask
    .. automethod:: apply_graphics_mask_to_binary
    .. automethod:: begin_task_run
    .. automethod:: create_graphics_mask
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: end_task_run
    .. automethod:: execute_actions
    .. automethod:: forward_input_image
    .. automethod:: get_elapsed_time
    .. automethod:: get_graphics_mask
    .. automethod:: get_input
    .. automethod:: get_input_count
    .. automethod:: get_input_data_type
    .. automethod:: get_inputs
    .. automethod:: get_output
    .. automethod:: get_output_count
    .. automethod:: get_output_data_type
    .. automethod:: get_outputs
    .. automethod:: get_parameters
    .. automethod:: get_progress_steps
    .. automethod:: global_input_changed
    .. automethod:: graphics_changed
    .. automethod:: is_graphics_changed_listening
    .. automethod:: is_mask_available
    .. automethod:: notify_video_end
    .. automethod:: notify_video_start
    .. automethod:: parameters_modified
    .. automethod:: remove_input
    .. automethod:: run
    .. automethod:: set_action_flag
    .. automethod:: set_active
    .. automethod:: set_input
    .. automethod:: set_input_data_type
    .. automethod:: set_inputs
    .. automethod:: set_output
    .. automethod:: set_output_color_map
    .. automethod:: set_output_data_type
    .. automethod:: set_outputs
    .. automethod:: set_parameters
    .. automethod:: stop
    .. automethod:: update_static_outputs
