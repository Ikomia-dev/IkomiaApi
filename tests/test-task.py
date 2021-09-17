import ikomia
import logging
import argparse
import cv2
from ikomia.utils import tests, ik
from ikomia.core import task, ParamMap
from ikomia.dataprocess import workflow, displayIO

logger = logging.getLogger(__name__)


def test_task_parameters():
    logger.info("===== Test::set task parameters =====")
    algo = task.create(ik.BoxFilter)
    logger.info("----- Use default parameters")
    logger.info(algo.getParam())
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    logger.info("----- Manually set all parameters from ParamMap")
    params = ParamMap()
    params["borderType"] = str(4)
    params["anchorX"] = str(-1)
    params["anchorY"] = str(-1)
    params["kSizeHeight"] = str(13)
    params["kSizeWidth"] = str(13)
    params["bNormalize"] = str(1)
    params["ddepth"] = str(-1)
    algo.setParamValues(params)
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    logger.info("----- Manually set some of the parameters from ParamMap")
    params = algo.getParamValues()
    params["kSizeHeight"] = str(31)
    params["kSizeWidth"] = str(31)
    algo.setParamValues(params)
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    logger.info("----- Manually set all parameters from dict")
    task.set_parameters(algo, {"borderType": 4, "anchorX": -1, "anchorY": -1, "kSizeHeight": 29, "kSizeWidth": 29,
                               "bNormalize": 1, "ddepth": -1})
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    logger.info("----- Manually set some parameters from dict")
    task.set_parameters(algo, {"kSizeHeight": 29, "kSizeWidth": 29})
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)


def test_get_task_outputs(wgisd_dir):
    logger.info("===== Test::get task outputs of various data types =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestOutput.json"
    wf = workflow.Workflow("test_outputs", ikomia.ik_registry)
    wf.load(wf_path)

    wgisd_task = wf.find_task(ik.WGISD_Dataset, 0)[1]
    wgisd_params = wgisd_task.getParamValues()
    wgisd_params["data_folder_path"] = wgisd_dir + "/data"
    wgisd_params["class_file_path"] = wgisd_dir + "/classes.txt"
    wgisd_params["load_mask"] = str(False)
    wgisd_task.setParamValues(wgisd_params)
    wf.set_image_input(path=img_path)
    wf.run()

    logger.info("----- Get MobileNet SSD outputs: image, graphics and blob measure")
    detector_task = wf.find_task(ik.MobileNetSSD, 0)[1]
    img_out = task.get_image_output(detector_task)
    assert(img_out is not None)
    displayIO.display(img_out, detector_task.name)
    graphics_out = task.get_graphics_output(detector_task)
    assert (graphics_out is not None)
    displayIO.display(graphics_out, detector_task.name)
    blob_out = task.get_blob_measure_output(detector_task)
    assert (blob_out is not None)
    displayIO.display(blob_out, detector_task.name)

    logger.info("----- Get Split Operator outputs: 3 images")
    split_task = wf.find_task(ik.SplitOperator, 0)[1]
    img_out = task.get_image_output(split_task)
    assert (img_out is not None)
    assert(len(img_out) == 3)
    img_out = task.get_image_output(split_task, 2)
    displayIO.display(img_out, split_task.name + "- blue channel")

    logger.info("----- Get CalcHist outputs: numeric")
    hist_task = wf.find_task(ik.CalcHist, 0)[1]
    numeric_out = task.get_numeric_output(hist_task)
    assert (numeric_out is not None)
    displayIO.display(numeric_out, hist_task.name)

    logger.info("----- Get WGISD_Dataset outputs: dataset")
    dataset_out = task.get_dataset_output(wgisd_task)
    assert (dataset_out is not None)
    assert (dataset_out.getCategoryCount() > 0)
    assert (len(dataset_out.getImagePaths()) > 0)


def test_get_image_with_graphics():
    logger.info("===== Test::get image with graphics =====")
    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    # run
    t = task.create(ik.MobileNetSSD)
    t.getInput(0).setImage(img)
    t.run()
    # get image with graphics
    img_with_graphics = task.get_image_with_graphics(t, 0, 0)
    cv2.imshow("Image with graphics", img_with_graphics)
    cv2.waitKey(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--wgisd_dir", type=str, default="/home/ludo/Images/Datasets/wgisd",
                        help="Classification datatset folder")
    opt = parser.parse_args()

    test_task_parameters()
    test_get_task_outputs(opt.wgisd_dir)
    test_get_image_with_graphics()
