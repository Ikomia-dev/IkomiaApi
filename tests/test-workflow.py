import logging
import os
import ikomia
from ikomia.dataprocess import registry, workflow, displayIO
from ikomia.core import config
from ikomia.utils import tests
import numpy as np

logger = logging.getLogger(__name__)


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
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_load", registry)
    wf.load(wf_path)
    logger.info("----- Workflow information -----")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_single_image_run(ik_registry):
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_single_image_run", ik_registry)
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


def test_directory_run(ik_registry):
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_dir_run", ik_registry)
    wf.setAutoSave(True)
    wf.load(wf_path)

    dir_path = tests.get_test_image_directory()
    wf.set_directory_input(dir_path)
    logger.info("Start workflow on image directory...")
    wf.run()
    logger.info("Workflow finished successfully")


def test_resnet_train(ik_registry, dataset_dir):
    wf_path = tests.get_test_workflow_directory() + "/WorkflowResNetTrain.json"
    wf = workflow.Workflow("test_resnet", ik_registry)
    wf.load(wf_path)
    wf.set_directory_input(dataset_dir)
    logger.info("Start ResNet training...")
    wf.run()
    logger.info("Training finished successfully")


def test_yolov5_train(ik_registry):
    wf_path = tests.get_test_workflow_directory() + "/WorkflowYoloV5Train.json"
    wf = workflow.Workflow("test_yolov5", ik_registry)
    wf.load(wf_path)
    logger.info("Start YoloV5 training...")
    wf.run()
    logger.info("Training finished successfully")


def test_export_graphviz(ik_registry):
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_graphviz", ik_registry)
    wf.load(wf_path)
    dot_file_name = wf.name + ".dot"
    path = os.path.join(config.main_cfg["data"]["path"], dot_file_name)
    wf.exportGraphviz(path)


def test_graph_structure(ik_registry):
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_single_image_run", ik_registry)
    wf.load(wf_path)

    # browse tasks
    ids = wf.getTaskIDs()
    for task_id in ids:
        task = wf.getTask(task_id)
        logger.info("===== Id:" + str(task_id) + " Name:" + task.name + " =====")
        logger.info(task)

    # get root and retrieve its child tasks
    root_id = wf.getRootID()
    assert(root_id in ids)
    childs = wf.getChilds(root_id)
    assert(len(childs) == 2)
    logger.info("Childs of root are: " + wf.getTask(childs[0]).name + " and " + wf.getTask(childs[1]).name)

    # find task from name
    found_task = wf.find_task("CLAHE")
    assert(len(found_task) == 1)

    # get parents
    parents = wf.getParents(found_task[0][0])
    logger.info("Parent of CLAHE is " + wf.getTask(parents[0]).name)

    # get final tasks(leafs)
    msg = "Final tasks are: "
    final_tasks = wf.getFinalTasks()

    for task in final_tasks:
        msg += task.name + ","

    logger.info(msg)

    # input edges of a task
    found_task = wf.find_task("DTFilter")
    assert (len(found_task) == 1)
    in_edges = wf.getInEdges(found_task[0][0])
    logger.info("Input edges of DTFilter: " + str(len(in_edges)))

    for edge in in_edges:
        logger.info("Edge port index: " + str(wf.getEdgeInfo(edge)))

    # output edges of a task
    found_task = wf.find_task("Equalize histogram")
    assert (len(found_task) == 1)
    out_edges = wf.getOutEdges(found_task[0][0])
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
    out_edges = wf.getOutEdges(found_task[0][0])
    assert (len(out_edges) == 1)

    # delete task
    count_before = len(wf.getTaskIDs())
    logger.info("Task count before delete: " + str(count_before))
    wf.deleteTask(found_task[0][0])
    count_after = len(wf.getTaskIDs())
    logger.info("Task count after delete: " + str(count_after))
    assert(count_before == count_after + 1)
    found_task = wf.find_task("Equalize histogram")
    assert(len(found_task) == 0)


def test_graph_build(ik_registry):
    wf = workflow.Workflow("FromScratch", ik_registry)

    # branch with auto-connection
    box_filter_id = wf.add_task("Box Filter")
    wf.connect_tasks(wf.getRootID(), box_filter_id)

    clahe_id = wf.add_task("CLAHE")
    wf.connect_tasks(box_filter_id, clahe_id)

    dtfilterenhance_id = wf.add_task("DTFilterEnhance")
    wf.connect_tasks(clahe_id, dtfilterenhance_id)

    lsc_id = wf.add_task("SuperpixelLSC")
    wf.connect_tasks(dtfilterenhance_id, lsc_id)

    # branch with manual connection
    bilateral_id = wf.add_task("Bilateral Filter")
    wf.connect_tasks(wf.getRootID(), bilateral_id, [(0, 0)])

    equalize_id = wf.add_task("Equalize histogram")
    wf.connect_tasks(bilateral_id, equalize_id, [(0, 0)])

    dtfilter_id = wf.add_task("DTFilter")
    wf.connect_tasks(equalize_id, dtfilter_id, [(0, 0), (0, 1)])

    convert_id = wf.add_task("ConvertTo")
    wf.connect_tasks(dtfilter_id, convert_id, [(0, 0)])

    seeds_id = wf.add_task("SuperpixelSEEDS")
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


def test_time_metrics(ik_registry):
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.Workflow("test_single_image_run", ik_registry)
    wf.load(wf_path)
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


if __name__ == "__main__":
    ikomia.initialize()
    reg = registry.IkomiaRegistry()
    # test_metadata()
    # test_load(reg)
    # test_single_image_run(reg)
    # test_directory_run(reg)
    # test_resnet_train(reg, "/run/media/ludo/data/Ludo/Work/Ikomia/Images/Datasets/hymenoptera_data")
    # test_resnet_train(reg, "/home/ludo/Images/Datasets/hymenoptera_data")
    # test_yolov5_train(reg)
    # test_export_graphviz(reg)
    test_graph_structure(reg)
    # test_time_metrics(reg)
    # test_graph_build(reg)
