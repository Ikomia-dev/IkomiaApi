import ikomia
import logging
import cv2
from ikomia.utils import tests
from ikomia.core import task, ParamMap
from ikomia.dataprocess import registry, displayIO

logger = logging.getLogger(__name__)


def test_task_parameters(ik_registry):
    logger.info("===== Test::set task parameters =====")
    algo = ik_registry.create_algorithm("Box Filter")
    logger.info("----- Run with default parameters")
    logger.info(algo.getParam())
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    # change parameters value from core.CParamMap
    logger.info("----- Run with manually set parameters (CParamMap)")
    params = ParamMap()
    params["borderType"] = str(4)
    params["anchorX"] = str(-1)
    params["anchorY"] = str(-1)
    params["kSizeHeight"] = str(13)
    params["kSizeWidth"] = str(13)
    params["bNormalize"] = str(1)
    params["ddepth"] = str(-1)
    algo.getParam().setParamMap(params)
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)

    # change parameters value from dict
    logger.info("----- Run with manually set parameters (dict)")
    task.set_parameter(algo, {"borderType": 4, "anchorX": -1, "anchorY": -1, "kSizeHeight": 29, "kSizeWidth": 29,
                              "bNormalize": 1, "ddepth": -1})
    logger.info(algo.getParam())
    algo.run()
    displayIO.display(algo.getOutput(0), algo.name)


if __name__ == "__main__":
    ikomia.initialize()
    reg = registry.IkomiaRegistry()
    test_task_parameters(reg)
