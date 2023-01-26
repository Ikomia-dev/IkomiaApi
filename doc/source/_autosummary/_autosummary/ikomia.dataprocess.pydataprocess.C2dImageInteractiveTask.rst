C2dImageInteractiveTask
=======================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: C2dImageInteractiveTask

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import C2dImageInteractiveTask

   .. rubric:: Methods
   .. autosummary::
      ~C2dImageInteractiveTask.__init__
      ~C2dImageInteractiveTask.clear_interaction_layer
      ~C2dImageInteractiveTask.compute_blobs
      ~C2dImageInteractiveTask.create_interaction_mask
      ~C2dImageInteractiveTask.emit_add_sub_progress_steps
      ~C2dImageInteractiveTask.emit_graphics_context_changed
      ~C2dImageInteractiveTask.emit_output_changed
      ~C2dImageInteractiveTask.emit_step_progress
      ~C2dImageInteractiveTask.get_blobs

   .. rubric:: Overridden methods
   .. autosummary::
      ~C2dImageInteractiveTask.global_input_changed
      ~C2dImageInteractiveTask.graphics_changed
      ~C2dImageInteractiveTask.is_graphics_changed_listening
      ~C2dImageInteractiveTask.set_active

   .. rubric:: Inherited methods
   .. autosummary::
      ~C2dImageInteractiveTask.add_input
      ~C2dImageInteractiveTask.add_output
      ~C2dImageInteractiveTask.apply_graphics_mask
      ~C2dImageInteractiveTask.apply_graphics_mask_to_binary
      ~C2dImageInteractiveTask.begin_task_run
      ~C2dImageInteractiveTask.create_graphics_mask
      ~C2dImageInteractiveTask.end_task_run
      ~C2dImageInteractiveTask.execute_actions
      ~C2dImageInteractiveTask.forward_input_image
      ~C2dImageInteractiveTask.get_elapsed_time
      ~C2dImageInteractiveTask.get_graphics_mask
      ~C2dImageInteractiveTask.get_input
      ~C2dImageInteractiveTask.get_input_count
      ~C2dImageInteractiveTask.get_input_data_type
      ~C2dImageInteractiveTask.get_inputs
      ~C2dImageInteractiveTask.get_interaction_mask
      ~C2dImageInteractiveTask.get_output
      ~C2dImageInteractiveTask.get_output_count
      ~C2dImageInteractiveTask.get_output_data_type
      ~C2dImageInteractiveTask.get_outputs
      ~C2dImageInteractiveTask.get_parameters
      ~C2dImageInteractiveTask.get_progress_steps
      ~C2dImageInteractiveTask.is_mask_available
      ~C2dImageInteractiveTask.parameters_modified
      ~C2dImageInteractiveTask.remove_input
      ~C2dImageInteractiveTask.run
      ~C2dImageInteractiveTask.set_action_flag
      ~C2dImageInteractiveTask.set_input
      ~C2dImageInteractiveTask.set_input_data_type
      ~C2dImageInteractiveTask.set_inputs
      ~C2dImageInteractiveTask.set_output
      ~C2dImageInteractiveTask.set_output_color_map
      ~C2dImageInteractiveTask.set_output_data_type
      ~C2dImageInteractiveTask.set_outputs
      ~C2dImageInteractiveTask.set_parameters
      ~C2dImageInteractiveTask.stop
      ~C2dImageInteractiveTask.update_static_outputs
   
   .. rubric:: Attributes
   .. autosummary::
      ~C2dImageInteractiveTask.name
      ~C2dImageInteractiveTask.type
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: add_input
   .. automethod:: add_output
   .. automethod:: apply_graphics_mask
   .. automethod:: apply_graphics_mask_to_binary
   .. automethod:: begin_task_run
   .. automethod:: clear_interaction_layer
   .. automethod:: compute_blobs
   .. automethod:: create_graphics_mask
   .. automethod:: create_interaction_mask
   .. automethod:: emit_add_sub_progress_steps
   .. automethod:: emit_graphics_context_changed
   .. automethod:: emit_output_changed
   .. automethod:: emit_step_progress
   .. automethod:: end_task_run
   .. automethod:: execute_actions
   .. automethod:: forward_input_image
   .. automethod:: get_blobs
   .. automethod:: get_elapsed_time
   .. automethod:: get_graphics_mask
   .. automethod:: get_input
   .. automethod:: get_input_count
   .. automethod:: get_input_data_type
   .. automethod:: get_inputs
   .. automethod:: get_interaction_mask
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
