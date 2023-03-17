import os.path
import logging
import argparse
from ikomia.utils import tests, ik, displayIO
from ikomia import core, dataio, dataprocess
from ikomia.dataprocess import workflow
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
    logger.info("===== Test: CWorkflowTask =====")
    # Default ctor
    t = core.CWorkflowTask()
    assert t.type == core.TaskType.GENERIC

    # Ctor with name
    name = "MyTask"
    t = core.CWorkflowTask(name)
    assert t.name == name

    t.set_auto_save(True)

    # Inputs
    t.set_input(core.CWorkflowTaskIO(core.IODataType.IMAGE), 0, True)
    t.set_input_data_type(core.IODataType.IMAGE_BINARY, 0)
    assert t.get_input_data_type(0) == core.IODataType.IMAGE_BINARY
    inputs = [core.CWorkflowTaskIO(core.IODataType.IMAGE), core.CWorkflowTaskIO(core.IODataType.VIDEO)]
    t.set_inputs(inputs, True)
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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.run()
    t.stop()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

    # Others
    t.set_action_flag(core.ActionFlag.APPLY_VOLUME, True)
    t.set_active(True)
    logger.info(f"Graphics change listining: {t.is_graphics_changed_listening()}")

    # Print
    logger.info(t)


def test_cpp_2d_image_task():
    logger.info("===== Test: C2dImageTask =====")
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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

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
    res = t.apply_graphics_mask(img, 0)
    assert res is not None
    img_mono = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    res = t.apply_graphics_mask_to_binary(img_mono, 0)
    assert res is not None

    # Forward input image
    image_in = t.get_input(0)
    image_in.set_image(image_data)
    t.forward_input_image(0, 0)
    image_out = t.get_output(0)
    assert image_out.get_image() is not None

    # Output colormap
    t.set_output_color_map(0, 0, [])

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
    logger.info(t)


def test_cpp_2d_image_interactive_task():
    logger.info("===== Test: C2dImageInteractiveTask =====")

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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

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
    logger.info("===== Test: CVideoTask =====")

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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

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
    logger.info("===== Test: CVideoOFTask =====")

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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

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
    logger.info("===== Test: CVideoTrackingTask =====")
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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")
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
    logger.info("===== Test: CDnnTrainTask =====")

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
    logger.info(f"Progress steps: {t.get_progress_steps()}")
    t.begin_task_run()
    t.run()
    t.stop()
    t.end_task_run()
    logger.info(f"Elapsed time: {t.get_elapsed_time()}")

    # Method to override
    t.get_progress_steps()
    t.emit_add_sub_progress_steps(10)
    t.emit_step_progress()
    t.emit_output_changed()
    t.execute_actions(core.ActionFlag.APPLY_VOLUME)


def test_task_parameters():
    logger.info("===== Test: set task parameters =====")
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()

    algo = core.task.create(ik.ocv_box_filter)
    print(algo)

    logger.info("----- Get parameters")
    logger.info(algo.get_parameters())

    logger.info("----- Use default parameters")
    logger.info(algo.get_param_object())
    input_img = algo.get_input(0)
    input_img.set_image(image_data)
    algo.run()
    displayIO.display(algo.get_output(0), algo.name)

    logger.info("----- Manually set some of the parameters from existing values")
    params = algo.get_parameters()
    params[ik.ocv_box_filter_param.kSizeHeight] = str(31)
    params[ik.ocv_box_filter_param.kSizeWidth] = str(31)
    algo.set_parameters(params)
    logger.info(algo.get_param_object())
    algo.run()
    displayIO.display(algo.get_output(0), algo.name)

    logger.info("----- Manually set all parameters from dict")
    algo.set_parameters({ik.ocv_box_filter_param.borderType: "4",
                         ik.ocv_box_filter_param.anchorX: "-1",
                         ik.ocv_box_filter_param.anchorY: "-1",
                         ik.ocv_box_filter_param.kSizeHeight: "29",
                         ik.ocv_box_filter_param.kSizeWidth: "29",
                         ik.ocv_box_filter_param.bNormalize: "1",
                         ik.ocv_box_filter_param.ddepth: "-1"})

    logger.info(algo.get_param_object())
    algo.run()
    displayIO.display(algo.get_output(0), algo.name)

    logger.info("----- Manually set some parameters from dict")
    algo.set_parameters({ik.ocv_box_filter_param.kSizeHeight: "19", ik.ocv_box_filter_param.kSizeWidth: "19"})
    logger.info(algo.get_param_object())
    algo.run()
    displayIO.display(algo.get_output(0), algo.name)


def test_get_task_outputs(wgisd_dir):
    logger.info("===== Test: get task outputs of various data types =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestOutput.json"
    wf = workflow.load(wf_path)

    wgisd_task = wf.find_task(ik.dataset_wgisd, 0)[1]
    wgisd_params = wgisd_task.get_parameters()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dir + "/classes.txt"
    wgisd_params[ik.dataset_wgisd_param.seg_mask_mode] = "None"
    wgisd_task.set_parameters(wgisd_params)
    wf.set_image_input(path=img_path)
    wf.run()

    logger.info("----- Get MobileNet SSD outputs: image, graphics and blob measure")
    detector_task = wf.find_task(ik.infer_mobilenet_ssd, 0)[1]
    img_out = core.task.get_image_output(detector_task, index=0)
    assert(img_out is not None)
    displayIO.display(img_out, detector_task.name)
    graphics_out = core.task.get_graphics_output(detector_task, index=0)
    assert (graphics_out is not None)
    displayIO.display(graphics_out, detector_task.name)
    blob_out = core.task.get_blob_measure_output(detector_task, index=0)
    assert (blob_out is not None)
    displayIO.display(blob_out, detector_task.name)

    logger.info("----- Get Split Operator outputs: 3 images")
    split_task = wf.find_task(ik.ocv_split, 0)[1]
    img_out = core.task.get_image_output(split_task)
    assert (img_out is not None)
    assert(len(img_out) == 3)
    img_out = core.task.get_image_output(split_task, index=2)
    displayIO.display(img_out, split_task.name + "- blue channel")

    logger.info("----- Get CalcHist outputs: numeric")
    hist_task = wf.find_task(ik.ocv_calc_hist, 0)[1]
    numeric_out = core.task.get_numeric_output(hist_task, index=0)
    assert (numeric_out is not None)
    displayIO.display(numeric_out, hist_task.name)

    logger.info("----- Get WGISD_Dataset outputs: dataset")
    dataset_out = core.task.get_dataset_output(wgisd_task, index=0)
    assert (dataset_out is not None)
    assert (dataset_out.getCategoryCount() > 0)
    assert (len(dataset_out.getImagePaths()) > 0)


def test_get_image_with_graphics():
    logger.info("===== Test: get image with graphics =====")
    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    # run
    t = core.task.create(ik.infer_mobilenet_ssd)
    t.get_input(0).setImage(img)
    t.run()
    # get image with graphics
    img_with_graphics = core.task.get_image_with_graphics(t, 0, 0)
    img_with_graphics = cv2.cvtColor(img_with_graphics, cv2.COLOR_BGR2RGB)
    cv2.imshow("Image with graphics", img_with_graphics)
    cv2.waitKey(0)


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
    if 'all' in running_tests or 'outputs' in running_tests:
        test_get_task_outputs(opt.wgisd_dir)
    if 'all' in running_tests or 'output_image_graphics' in running_tests:
        test_get_image_with_graphics()
