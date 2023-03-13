import logging
import argparse
from ikomia.utils import tests, ik
from ikomia.dataprocess import workflow

logger = logging.getLogger(__name__)


def test_resnet_train(dataset_dir):
    logger.info("===== Test::launch ResNet training =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowResNetTrain.json"
    wf = workflow.load(wf_path)
    logger.info("Start ResNet training...")
    wf.run_on(folder=dataset_dir)
    logger.info("Training finished successfully")


def test_yolov5_train(wgisd_dataset_dir):
    logger.info("===== Test::launch YoloV5 training =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowYoloV5Train.json"
    wf = workflow.load(wf_path)

    # set dataset directory
    wgisd_id, wgisd = wf.find_task(ik.dataset_wgisd)
    wgisd_params = wgisd.get_parameters()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dataset_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dataset_dir + "/classes.txt"
    wgisd.set_parameters(wgisd_params)

    logger.info("Start YoloV5 training...")
    wf.run()
    logger.info("Training finished successfully")


def test_yolo_train(wgisd_dataset_dir):
    logger.info("===== Test::launch Darknet YOLO training =====")
    wf = workflow.create("YoloTrain")
    wgisd = wf.add_task(ik.dataset_wgisd)
    wgisd_params = wgisd.get_parameters()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dataset_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dataset_dir + "/classes.txt"
    wgisd_params[ik.dataset_wgisd_param.seg_mask_mode] = "None"
    wgisd.set_parameters(wgisd_params)

    yolo = wf.add_task(ik.train_yolo)
    yolo_params = yolo.get_parameters()
    yolo_params["batchSize"] = 16
    task.set_parameters(yolo, yolo_params)
    wf.connect_tasks(wgisd, yolo)

    wf.run()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--classif_dataset_dir",
                        type=str,
                        default="/home/ludo/Images/Datasets/hymenoptera_data", help="Classification datatset folder")
    parser.add_argument("--detect_dataset_dir",
                        type=str,
                        default="/home/ludo/Images/Datasets/wgisd", help="Object detection datatset folder")
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")
    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'train_resnet' in running_tests:
        test_resnet_train(opt.classif_dataset_dir)
    if 'all' in running_tests or 'train_yolov4' in running_tests:
        test_yolo_train(opt.detect_dataset_dir)
    if 'all' in running_tests or 'train_yolov5' in running_tests:
        test_yolov5_train(opt.detect_dataset_dir)

