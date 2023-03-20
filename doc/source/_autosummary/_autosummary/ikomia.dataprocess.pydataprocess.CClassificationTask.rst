CClassificationTask
===================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CClassificationTask

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CClassificationTask


    .. rubric:: Methods
    .. autosummary::
        ~CClassificationTask.__init__
        ~CClassificationTask.add_object
        ~CClassificationTask.get_input_objects
        ~CClassificationTask.get_names
        ~CClassificationTask.get_objects_results
        ~CClassificationTask.get_object_sub_image
        ~CClassificationTask.get_image_with_graphics
        ~CClassificationTask.get_whole_image_results
        ~CClassificationTask.is_whole_image_classification
        ~CClassificationTask.read_class_names
        ~CClassificationTask.set_colors
        ~CClassificationTask.set_names
        ~CClassificationTask.set_whole_image_results
        ~CClassificationTask.emit_add_sub_progress_steps
        ~CClassificationTask.emit_graphics_context_changed
        ~CClassificationTask.emit_output_changed
        ~CClassificationTask.emit_step_progress

    .. rubric:: Overridden methods
    .. autosummary::
        ~CClassificationTask.end_task_run

    .. rubric:: Inherited methods
    .. autosummary::
        ~CClassificationTask.add_input
        ~CClassificationTask.add_output
        ~CClassificationTask.apply_graphics_mask
        ~CClassificationTask.apply_graphics_mask_to_binary
        ~CClassificationTask.begin_task_run
        ~CClassificationTask.create_input_graphics_mask
        ~CClassificationTask.create_graphics_mask
        ~CClassificationTask.execute_actions
        ~CClassificationTask.forward_input_image
        ~CClassificationTask.get_elapsed_time
        ~CClassificationTask.get_graphics_mask
        ~CClassificationTask.get_input
        ~CClassificationTask.get_input_count
        ~CClassificationTask.get_input_data_type
        ~CClassificationTask.get_inputs
        ~CClassificationTask.get_output
        ~CClassificationTask.get_output_count
        ~CClassificationTask.get_output_data_type
        ~CClassificationTask.get_outputs
        ~CClassificationTask.get_parameters
        ~CClassificationTask.get_progress_steps
        ~CClassificationTask.global_input_changed
        ~CClassificationTask.graphics_changed
        ~CClassificationTask.is_graphics_changed_listening
        ~CClassificationTask.is_mask_available
        ~CClassificationTask.parameters_modified
        ~CClassificationTask.remove_input
        ~CClassificationTask.run
        ~CClassificationTask.set_action_flag
        ~CClassificationTask.set_active
        ~CClassificationTask.set_input
        ~CClassificationTask.set_input_data_type
        ~CClassificationTask.set_inputs
        ~CClassificationTask.set_output
        ~CClassificationTask.set_output_color_map
        ~CClassificationTask.set_output_data_type
        ~CClassificationTask.set_outputs
        ~CClassificationTask.set_parameters
        ~CClassificationTask.stop
        ~CClassificationTask.update_static_outputs

    .. rubric:: Attributes
    .. autosummary::
        ~CClassificationTask.name
        ~CClassificationTask.type

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_object
    .. automethod:: get_input_objects
    .. automethod:: get_names
    .. automethod:: get_objects_results
    .. automethod:: get_object_sub_image
    .. automethod:: get_image_with_graphics
    .. automethod:: get_whole_image_results
    .. automethod:: is_whole_image_classification
    .. automethod:: read_class_names
    .. automethod:: set_colors
    .. automethod:: set_names
    .. automethod:: set_whole_image_results
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: end_task_run
