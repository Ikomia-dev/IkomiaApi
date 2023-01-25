C2dImageTask
============

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: C2dImageTask

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import C2dImageTask


   .. rubric:: Methods
   .. autosummary::
      ~C2dImageTask.__init__
      ~C2dImageTask.apply_graphics_mask
      ~C2dImageTask.apply_graphics_mask_to_binary
      ~C2dImageTask.create_input_graphics_mask
      ~C2dImageTask.create_graphics_mask
      ~C2dImageTask.emit_add_sub_progress_steps
      ~C2dImageTask.emit_graphics_context_changed
      ~C2dImageTask.emit_output_changed
      ~C2dImageTask.emit_step_progress
      ~C2dImageTask.forward_input_image
      ~C2dImageTask.get_graphics_mask
      ~C2dImageTask.is_mask_available
      ~C2dImageTask.set_output_color_map

   .. rubric:: Overridden methods
   .. autosummary::
      ~C2dImageTask.begin_task_run
      ~C2dImageTask.end_task_run
      ~C2dImageTask.graphics_changed
      ~C2dImageTask.set_active
      ~C2dImageTask.update_static_outputs

   .. rubric:: Inherited methods
   .. autosummary::
      ~C2dImageTask.add_input
      ~C2dImageTask.add_output
      ~C2dImageTask.execute_actions
      ~C2dImageTask.get_elapsed_time
      ~C2dImageTask.get_input
      ~C2dImageTask.get_input_count
      ~C2dImageTask.get_input_data_type
      ~C2dImageTask.get_inputs
      ~C2dImageTask.get_output
      ~C2dImageTask.get_output_count
      ~C2dImageTask.get_output_data_type
      ~C2dImageTask.get_outputs
      ~C2dImageTask.get_parameters
      ~C2dImageTask.get_progress_steps
      ~C2dImageTask.global_input_changed
      ~C2dImageTask.is_graphics_changed_listening
      ~C2dImageTask.parameters_modified
      ~C2dImageTask.remove_input
      ~C2dImageTask.run
      ~C2dImageTask.set_action_flag
      ~C2dImageTask.set_input
      ~C2dImageTask.set_input_data_type
      ~C2dImageTask.set_inputs
      ~C2dImageTask.set_output
      ~C2dImageTask.set_output_data_type
      ~C2dImageTask.set_outputs
      ~C2dImageTask.set_parameters
      ~C2dImageTask.stop      
   
   .. rubric:: Attributes
   .. autosummary::
      ~C2dImageTask.name
      ~C2dImageTask.type
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: add_input
   .. automethod:: add_output
   .. automethod:: apply_graphics_mask
   .. automethod:: apply_graphics_mask_to_binary
   .. automethod:: begin_task_run
   .. automethod:: create_input_graphics_mask
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
   