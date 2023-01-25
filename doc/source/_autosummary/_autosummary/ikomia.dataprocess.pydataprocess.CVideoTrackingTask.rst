CVideoTrackingTask
==================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CVideoTrackingTask

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import CVideoTrackingTask

   .. rubric:: Methods
   .. autosummary::
      ~CVideoTrackingTask.__init__
      ~CVideoTrackingTask.manage_outputs
      ~CVideoTrackingTask.set_roi_to_track

   .. rubric:: Overridden methods
   .. autosummary::
      ~CVideoTrackingTask.notify_video_end
      ~CVideoTrackingTask.notify_video_start

   .. rubric:: Inherited methods
   .. autosummary::      
      ~CVideoTrackingTask.add_input
      ~CVideoTrackingTask.add_output
      ~CVideoTrackingTask.apply_graphics_mask
      ~CVideoTrackingTask.apply_graphics_mask_to_binary
      ~CVideoTrackingTask.begin_task_run
      ~CVideoTrackingTask.create_graphics_mask
      ~CVideoTrackingTask.emit_add_sub_progress_steps
      ~CVideoTrackingTask.emit_graphics_context_changed
      ~CVideoTrackingTask.emit_output_changed
      ~CVideoTrackingTask.emit_step_progress
      ~CVideoTrackingTask.end_task_run
      ~CVideoTrackingTask.execute_actions
      ~CVideoTrackingTask.forward_input_image
      ~CVideoTrackingTask.get_elapsed_time
      ~CVideoTrackingTask.get_graphics_mask
      ~CVideoTrackingTask.get_input
      ~CVideoTrackingTask.get_input_count
      ~CVideoTrackingTask.get_input_data_type
      ~CVideoTrackingTask.get_inputs
      ~CVideoTrackingTask.get_output
      ~CVideoTrackingTask.get_output_count
      ~CVideoTrackingTask.get_output_data_type
      ~CVideoTrackingTask.get_outputs
      ~CVideoTrackingTask.get_parameters
      ~CVideoTrackingTask.get_progress_steps
      ~CVideoTrackingTask.global_input_changed
      ~CVideoTrackingTask.graphics_changed
      ~CVideoTrackingTask.is_graphics_changed_listening
      ~CVideoTrackingTask.is_mask_available
      ~CVideoTrackingTask.parameters_modified
      ~CVideoTrackingTask.remove_input
      ~CVideoTrackingTask.run
      ~CVideoTrackingTask.set_action_flag
      ~CVideoTrackingTask.set_active
      ~CVideoTrackingTask.set_input
      ~CVideoTrackingTask.set_input_data_type
      ~CVideoTrackingTask.set_inputs
      ~CVideoTrackingTask.set_output
      ~CVideoTrackingTask.set_output_color_map
      ~CVideoTrackingTask.set_output_data_type
      ~CVideoTrackingTask.set_outputs
      ~CVideoTrackingTask.set_parameters
      ~CVideoTrackingTask.stop
      ~CVideoTrackingTask.update_static_outputs

   .. rubric:: Attributes
   .. autosummary::
      ~CVideoTrackingTask.name
      ~CVideoTrackingTask.type
   
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
   .. automethod:: manage_outputs
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
   .. automethod:: set_roi_to_track
   .. automethod:: stop
   .. automethod:: update_static_outputs
