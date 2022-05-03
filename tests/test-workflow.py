import logging
import os
import argparse
import ikomia
from ikomia.dataprocess import workflow, displayIO
from ikomia.core import config
from ikomia.utils import tests, ik
import numpy as np
import cv2

logger = logging.getLogger(__name__)


def test_metadata():
    logger.info("===== Test::set workflow metadata =====")
    wf = workflow.create("test_metadata")
    description = "This is a test workflow"
    keywords = "ikomia,test,empty"
    wf.description = description
    wf.keywords = keywords
    logger.info(wf.description)
    logger.info(wf.keywords)
    assert(wf.description == description)
    assert(wf.keywords == keywords)


def test_load_builtin():
    # load test workflow
    logger.info("===== Test::load workflow from JSON with builtin algorithms =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    logger.info("----- Workflow information:")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_load_marketplace():
    # load test workflow
    logger.info("===== Test::load workflow from JSON with marketplace algorithm =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowYolorBlur.json"
    wf = workflow.load(wf_path)
    logger.info("----- Workflow information:")
    logger.info(wf.name)
    logger.info(wf.description)
    logger.info(wf.keywords)
    logger.info("Input count: " + str(wf.getInputCount()))
    logger.info("Task count: " + str(wf.getTaskCount()))


def test_run_single_image():
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


def test_run_image_folder():
    logger.info("===== Test::execute image workflow on folder =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    dir_path = tests.get_test_image_directory()
    wf.set_directory_input(dir_path)
    logger.info("Start workflow on image directory...")
    wf.run()
    logger.info("Workflow finished successfully")


def test_run_image_common():
    logger.info("===== Test::execute workflow on common image inputs =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    logger.info("----- Run on image from array")
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    wf.run_on(array=img_array)

    logger.info("----- Run on image from file path")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf.run_on(path=img_path)

    logger.info("----- Run on image from URL")
    img_url = "https://helpx.adobe.com/content/dam/help/en/photoshop/using/convert-color-image-black-white/jcr_content/main-pars/before_and_after/image-before/Landscape-Color.jpg"
    wf.run_on(url=img_url)

    logger.info("----- Run on image from folder")
    dir_path = tests.get_test_image_directory()
    wf.run_on(folder=dir_path)


def test_run_single_video():
    logger.info("===== Test::execute workflow on single video =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    logger.info("----- Run on video from file path")
    video_path = tests.get_test_video_directory() + "/basketball.mp4"
    wf.run_on(path=video_path)

    logger.info("----- Run on video from URL")
    video_url = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4"
    wf.run_on(url=video_url)


def test_run_video_folder():
    logger.info("===== Test::execute workflow on folder =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.setAutoSave(True)

    logger.info("----- Run on video from folder")
    dir_path = tests.get_test_video_directory()
    wf.run_on(folder=dir_path)


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
    wgisd_params = wgisd.getParamValues()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dataset_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dataset_dir + "/classes.txt"
    wgisd.setParamValues(wgisd_params)

    logger.info("Start YoloV5 training...")
    wf.run()
    logger.info("Training finished successfully")


def test_yolo_train(wgisd_dataset_dir):
    logger.info("===== Test::launch Darknet YOLO training =====")
    wf = workflow.create("YoloTrain")
    wgisd_id, wgisd = wf.add_task(ik.dataset_wgisd)
    wgisd_params = wgisd.getParamValues()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dataset_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dataset_dir + "/classes.txt"
    wgisd_params[ik.dataset_wgisd_param.seg_mask_mode] = "None"
    wgisd.setParamValues(wgisd_params)

    yolo_id, yolo = wf.add_task(ik.train_yolo)
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

    # get all tasks
    logger.info(wf.get_tasks())

    # browse tasks
    ids = wf.getTaskIDs()
    for task_id in ids:
        task = wf.getTask(task_id)
        logger.info("##### Id:" + str(task_id) + " Name:" + task.name)
        logger.info(task)

    # get root and retrieve its child tasks
    root_id = wf.getRootID()
    assert(root_id in ids)
    childs = wf.getChildren(root_id)
    assert(len(childs) == 2)
    logger.info("Childs of root are: " + wf.getTask(childs[0]).name + " and " + wf.getTask(childs[1]).name)

    # find task from name
    task_id, task_obj = wf.find_task(ik.ocv_clahe)
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
    task_id, task_obj = wf.find_task(ik.ocv_dt_filter)
    assert(task_id and task_obj is not None)
    in_edges = wf.getInEdges(task_id)
    logger.info("Input edges of DTFilter: " + str(len(in_edges)))

    for edge in in_edges:
        logger.info("Edge port index: " + str(wf.getEdgeInfo(edge)))

    # output edges of a task
    task_id, task_obj = wf.find_task(ik.ocv_equalize_histogram)
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
    found_task = wf.find_task(ik.ocv_equalize_histogram)
    assert(len(found_task) == 0)


def test_graph_build():
    logger.info("===== Test::create workflow from scratch =====")
    wf = workflow.create("FromScratch")

    # branch with auto-connection
    box_filter_id, box_filter = wf.add_task(ik.ocv_box_filter)
    wf.connect_tasks(wf.getRootID(), box_filter_id)

    clahe_id, clahe = wf.add_task(ik.ocv_clahe)
    wf.connect_tasks(box_filter_id, clahe_id)

    dtfilterenhance_id, dtfilterenhance = wf.add_task(ik.ocv_dt_filter_enhance)
    wf.connect_tasks(clahe_id, dtfilterenhance_id)

    lsc_id, lsc = wf.add_task(ik.ocv_superpixel_lsc)
    wf.connect_tasks(dtfilterenhance_id, lsc_id)

    # branch with manual connection
    bilateral_id, bilateral = wf.add_task(ik.ocv_bilateral_filter)
    wf.connect_tasks(wf.getRootID(), bilateral_id, [(0, 0)])

    equalize_id, equalize = wf.add_task(ik.ocv_equalize_histogram)
    wf.connect_tasks(bilateral_id, equalize_id, [(0, 0)])

    dtfilter_id, dtfilter = wf.add_task(ik.ocv_dt_filter)
    wf.connect_tasks(equalize_id, dtfilter_id, [(0, 0), (0, 1)])

    convert_id, convert = wf.add_task(ik.ocv_convert_to)
    wf.connect_tasks(dtfilter_id, convert_id, [(0, 0)])

    seeds_id, seeds = wf.add_task(ik.ocv_superpixel_seeds)
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
    wgisd_task = wf.find_task(ik.dataset_wgisd, 0)[1]
    wgisd_params = wgisd_task.getParamValues()
    wgisd_params[ik.dataset_wgisd_param.data_folder_path] = wgisd_dataset_dir + "/data"
    wgisd_params[ik.dataset_wgisd_param.class_file_path] = wgisd_dataset_dir + "/classes.txt"
    wgisd_params[ik.dataset_wgisd_param.seg_mask_mode] = "None"
    wgisd_task.setParamValues(wgisd_params)

    # run workflow
    wf.set_image_input(path=img_path)
    wf.run()

    logger.info("----- Get MobileNet SSD outputs: image, graphics and blob measure")
    img_out = wf.get_image_output(task_name=ik.infer_mobilenet_ssd)
    assert (img_out is not None)
    displayIO.display(img_out, "MobileNet SSD")
    graphics_out = wf.get_graphics_output(task_name=ik.infer_mobilenet_ssd)
    assert (graphics_out is not None)
    displayIO.display(graphics_out, "MobileNet SSD")
    blob_out = wf.get_blob_measure_output(task_name=ik.infer_mobilenet_ssd)
    assert (blob_out is not None)
    displayIO.display(blob_out, "MobileNet SSD")

    logger.info("----- Get Split Operator outputs: 3 images")
    img_out = wf.get_image_output(task_name=ik.ocv_split)
    assert (img_out is not None)
    assert (len(img_out) == 3)
    img_out = wf.get_image_output(task_name=ik.ocv_split, index=2)
    assert (img_out is not None)
    displayIO.display(img_out, "Blue channel")

    logger.info("----- Get CalcHist outputs: numeric")
    hist_task_id = wf.find_task(ik.ocv_calc_hist, 0)[0]
    numeric_out = wf.get_numeric_output(task_id=hist_task_id)
    assert (numeric_out is not None)
    displayIO.display(numeric_out, "CalcHist")

    logger.info("----- Get WGISD_Dataset outputs: dataset")
    dataset_out = wf.get_dataset_output(task_name=ik.dataset_wgisd)
    assert (dataset_out is not None)
    assert (dataset_out.getCategoryCount() > 0)
    assert (len(dataset_out.getImagePaths()) > 0)


def test_get_image_with_graphics():
    logger.info("===== Test::get image with graphics =====")
    # create workflow with MobileNet SSD task
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf = workflow.create("ImageWithGraphics")
    id1, t1 = wf.add_task(ik.infer_mobilenet_ssd)
    wf.connect_tasks(wf.getRootID(), id1)
    wf.run_on(path=img_path)

    logger.info("-----Get image from task id")
    image1 = wf.get_image_with_graphics(task_id=id1, image_index=0, graphics_index=0)
    assert(image1 is not None)
    cv2.imshow("MobileNet SSD", image1)
    cv2.waitKey(0)

    # add second MobileNet SSD task
    id2, t2 = wf.add_task(ik.infer_mobilenet_ssd)
    wf.connect_tasks(wf.getRootID(), id2)
    wf.run_on(path=img_path)

    logger.info("-----Get image from task name")
    images = wf.get_image_with_graphics(task_name=ik.infer_mobilenet_ssd, image_index=0, graphics_index=0)
    assert(len(images) == 2)
    cv2.imshow("MobileNet SSD #1", images[0])
    cv2.imshow("MobileNet SSD #2", images[1])
    cv2.waitKey(0)


def test_get_image():
    logger.info("===== Test::get image =====")
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.run_on(path=img_path)

    image = wf.get_image(task_name=ik.ocv_box_filter)
    assert image.size != 0
    clahe_id, clahe = wf.find_task(name=ik.ocv_clahe)
    image = wf.get_image(task_id=clahe_id)
    assert image.size != 0
    images = wf.get_image(task_name=ik.ocv_superpixel_lsc)
    assert(len(images) == 3)
    image = wf.get_image(task_name=ik.ocv_superpixel_lsc, index=0)
    assert image.size != 0


def test_set_task_parameters():
    logger.info("===== Test::set parameters of specific tasks =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)

    bf_id, box_filter = wf.find_task(name=ik.ocv_box_filter, index=0)
    logger.info(box_filter.getParam())
    wf.set_parameters({ik.ocv_box_filter_param.kSizeHeight: 11,
                       ik.ocv_box_filter_param.kSizeWidth: 11}, task_name=ik.ocv_box_filter, index=0)
    params = box_filter.getParamValues()
    assert (params["kSizeHeight"] == str(11) and params["kSizeWidth"] == str(11))

    bl_id, bilateral_filter = wf.find_task(name=ik.ocv_bilateral_filter, index=0)
    logger.info(bilateral_filter.getParam())
    wf.set_parameters({ik.ocv_bilateral_filter_param.sigmaSpace: 31.0,
                       ik.ocv_bilateral_filter_param.sigmaColor: 11.0}, task_id=bl_id)
    params = bilateral_filter.getParamValues()

    assert (float(params["sigmaSpace"]) == 31.0 and float(params["sigmaColor"]) == 11.0)


def test_video_stream():
    cap = cv2.VideoCapture(0)

    wf = workflow.create("Video Processing")
    clahe_id, clahe = wf.add_task(ik.ocv_clahe)
    wf.connect_tasks(wf.getRootID(), clahe_id)
    canny_id, canny = wf.add_task(ik.ocv_canny)
    wf.connect_tasks(clahe_id, canny_id)

    while True:
        ret, img = cap.read()
        wf.run_on(array=img)
        res_img = wf.get_image(canny_id, 0)
        cv2.imshow("Result", res_img)

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


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
    running_tests = 'graph_struct'

    ikomia.authenticate()

    if 'all' in running_tests or 'metadata' in running_tests:
        test_metadata()
    if 'all' in running_tests or 'builtin' in running_tests:
        test_load_builtin()
    if 'all' in running_tests or 'marketplace' in running_tests:
        test_load_marketplace()
    if 'all' in running_tests or 'run_single_image' in running_tests:
        test_run_single_image()
    if 'all' in running_tests or 'run_image_folder' in running_tests:
        test_run_image_folder()
    if 'all' in running_tests or 'run_image_common' in running_tests:
        test_run_image_common()
    if 'all' in running_tests or 'run_single_video' in running_tests:
        test_run_single_video()
    if 'all' in running_tests or 'run_video_folder' in running_tests:
        test_run_video_folder()
    if 'all' in running_tests or 'graphviz' in running_tests:
        test_export_graphviz()
    if 'all' in running_tests or 'graph_struct' in running_tests:
        test_graph_structure()
    if 'all' in running_tests or 'time_metrics' in running_tests:
        test_time_metrics()
    if 'all' in running_tests or 'graph_build' in running_tests:
        test_graph_build()
    if 'all' in running_tests or 'outputs' in running_tests:
        test_get_outputs(opt.detect_dataset_dir)
    if 'all' in running_tests or 'output_image_graphics' in running_tests:
        test_get_image_with_graphics()
    if 'all' in running_tests or 'output_image' in running_tests:
        test_get_image()
    if 'all' in running_tests or 'set_parameters' in running_tests:
        test_set_task_parameters()
    if 'all' in running_tests or 'test_video_stream' in running_tests:
        test_video_stream()
    if 'all' in running_tests or 'train_resnet' in running_tests:
        test_resnet_train(opt.classif_dataset_dir)
    if 'all' in running_tests or 'train_yolov4' in running_tests:
        test_yolo_train(opt.detect_dataset_dir)
    if 'all' in running_tests or 'train_yolov5' in running_tests:
        test_yolov5_train(opt.detect_dataset_dir)
