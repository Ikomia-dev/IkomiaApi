import logging
import os
import ikomia
from ikomia.dataprocess import registry, workflow
from ikomia.core import config
import numpy as np

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
    assert(wf.keywords == keywords)


def test_load(registry):
    # load test workflow
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_load", registry)
    wf.load(wf_path)
    logger.info("----- Workflow information -----")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_single_image_run(registry):
    img_path = get_test_image_directory() + "/Lena.png"
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_single_image_run", registry)
    wf.setAutoSave(True)
    wf.load(wf_path)

    # apply on local image
    wf.set_image_input(path=img_path)
    logger.info("Start workflow on local image...")
    wf.run()
    logger.info("Workflow finished successfully")

    # apply on web image
    img_url = "https://helpx.adobe.com/content/dam/help/en/photoshop/using/convert-color-image-black-white/jcr_content/main-pars/before_and_after/image-before/Landscape-Color.jpg"
    wf.set_image_input(index=0, url=img_url)
    logger.info("Start workflow on image url...")
    wf.run()
    logger.info("Workflow finished successfully")

    # apply on Numpy array
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    wf.set_image_input(index=0, array=img_array)
    logger.info("Start workflow on Numpy array...")
    wf.run()
    logger.info("Workflow finished successfully")


def test_directory_run(registry):
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_dir_run", registry)
    wf.setAutoSave(True)
    wf.load(wf_path)

    dir_path = get_test_image_directory()
    wf.set_directory_input(dir_path)
    logger.info("Start workflow on image directory...")
    wf.run()
    logger.info("Workflow finished successfully")


def test_resnet_train(registry, dataset_dir):
    wf_path = get_test_workflow_directory() + "/WorkflowResNetTrain.json"
    wf = workflow.Workflow("test_resnet", registry)
    wf.load(wf_path)
    wf.set_directory_input(dataset_dir)
    logger.info("Start ResNet training...")
    wf.run()
    logger.info("Training finished successfully")


def test_yolov5_train(registry):
    wf_path = get_test_workflow_directory() + "/WorkflowYoloV5Train.json"
    wf = workflow.Workflow("test_yolov5", registry)
    wf.load(wf_path)
    logger.info("Start YoloV5 training...")
    wf.run()
    logger.info("Training finished successfully")


def test_export_graphviz(registry):
    wf_path = get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_graphviz", registry)
    wf.load(wf_path)
    dot_file_name = wf.name + ".dot"
    path = os.path.join(config.main_cfg["data"]["path"], dot_file_name)
    wf.exportGraphviz(path)


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    ik_registry = registry.IkomiaRegistry()
    # test_metadata()
    # test_load(ik_registry)
    # test_single_image_run(ik_registry)
    # test_directory_run(ik_registry)
    # test_resnet_train(ik_registry, "/run/media/ludo/data/Ludo/Work/Ikomia/Images/Datasets/hymenoptera_data")
    # test_resnet_train(ik_registry, "/home/ludo/Images/Datasets/hymenoptera_data")
    # test_yolov5_train(ik_registry)
    test_export_graphviz(ik_registry)
