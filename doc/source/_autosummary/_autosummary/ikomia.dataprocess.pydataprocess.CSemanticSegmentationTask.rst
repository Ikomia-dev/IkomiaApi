CSemanticSegmentationTask
=========================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CSemanticSegmentationTask

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CSemanticSegmentationTask


    .. rubric:: Methods
    .. autosummary::
        ~CSemanticSegmentationTask.__init__
        ~CSemanticSegmentationTask.get_names
        ~CSemanticSegmentationTask.get_results
        ~CSemanticSegmentationTask.get_image_with_mask
        ~CSemanticSegmentationTask.read_class_names
        ~CSemanticSegmentationTask.set_colors
        ~CSemanticSegmentationTask.set_names
        ~CSemanticSegmentationTask.set_mask
        ~CSemanticSegmentationTask.emit_add_sub_progress_steps
        ~CSemanticSegmentationTask.emit_graphics_context_changed
        ~CSemanticSegmentationTask.emit_output_changed
        ~CSemanticSegmentationTask.emit_step_progress

    .. rubric:: Overridden methods
    .. autosummary::
        ~CSemanticSegmentationTask.end_task_run

    .. rubric:: Inherited methods
    .. autosummary::
        ~CSemanticSegmentationTask.add_input
        ~CSemanticSegmentationTask.add_output
        ~CSemanticSegmentationTask.apply_graphics_mask
        ~CSemanticSegmentationTask.apply_graphics_mask_to_binary
        ~CSemanticSegmentationTask.begin_task_run
        ~CSemanticSegmentationTask.create_input_graphics_mask
        ~CSemanticSegmentationTask.create_graphics_mask
        ~CSemanticSegmentationTask.execute_actions
        ~CSemanticSegmentationTask.forward_input_image
        ~CSemanticSegmentationTask.get_elapsed_time
        ~CSemanticSegmentationTask.get_graphics_mask
        ~CSemanticSegmentationTask.get_input
        ~CSemanticSegmentationTask.get_input_count
        ~CSemanticSegmentationTask.get_input_data_type
        ~CSemanticSegmentationTask.get_inputs
        ~CSemanticSegmentationTask.get_output
        ~CSemanticSegmentationTask.get_output_count
        ~CSemanticSegmentationTask.get_output_data_type
        ~CSemanticSegmentationTask.get_outputs
        ~CSemanticSegmentationTask.get_parameters
        ~CSemanticSegmentationTask.get_progress_steps
        ~CSemanticSegmentationTask.global_input_changed
        ~CSemanticSegmentationTask.graphics_changed
        ~CSemanticSegmentationTask.is_graphics_changed_listening
        ~CSemanticSegmentationTask.is_mask_available
        ~CSemanticSegmentationTask.parameters_modified
        ~CSemanticSegmentationTask.remove_input
        ~CSemanticSegmentationTask.run
        ~CSemanticSegmentationTask.set_action_flag
        ~CSemanticSegmentationTask.set_active
        ~CSemanticSegmentationTask.set_input
        ~CSemanticSegmentationTask.set_input_data_type
        ~CSemanticSegmentationTask.set_inputs
        ~CSemanticSegmentationTask.set_output
        ~CSemanticSegmentationTask.set_output_color_map
        ~CSemanticSegmentationTask.set_output_data_type
        ~CSemanticSegmentationTask.set_outputs
        ~CSemanticSegmentationTask.set_parameters
        ~CSemanticSegmentationTask.stop
        ~CSemanticSegmentationTask.update_static_outputs

    .. rubric:: Attributes
    .. autosummary::
        ~CSemanticSegmentationTask.name
        ~CSemanticSegmentationTask.type

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: get_names
    .. automethod:: get_results
    .. automethod:: get_image_with_mask
    .. automethod:: read_class_names
    .. automethod:: set_colors
    .. automethod:: set_names
    .. automethod:: set_mask
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: end_task_run
