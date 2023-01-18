import os.path

import ikomia
import logging
import argparse
import cv2
from ikomia.utils import tests, ik
from ikomia import core
from ikomia.dataprocess import workflow, displayIO

logger = logging.getLogger(__name__)


def test_cpp_base_class():
    logger.info("===== Test: task base class =====")
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


def test_task_parameters():
    logger.info("===== Test: set task parameters =====")
    algo = core.task.create(ik.ocv_box_filter)

    logger.info("----- Get parameters")
    logger.info(algo.get_parameters())

    logger.info("----- Use default parameters")
    logger.info(algo.get_param_object())
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    input_img = algo.get_input(0)
    input_img.setImage(img)
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
    if 'all' in running_tests or 'parameters' in running_tests:
        test_task_parameters()
    if 'all' in running_tests or 'outputs' in running_tests:
        test_get_task_outputs(opt.wgisd_dir)
    if 'all' in running_tests or 'output_image_graphics' in running_tests:
        test_get_image_with_graphics()
