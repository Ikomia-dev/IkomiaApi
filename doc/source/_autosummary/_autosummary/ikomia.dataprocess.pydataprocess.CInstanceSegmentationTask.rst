CInstanceSegmentationTask
=========================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CInstanceSegmentationTask

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CInstanceSegmentationTask


    .. rubric:: Methods
    .. autosummary::
        ~CInstanceSegmentationTask.__init__
        ~CInstanceSegmentationTask.add_object
        ~CInstanceSegmentationTask.get_names
        ~CInstanceSegmentationTask.get_results
        ~CInstanceSegmentationTask.get_image_with_graphics
        ~CInstanceSegmentationTask.get_image_with_mask
        ~CInstanceSegmentationTask.get_image_with_mask_and_graphics
        ~CInstanceSegmentationTask.read_class_names
        ~CInstanceSegmentationTask.set_colors
        ~CInstanceSegmentationTask.set_names
        ~CInstanceSegmentationTask.emit_add_sub_progress_steps
        ~CInstanceSegmentationTask.emit_graphics_context_changed
        ~CInstanceSegmentationTask.emit_output_changed
        ~CInstanceSegmentationTask.emit_step_progress

    .. rubric:: Overridden methods
    .. autosummary::
        ~CInstanceSegmentationTask.end_task_run

    .. rubric:: Inherited methods
    .. autosummary::
        ~CInstanceSegmentationTask.add_input
        ~CInstanceSegmentationTask.add_output
        ~CInstanceSegmentationTask.apply_graphics_mask
        ~CInstanceSegmentationTask.apply_graphics_mask_to_binary
        ~CInstanceSegmentationTask.begin_task_run
        ~CInstanceSegmentationTask.create_input_graphics_mask
        ~CInstanceSegmentationTask.create_graphics_mask
        ~CInstanceSegmentationTask.execute_actions
        ~CInstanceSegmentationTask.forward_input_image
        ~CInstanceSegmentationTask.get_elapsed_time
        ~CInstanceSegmentationTask.get_graphics_mask
        ~CInstanceSegmentationTask.get_input
        ~CInstanceSegmentationTask.get_input_count
        ~CInstanceSegmentationTask.get_input_data_type
        ~CInstanceSegmentationTask.get_inputs
        ~CInstanceSegmentationTask.get_output
        ~CInstanceSegmentationTask.get_output_count
        ~CInstanceSegmentationTask.get_output_data_type
        ~CInstanceSegmentationTask.get_outputs
        ~CInstanceSegmentationTask.get_parameters
        ~CInstanceSegmentationTask.get_progress_steps
        ~CInstanceSegmentationTask.global_input_changed
        ~CInstanceSegmentationTask.graphics_changed
        ~CInstanceSegmentationTask.is_graphics_changed_listening
        ~CInstanceSegmentationTask.is_mask_available
        ~CInstanceSegmentationTask.parameters_modified
        ~CInstanceSegmentationTask.remove_input
        ~CInstanceSegmentationTask.run
        ~CInstanceSegmentationTask.set_action_flag
        ~CInstanceSegmentationTask.set_active
        ~CInstanceSegmentationTask.set_input
        ~CInstanceSegmentationTask.set_input_data_type
        ~CInstanceSegmentationTask.set_inputs
        ~CInstanceSegmentationTask.set_output
        ~CInstanceSegmentationTask.set_output_color_map
        ~CInstanceSegmentationTask.set_output_data_type
        ~CInstanceSegmentationTask.set_outputs
        ~CInstanceSegmentationTask.set_parameters
        ~CInstanceSegmentationTask.stop
        ~CInstanceSegmentationTask.update_static_outputs

    .. rubric:: Attributes
    .. autosummary::
        ~CInstanceSegmentationTask.name
        ~CInstanceSegmentationTask.type

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_object
    .. automethod:: get_names
    .. automethod:: get_results
    .. automethod:: get_image_with_graphics
    .. automethod:: get_image_with_mask
    .. automethod:: get_image_with_mask_and_graphics
    .. automethod:: read_class_names
    .. automethod:: set_colors
    .. automethod:: set_names
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: end_task_run
