import logging
import os
from ikomia.dataprocess import registry, workflow

logger = logging.getLogger(__name__)


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Images"


def get_test_video_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Videos"


def get_test_workflow_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Workflows"


def test_metadata():
    wf = workflow.Workflow("test_metadata")
    description = "This is a test workflow"
    keywords = "ikomia,test,empty"
    wf.description = description
    wf.keywords = keywords
    logger.info(wf.description)
    logger.info(wf.keywords)
    assert(wf.description == description)
    assert (wf.keywords == keywords)


def test_load(reg):
    # load test workflow
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_load", reg)
    wf.load(wf_path)
    logger.info("----- Workflow information -----")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_simple_run(reg):
    img_path = get_test_image_directory() + "/Lena.png"
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_load", reg)
    wf.load(wf_path)
    wf.setAutoSave(True)
    wf.add_image_input(img_path)
    logger.info("----- Start running -----")
    wf.run()
    logger.info("----- Finished successfully -----")


if __name__ == "__main__":
    reg = registry.IkomiaRegistry()
    # test_metadata()
    test_load(reg)
    test_simple_run(reg)