import os.path
import logging
import argparse
from ikomia.utils import tests, ik
from ikomia import core, dataio, dataprocess
import cv2
import numpy as np

logger = logging.getLogger(__name__)


def _create_graphics_list():
    graphics = []
    pt = core.CGraphicsPoint(core.CPointF(150, 80))
    graphics.append(pt)

    ellipse = core.CGraphicsEllipse(0, 0, 100, 50)
    graphics.append(ellipse)

    rect = core.CGraphicsRectangle(0, 60, 100, 50)
    graphics.append(rect)

    polyline = core.CGraphicsPolyline([core.CPointF(0, 0),
                                       core.CPointF(30, 30),
                                       core.CPointF(100, 30),
                                       core.CPointF(130, 100)])
    graphics.append(polyline)

    polygon = core.CGraphicsPolygon([core.CPointF(50, 10),
                                     core.CPointF(100, 100),
                                     core.CPointF(80, 150),
                                     core.CPointF(20, 120),
                                     core.CPointF(20, 60)])
    graphics.append(polygon)

    return graphics


def test_cpp_base_class():
    logger.warning("===== Test: CWorkflowTask =====")
    # Default ctor
    t = core.CWorkflowTask()
    assert t.type == core.TaskType.GENERIC

    # Ctor with name
    name = "MyTask"
    t = core.CWorkflowTask(name)
    assert t.name == name

    t.set_auto_save(True)

    # Inputs
    t.set_input(core.CWorkflowTaskIO(core.IODataType.IMAGE), 0)
    t.set_input_data_type(core.IODataType.IMAGE_BINARY, 0)
    assert t.get_input_data_type(0) == core.IODataType.IMAGE_BINARY
    inputs = [core.CWorkflowTaskIO(core.IODataType.IMAGE), core.CWorkflowTaskIO(core.IODataType.VIDEO)]
    t.set_inputs(inputs)
    assert t.get_input_count() == 2
    inputs = t.get_inputs()
    assert len(inputs) == 2
    task_in = t.get_input(1)
    assert type(task_in) == core.CWorkflowTaskIO
    assert t.get_input_data_type(0) == core.IODataType.IMAGE
    t.add_input(core.CWorkflowTaskIO(core.IODataType.IMAGE))
    assert t.get_input_count() == 3
    t.remove_input(1)
    assert t.get_input_count() == 2

    # Outputs
    t.set_output(core.CWorkflowTaskIO(core.IODataType.IMAGE), 0)
    t.set_output_data_type(core.IODataType.IMAGE_BINARY, 0)
    assert t.get_output_data_type(0) == core.IODataType.IMAGE_BINARY
    outputs = [core.CWorkflowTaskIO(core.IODataType.IMAGE), core.CWorkflowTaskIO(core.IODataType.VIDEO)]
    t.set_outputs(outputs)
    t.output_folder = os.path.curdir
    assert t.get_output_count() == 2
    outputs = t.get_outputs()
    assert len(outputs) == 2
    task_out = t.get_output(1)
    assert type(task_out) == core.CWorkflowTaskIO
    assert t.get_output_data_type(0) == core.IODataType.IMAGE
    t.add_output(core.CWorkflowTaskIO(core.IODataType.IMAGE))
    assert t.get_output_count() == 3
    t.remove_output(1)
    assert t.get_output_count() == 2
    t.update_static_outputs()

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.run()
    t.stop()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Others
    t.set_action_flag(core.ActionFlag.APPLY_VOLUME, True)
    t.set_active(True)
    logger.warning(f"Graphics change listining: {t.is_graphics_changed_listening()}")

    # Print
    logger.warning(t)


def test_cpp_2d_image_task():
    logger.warning("===== Test: C2dImageTask =====")
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()
    h, w, c = image_data.shape

    # Default ctor
    t = dataprocess.C2dImageTask()
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Ctor with graphics flag
    t = dataprocess.C2dImageTask(False)
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.get_input_count() == 1
    assert t.get_output_count() == 1

    # Ctor with name
    name = "Blur"
    t = dataprocess.C2dImageTask(name)
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Ctor with name and graphics flag
    t = dataprocess.C2dImageTask(name, True)
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Graphics mask from graphics input
    items = _create_graphics_list()
    graphics_in = t.get_input(1)
    graphics_in.set_items(items)
    mask = t.create_input_graphics_mask(1, w, h)
    assert mask is not None
    assert mask.shape == (h, w)

    # Graphics mask from graphics items
    t.create_graphics_mask(w, h, graphics_in)
    assert t.is_mask_available(0)
    mask = t.get_graphics_mask(0)
    assert mask is not None
    assert mask.shape == (h, w)

    # Apply graphics mask
    img = image_data.copy()
    processed = cv2.stackBlur(img, (19, 19))
    res = t.apply_graphics_mask(img, processed, 0)
    assert res is not None
    img_mono = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    processed_mono = cv2.cvtColor(processed, cv2.COLOR_RGB2GRAY)
    res = t.apply_graphics_mask_to_binary(img_mono, processed_mono, 0)
    assert res is not None

    # Forward input image
    image_in = t.get_input(0)
    image_in.set_image(image_data)
    t.forward_input_image(0, 0)
    image_out = t.get_output(0)
    assert image_out.get_image() is not None

    # Output colormap
    t.set_output_color_map(0, 0, [], True)

    # Method to override
    t.graphics_changed()
    t.global_input_changed(False)
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_graphics_context_changed()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)

    # Print
    logger.warning(t)


def test_cpp_2d_image_interactive_task():
    logger.warning("===== Test: C2dImageInteractiveTask =====")

    # Default ctor
    t = dataprocess.C2dImageInteractiveTask()
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Ctor with name
    name = "Blur"
    t = dataprocess.C2dImageInteractiveTask(name)
    assert t.type == core.TaskType.IMAGE_PROCESS_2D
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Interaction layer
    t.set_active(True)
    t.create_interaction_mask(512, 512)
    mask = t.get_interaction_mask()
    assert mask is not None
    t.compute_blobs()
    objects = t.get_blobs()
    assert len(objects) == 0
    t.clear_interaction_layer()

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Method to override
    t.graphics_changed()
    t.global_input_changed(False)
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_graphics_context_changed()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_cpp_video_task():
    logger.warning("===== Test: CVideoTask =====")

    # Default ctor
    t = dataprocess.CVideoTask()
    assert t.type == core.TaskType.VIDEO
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Ctor with name
    name = "Blur"
    t = dataprocess.CVideoTask(name)
    assert t.type == core.TaskType.VIDEO
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Method to override
    t.notify_video_start(100)
    t.notify_video_end()
    t.graphics_changed()
    t.global_input_changed(False)
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_graphics_context_changed()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_cpp_video_of_task():
    logger.warning("===== Test: CVideoOFTask =====")

    # Default ctor
    t = dataprocess.CVideoOFTask()
    assert t.type == core.TaskType.VIDEO
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Ctor with name
    name = "Optical Flow"
    t = dataprocess.CVideoOFTask(name)
    assert t.type == core.TaskType.VIDEO
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 1

    # Visualization methods
    flow = np.ones((512, 512, 2), dtype=np.single)
    flow_map = t.flow_to_display(flow)
    assert flow_map is not None
    flow_map_with_vectors = t.draw_opt_flow_map(flow, flow_map, 1)
    assert flow_map_with_vectors is not None

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Method to override
    t.notify_video_start(100)
    t.notify_video_end()
    t.graphics_changed()
    t.global_input_changed(False)
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_graphics_context_changed()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_cpp_video_tracking_task():
    logger.warning("===== Test: CVideoTrackingTask =====")
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()
    h, w, c = image_data.shape

    # Default ctor
    t = dataprocess.CVideoTrackingTask()
    assert t.type == core.TaskType.VIDEO
    assert t.get_input_count() == 2
    assert t.get_output_count() == 4

    # Ctor with name
    name = "Object tracking"
    t = dataprocess.CVideoTrackingTask(name)
    assert t.type == core.TaskType.VIDEO
    assert t.name == name
    assert t.get_input_count() == 2
    assert t.get_output_count() == 4

    # Run
    image_in = t.get_input(0)
    image_in.set_image(image_data)
    graphics_in = t.get_input(1)
    items = []
    rect = core.CGraphicsRectangle(0, 60, 100, 50)
    items.append(rect)
    graphics_in.set_items(items)
    t.set_roi_to_track()
    t.notify_video_start(100)
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")
    t.manage_outputs()

    # Method to override
    t.notify_video_end()
    t.graphics_changed()
    t.global_input_changed(False)
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_graphics_context_changed()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_cpp_dnn_train_task():
    logger.warning("===== Test: CDnnTrainTask =====")

    # Default ctor
    t = dataprocess.CDnnTrainTask()
    assert t.type == core.TaskType.DNN_TRAIN
    assert t.get_input_count() == 0
    assert t.get_output_count() == 0

    # Ctor with name
    name = "DNN"
    t = dataprocess.CDnnTrainTask(name)
    assert t.type == core.TaskType.DNN_TRAIN
    assert t.name == name
    assert t.get_input_count() == 0
    assert t.get_output_count() == 0

    # Ctor with name and parameter object
    param = core.CWorkflowTaskParam()
    t = dataprocess.CDnnTrainTask(name, param)
    assert t.type == core.TaskType.DNN_TRAIN
    assert t.name == name
    assert t.get_input_count() == 0
    assert t.get_output_count() == 0

    # Enable DNN logging
    t.enable_mlflow(True)
    t.enable_tensorboard(True)

    # Run
    logger.warning(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.warning(f"Elapsed time: {t.get_elapsed_time()}")

    # Method to override
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_task_parameters():
    logger.warning("===== Test: set task parameters =====")
    algo = core.task.create(name=ik.ocv_box_filter().name)
    params = algo.get_parameters()
    assert len(params) > 0

    logger.warning("----- Manually set some of the parameters from existing values")
    new_params = {
        ik.ocv_box_filter.kSizeWidth: "31",
        ik.ocv_box_filter.kSizeHeight: "33",
    }
    algo.set_parameters(new_params)
    logger.warning(algo.get_param_object())
    params = algo.get_parameters()
    assert len(params) > 0
    assert params[ik.ocv_box_filter.kSizeWidth] == "31"
    assert params[ik.ocv_box_filter.kSizeHeight] == "33"

    logger.warning("----- Manually set all parameters from dict")
    new_params = {
        ik.ocv_box_filter.kSizeWidth: "29",
        ik.ocv_box_filter.kSizeHeight: "29",
        ik.ocv_box_filter.borderType: "4",
        ik.ocv_box_filter.anchorX: "-1",
        ik.ocv_box_filter.anchorY: "-1",
        ik.ocv_box_filter.bNormalize: "1",
        ik.ocv_box_filter.ddepth: "-1"
    }
    algo.set_parameters(new_params)
    logger.warning(algo.get_param_object())
    params = algo.get_parameters()
    assert len(params) > 0
    assert params[ik.ocv_box_filter.kSizeWidth] == "29"
    assert params[ik.ocv_box_filter.kSizeHeight] == "29"
    assert params[ik.ocv_box_filter.borderType] == "4"
    assert params[ik.ocv_box_filter.anchorX] == "-1"
    assert params[ik.ocv_box_filter.anchorY] == "-1"
    assert params[ik.ocv_box_filter.bNormalize] == "1"
    assert params[ik.ocv_box_filter.ddepth] == "-1"


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--wgisd_dir",
                        type=str,
                        default="/home/ludo/Images/Datasets/wgisd",
                        help="Classification datatset folder")
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'cpp_base_class' in running_tests:
        test_cpp_base_class()
    if 'all' in running_tests or 'cpp_2d_image_task' in running_tests:
        test_cpp_2d_image_task()
    if 'all' in running_tests or 'cpp_2d_image_interactive_task' in running_tests:
        test_cpp_2d_image_interactive_task()
    if 'all' in running_tests or 'cpp_video_task' in running_tests:
        test_cpp_video_task()
    if 'all' in running_tests or 'cpp_video_of_task' in running_tests:
        test_cpp_video_of_task()
    if 'all' in running_tests or 'cpp_video_tracking_task' in running_tests:
        test_cpp_video_tracking_task()
    if 'all' in running_tests or 'cpp_dnn_train_task' in running_tests:
        test_cpp_dnn_train_task()
    if 'all' in running_tests or 'parameters' in running_tests:
        test_task_parameters()
