import ikomia
import logging
import cv2
from ikomia.utils import tests
from ikomia.core import task, ParamMap
from ikomia.dataprocess import workflow, displayIO

logger = logging.getLogger(__name__)


def test_task_parameters():
    logger.info("===== Test::set task parameters =====")
    algo = ikomia.ik_registry.create_algorithm("Box Filter")
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


def test_get_task_outputs():
    logger.info("===== Test::get task outputs of various data types =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestOutput.json"
    wf = workflow.Workflow("test_single_image_run", ikomia.ik_registry)
    wf.load(wf_path)
    wf.set_image_input(path=img_path)
    wf.run()

    detector_task = wf.find_task("MobileNet SSD", 0)[1]
    img_out = task.get_image_output(detector_task)
    displayIO.display(img_out, detector_task.name)
    graphics_out = task.get_graphics_output(detector_task)
    displayIO.display(graphics_out, detector_task.name)
    blob_out = task.get_blob_measure_output(detector_task)
    displayIO.display(blob_out, detector_task.name)


if __name__ == "__main__":
    test_task_parameters()
    test_get_task_outputs()
