import logging
import os
import argparse
from ikomia.dataprocess import workflow, displayIO
from ikomia.core import config
from ikomia.utils import tests
import numpy as np

logger = logging.getLogger(__name__)


def test_metadata():
    logger.info("===== Test::set workflow metadata =====")
    wf = workflow.Workflow("test_metadata")
    description = "This is a test workflow"
    keywords = "ikomia,test,empty"
    wf.description = description
    wf.keywords = keywords
    logger.info(wf.description)
    logger.info(wf.keywords)
    assert(wf.description == description)
    assert(wf.keywords == keywords)


def test_load():
    # load test workflow
    logger.info("===== Test::load workflow from JSON =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    logger.info("----- Workflow information:")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_single_image_run():
    logger.info("===== Test::execute workflow on single image =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

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


def test_directory_run():
    logger.info("===== Test::execute workflow on folder =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    dir_path = tests.get_test_image_directory()
    wf.set_directory_input(dir_path)
    logger.info("Start workflow on image directory...")
    wf.run()
    logger.info("Workflow finished successfully")


def test_run_common():
    logger.info("===== Test::execute workflow on common inputs =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    logger.info("----- Run on image from array")
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    workflow.run_on(wf, array=img_array)

    logger.info("----- Run on image from file path")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    workflow.run_on(wf, path=img_path)

    logger.info("----- Run on image from URL")
    img_url = "https://helpx.adobe.com/content/dam/help/en/photoshop/using/convert-color-image-black-white/jcr_content/main-pars/before_and_after/image-before/Landscape-Color.jpg"
    workflow.run_on(wf, url=img_url)

    logger.info("----- Run on image from folder")
    dir_path = tests.get_test_image_directory()
    workflow.run_on(wf, folder=dir_path)


def test_resnet_train(dataset_dir):
    logger.info("===== Test::launch ResNet training =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowResNetTrain.json"
    wf = workflow.load(wf_path)
    logger.info("Start ResNet training...")
    workflow.run_on(wf, folder=dataset_dir)
    logger.info("Training finished successfully")


def test_yolov5_train(wgisd_dataset_dir):
    logger.info("===== Test::launch YoloV5 training =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowYoloV5Train.json"
    wf = workflow.load(wf_path)

    # set dataset directory
    wgisd_id, wgisd = wf.find_task("WGISD_Dataset")
    wgisd_params = wgisd.getParamValues()
    wgisd_params["data_folder_path"] = wgisd_dataset_dir + "/data"
    wgisd_params["class_file_path"] = wgisd_dataset_dir + "/classes.txt"
    wgisd.setParamValues(wgisd_params)

    logger.info("Start YoloV5 training...")
    wf.run()
    logger.info("Training finished successfully")


def test_yolo_train(wgisd_dataset_dir):
    logger.info("===== Test::launch Darknet YOLO training =====")
    wf = workflow.create("YoloTrain")
    wgisd_id, wgisd = wf.add_task("WGISD_Dataset")
    wgisd_params = wgisd.getParamValues()
    wgisd_params["data_folder_path"] = wgisd_dataset_dir + "/data"
    wgisd_params["class_file_path"] = wgisd_dataset_dir + "/classes.txt"
    wgisd_params["load_mask"] = str(False)
    wgisd.setParamValues(wgisd_params)

    yolo_id, yolo = wf.add_task("YoloTrain")
    wf.connect_tasks(wgisd_id, yolo_id)

    wf.run()


def test_export_graphviz():
    logger.info("===== Test::export workflow as Graphviz =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    dot_file_name = wf.name + ".dot"
    path = os.path.join(config.main_cfg["data"]["path"], dot_file_name)
    wf.exportGraphviz(path)


def test_graph_structure():
    logger.info("===== Test::workflow introspection =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)

    # browse tasks
    ids = wf.getTaskIDs()
    for task_id in ids:
        task = wf.getTask(task_id)
        logger.info("##### Id:" + str(task_id) + " Name:" + task.name)
        logger.info(task)

    # get root and retrieve its child tasks
    root_id = wf.getRootID()
    assert(root_id in ids)
    childs = wf.getChilds(root_id)
    assert(len(childs) == 2)
    logger.info("Childs of root are: " + wf.getTask(childs[0]).name + " and " + wf.getTask(childs[1]).name)

    # find task from name
    task_id, task_obj = wf.find_task("CLAHE")
    assert(task_id and task_obj is not None)

    # get parents
    parents = wf.getParents(task_id)
    logger.info("Parent of CLAHE is " + wf.getTask(parents[0]).name)

    # get final tasks(leafs)
    msg = "Final tasks are: "
    final_tasks = wf.getFinalTasks()

    for task in final_tasks:
        msg += task.name + ","

    logger.info(msg)

    # input edges of a task
    task_id, task_obj = wf.find_task("DTFilter")
    assert(task_id and task_obj is not None)
    in_edges = wf.getInEdges(task_id)
    logger.info("Input edges of DTFilter: " + str(len(in_edges)))

    for edge in in_edges:
        logger.info("Edge port index: " + str(wf.getEdgeInfo(edge)))

    # output edges of a task
    task_id, task_obj = wf.find_task("Equalize histogram")
    assert(task_id and task_obj is not None)
    out_edges = wf.getOutEdges(task_id)
    logger.info("Output edges of Equalize histogram: " + str(len(out_edges)))

    for edge in out_edges:
        logger.info("Edge port index: " + str(wf.getEdgeInfo(edge)))

    # edge source and target
    src_task_id = wf.getEdgeSource(out_edges[0])
    logger.info("Source task of edge #" + str(out_edges[0]) + " is " + wf.getTask(src_task_id).name)
    target_task_id = wf.getEdgeTarget(out_edges[0])
    logger.info("Target task of edge #" + str(out_edges[0]) + " is " + wf.getTask(target_task_id).name)

    # delete edge
    wf.deleteEdge(out_edges[0])
    out_edges = wf.getOutEdges(task_id)
    assert (len(out_edges) == 1)

    # delete task
    count_before = len(wf.getTaskIDs())
    logger.info("Task count before delete: " + str(count_before))
    wf.deleteTask(task_id)
    count_after = len(wf.getTaskIDs())
    logger.info("Task count after delete: " + str(count_after))
    assert(count_before == count_after + 1)
    found_task = wf.find_task("Equalize histogram")
    assert(len(found_task) == 0)


def test_graph_build():
    logger.info("===== Test::create workflow from scratch =====")
    wf = workflow.create("FromScratch")

    # branch with auto-connection
    box_filter_id, box_filter = wf.add_task("Box Filter")
    wf.connect_tasks(wf.getRootID(), box_filter_id)

    clahe_id, clahe = wf.add_task("CLAHE")
    wf.connect_tasks(box_filter_id, clahe_id)

    dtfilterenhance_id, dtfilterenhance = wf.add_task("DTFilterEnhance")
    wf.connect_tasks(clahe_id, dtfilterenhance_id)

    lsc_id, lsc = wf.add_task("SuperpixelLSC")
    wf.connect_tasks(dtfilterenhance_id, lsc_id)

    # branch with manual connection
    bilateral_id, bilateral = wf.add_task("Bilateral Filter")
    wf.connect_tasks(wf.getRootID(), bilateral_id, [(0, 0)])

    equalize_id, equalize = wf.add_task("Equalize histogram")
    wf.connect_tasks(bilateral_id, equalize_id, [(0, 0)])

    dtfilter_id, dtfilter = wf.add_task("DTFilter")
    wf.connect_tasks(equalize_id, dtfilter_id, [(0, 0), (0, 1)])

    convert_id, convert = wf.add_task("ConvertTo")
    wf.connect_tasks(dtfilter_id, convert_id, [(0, 0)])

    seeds_id, seeds = wf.add_task("SuperpixelSEEDS")
    wf.connect_tasks(convert_id, seeds_id, [(0, 0)])

    # visualization
    displayIO.display(wf, "Manually built workflow")

    # run
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf.set_image_input(path=img_path)
    wf.run()

    # check results
    leaf_tasks = wf.getFinalTasks()
    for task in leaf_tasks:
        displayIO.display(task, task.name)


def test_time_metrics():
    logger.info("===== Test::compute workflow time metrics =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.set_image_input(path=img_path)
    wf.run()
    logger.info("Workflow running time (ms): " + str(wf.getTotalElapsedTime()))

    # manual retrieval
    ids = wf.getTaskIDs()
    for task_id in ids:
        task = wf.getTask(task_id)
        logger.info(task.name + ":" + str(task.getElapsedTime()) + " - From start: " + str(wf.getElapsedTimeTo(task_id)))

    # tool function
    logger.info(wf.get_time_metrics())


def test_get_outputs(wgisd_dataset_dir):
    logger.info("===== Test::get workflow outputs of various data types =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestOutput.json"
    wf = workflow.load(wf_path)

    # set WGISD_Dataset parameters
    wgisd_task = wf.find_task("WGISD_Dataset", 0)[1]
    wgisd_params = wgisd_task.getParamValues()
    wgisd_params["data_folder_path"] = wgisd_dataset_dir + "/data"
    wgisd_params["class_file_path"] = wgisd_dataset_dir + "/classes.txt"
    wgisd_params["load_mask"] = str(False)
    wgisd_task.setParamValues(wgisd_params)

    # run workflow
    wf.set_image_input(path=img_path)
    wf.run()

    logger.info("----- Get MobileNet SSD outputs: image, graphics and blob measure")
    img_out = wf.get_image_output(task_name="MobileNet SSD")
    assert (img_out is not None)
    displayIO.display(img_out, "MobileNet SSD")
    graphics_out = wf.get_graphics_output(task_name="MobileNet SSD")
    assert (graphics_out is not None)
    displayIO.display(graphics_out, "MobileNet SSD")
    blob_out = wf.get_blob_measure_output(task_name="MobileNet SSD")
    assert (blob_out is not None)
    displayIO.display(blob_out, "MobileNet SSD")

    logger.info("----- Get Split Operator outputs: 3 images")
    img_out = wf.get_image_output(task_name="Split Operator")
    assert (img_out is not None)
    assert (len(img_out) == 3)
    img_out = wf.get_image_output(task_name="Split Operator", index=2)
    assert (img_out is not None)
    displayIO.display(img_out, "Blue channel")

    logger.info("----- Get CalcHist outputs: numeric")
    hist_task_id = wf.find_task("CalcHist", 0)[0]
    numeric_out = wf.get_numeric_output(task_id=hist_task_id)
    assert (numeric_out is not None)
    displayIO.display(numeric_out, "CalcHist")

    logger.info("----- Get WGISD_Dataset outputs: dataset")
    dataset_out = wf.get_dataset_output(task_name="WGISD_Dataset")
    assert (dataset_out is not None)
    assert (dataset_out.getCategoryCount() > 0)
    assert (len(dataset_out.getImagePaths()) > 0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--classif_dataset_dir", type=str, default="/home/ludo/Images/Datasets/hymenoptera_data", help="Classification datatset folder")
    parser.add_argument("--detect_dataset_dir", type=str, default="/home/ludo/Images/Datasets/wgisd", help="Object detection datatset folder")
    parser.add_argument("--train_test", type=bool, default=True, help="Launch training tests")
    opt = parser.parse_args()

    test_metadata()
    test_load()
    test_single_image_run()
    test_directory_run()
    test_run_common()
    test_export_graphviz()
    test_graph_structure()
    test_time_metrics()
    test_graph_build()
    test_get_outputs(opt.detect_dataset_dir)

    if opt.train_test:
        test_resnet_train(opt.classif_dataset_dir)
        test_yolo_train(opt.detect_dataset_dir)
        test_yolov5_train(opt.detect_dataset_dir)
