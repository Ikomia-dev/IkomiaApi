TrainProcess
============

.. currentmodule:: ikomia.dnn.dnntrain

.. autoclass:: TrainProcess

    .. rubric:: Methods
    .. autosummary::

        ~TrainProcess.__init__
        ~TrainProcess.begin_task_run
        ~TrainProcess.end_task_run
        ~TrainProcess.log_param
        ~TrainProcess.log_params
        ~TrainProcess.log_metric
        ~TrainProcess.log_metrics
        ~TrainProcess.log_artifact
        ~TrainProcess.log_artifacts
        ~TrainProcess.stop

    .. rubric:: Inherited methods
    .. autosummary::

        ~TrainProcess.add_input
        ~TrainProcess.add_output
        ~TrainProcess.emit_add_sub_progress_steps
        ~TrainProcess.emit_graphics_context_changed
        ~TrainProcess.emit_output_changed
        ~TrainProcess.emit_step_progress
        ~TrainProcess.enable_mlflow
        ~TrainProcess.enable_tensorboard
        ~TrainProcess.execute_actions
        ~TrainProcess.get_elapsed_time
        ~TrainProcess.get_input
        ~TrainProcess.get_input_count
        ~TrainProcess.get_input_data_type
        ~TrainProcess.get_inputs
        ~TrainProcess.get_output
        ~TrainProcess.get_output_count
        ~TrainProcess.get_output_data_type
        ~TrainProcess.get_outputs
        ~TrainProcess.get_parameters
        ~TrainProcess.get_progress_steps
        ~TrainProcess.global_input_changed
        ~TrainProcess.graphics_changed
        ~TrainProcess.is_graphics_changed_listening
        ~TrainProcess.parameters_modified
        ~TrainProcess.remove_input
        ~TrainProcess.run
        ~TrainProcess.set_action_flag
        ~TrainProcess.set_active
        ~TrainProcess.set_input
        ~TrainProcess.set_input_data_type
        ~TrainProcess.set_inputs
        ~TrainProcess.set_output
        ~TrainProcess.set_output_data_type
        ~TrainProcess.set_outputs
        ~TrainProcess.set_parameters
        ~TrainProcess.update_static_outputs

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: stop
    .. automethod:: add_input
    .. automethod:: add_output
    .. automethod:: begin_task_run
    .. automethod:: emit_add_sub_progress_steps
    .. automethod:: emit_graphics_context_changed
    .. automethod:: emit_output_changed
    .. automethod:: emit_step_progress
    .. automethod:: enable_mlflow
    .. automethod:: enable_tensorboard
    .. automethod:: end_task_run
    .. automethod:: execute_actions
    .. automethod:: get_elapsed_time
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
    .. automethod:: log_param
    .. automethod:: log_params
    .. automethod:: log_metric
    .. automethod:: log_metrics
    .. automethod:: log_artifact
    .. automethod:: log_artifacts
    .. automethod:: parameters_modified
    .. automethod:: remove_input
    .. automethod:: run
    .. automethod:: set_action_flag
    .. automethod:: set_active
    .. automethod:: set_input
    .. automethod:: set_input_data_type
    .. automethod:: set_inputs
    .. automethod:: set_output
    .. automethod:: set_output_data_type
    .. automethod:: set_outputs
    .. automethod:: set_parameters
    .. automethod:: update_static_outputs

    .. rubric:: Attributes
    .. autosummary::
   
        ~TrainProcess.name
        ~TrainProcess.type
