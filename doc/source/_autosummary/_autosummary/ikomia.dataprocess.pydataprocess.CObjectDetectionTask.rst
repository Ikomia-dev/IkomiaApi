CObjectDetectionTask
====================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CObjectDetectionTask

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CObjectDetectionTask


    .. rubric:: Methods
    .. autosummary::
        ~CObjectDetectionTask.__init__
        ~CObjectDetectionTask.add_object
        ~CObjectDetectionTask.get_names
        ~CObjectDetectionTask.get_results
        ~CObjectDetectionTask.get_image_with_graphics
        ~CObjectDetectionTask.read_class_names
        ~CObjectDetectionTask.set_colors
        ~CObjectDetectionTask.set_names
        ~CObjectDetectionTask.emit_add_sub_progress_steps
        ~CObjectDetectionTask.emit_graphics_context_changed
        ~CObjectDetectionTask.emit_output_changed
        ~CObjectDetectionTask.emit_step_progress

    .. rubric:: Overridden methods
    .. autosummary::
        ~CObjectDetectionTask.end_task_run

    .. rubric:: Inherited methods
    .. autosummary::
        ~CObjectDetectionTask.add_input
        ~CObjectDetectionTask.add_output
        ~CObjectDetectionTask.apply_graphics_mask
        ~CObjectDetectionTask.apply_graphics_mask_to_binary
        ~CObjectDetectionTask.begin_task_run
        ~CObjectDetectionTask.create_input_graphics_mask
        ~CObjectDetectionTask.create_graphics_mask
        ~CObjectDetectionTask.execute_actions
        ~CObjectDetectionTask.forward_input_image
        ~CObjectDetectionTask.get_elapsed_time
        ~CObjectDetectionTask.get_graphics_mask
        ~CObjectDetectionTask.get_input
        ~CObjectDetectionTask.get_input_count
        ~CObjectDetectionTask.get_input_data_type
        ~CObjectDetectionTask.get_inputs
        ~CObjectDetectionTask.get_output
        ~CObjectDetectionTask.get_output_count
        ~CObjectDetectionTask.get_output_data_type
        ~CObjectDetectionTask.get_outputs
        ~CObjectDetectionTask.get_parameters
        ~CObjectDetectionTask.get_progress_steps
        ~CObjectDetectionTask.global_input_changed
        ~CObjectDetectionTask.graphics_changed
        ~CObjectDetectionTask.is_graphics_changed_listening
        ~CObjectDetectionTask.is_mask_available
        ~CObjectDetectionTask.parameters_modified
        ~CObjectDetectionTask.remove_input
        ~CObjectDetectionTask.run
        ~CObjectDetectionTask.set_action_flag
        ~CObjectDetectionTask.set_active
        ~CObjectDetectionTask.set_input
        ~CObjectDetectionTask.set_input_data_type
        ~CObjectDetectionTask.set_inputs
        ~CObjectDetectionTask.set_output
        ~CObjectDetectionTask.set_output_color_map
        ~CObjectDetectionTask.set_output_data_type
        ~CObjectDetectionTask.set_outputs
        ~CObjectDetectionTask.set_parameters
        ~CObjectDetectionTask.stop
        ~CObjectDetectionTask.update_static_outputs

    .. rubric:: Attributes
    .. autosummary::
        ~CObjectDetectionTask.name
        ~CObjectDetectionTask.type

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_object
    .. automethod:: get_names
    .. automethod:: get_results
    .. automethod:: get_image_with_graphics
    .. automethod:: read_class_names
    .. automethod:: set_colors
    .. automethod:: set_names
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: end_task_run
