import logging
import os
import shutil
import argparse
import ikomia
from ikomia.dataprocess import \
    workflow, CWorkflow, CImageIO, CVideoIO, CPathIO, CNumericIO, CObjectDetectionIO, CInstanceSegmentationIO, \
    CSemanticSegmentationIO, CKeypointsIO, CGraphicsOutput
from ikomia.core import IODataType
from ikomia.utils import tests, ik
import numpy as np
import cv2

logger = logging.getLogger(__name__)


def test_cpp_workflow():
    """
    Test Python bindings of Ikomia Core CWorkflow
    """
    logger.warning("===== Test: CWorkflow =====")

    # Default ctor
    wf = CWorkflow()
    assert wf.get_root_id() != 0
    # Ctor with name
    name = "Test workflow"
    wf = CWorkflow(name)
    assert wf.get_root_id() != 0
    assert wf.name == name
    # Ctor with name and registry
    name = "Test workflow"
    wf = CWorkflow(name, ikomia.ik_registry)
    assert wf.get_root_id() != 0
    assert wf.name == name

    # Metadata
    description = "This is a test workflow"
    keywords = "ikomia,test,empty"
    wf.description = description
    wf.keywords = keywords
    assert (wf.description == description)
    assert (wf.keywords == keywords)

    # Load
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTest1.json")
    required_tasks = wf.get_required_tasks(wf_path)
    assert len(required_tasks) == 9
    wf.load(wf_path)

    # Get IDs
    ids = wf.get_task_ids()
    assert len(ids) == len(required_tasks) + 1  # with root

    # Browse tasks
    for task_id in ids:
        t = wf.get_task(task_id)
        logger.warning(f"Id: {task_id} - Name: {t.name}")
        logger.warning(t)

    # Get root and retrieve its child tasks
    root_id = wf.get_root_id()
    assert (root_id in ids)
    children = wf.get_children(root_id)
    assert (len(children) == 2)
    logger.warning(f"Childs of root are: {wf.get_task(children[0]).name} and {wf.get_task(children[1]).name}")

    # Get parents
    t = wf.get_task(ids[5])
    parents = wf.get_parents(ids[5])
    logger.warning(f"Parent of {t.name} is {wf.get_task(parents[0]).name}")

    # Get final tasks(leafs)
    msg = "Final tasks are: "
    final_tasks = wf.get_final_tasks()
    msg += ",".join([t.name for t in final_tasks])

    logger.warning(msg)

    # Input edges of a task
    t = wf.get_task(ids[5])
    in_edges = wf.get_in_edges(ids[5])
    logger.warning(f"Input edges of {t.name}: {len(in_edges)}")

    for edge in in_edges:
        logger.warning(f"Edge port index: {wf.get_edge_info(edge)}")

    # Output edges of a task
    out_edges = wf.get_out_edges(ids[5])
    logger.warning(f"Output edges of {t.name}: {len(out_edges)}")

    for edge in out_edges:
        logger.warning(f"Edge port index: {wf.get_edge_info(edge)}")

    # Edge source and target
    src_task_id = wf.get_edge_source(out_edges[0])
    logger.warning(f"Source task of edge # {out_edges[0]} is {wf.get_task(src_task_id).name}")
    target_task_id = wf.get_edge_target(out_edges[0])
    logger.warning(f"Target task of edge # {out_edges[0]} is {wf.get_task(target_task_id).name}")

    # Set input
    types = wf.get_root_target_types()
    assert len(types) == 1
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    img_in = CImageIO(IODataType.IMAGE, "Source image", image_path)
    wf.set_input(img_in, 0, True)
    assert wf.get_input_count() == 1
    assert wf.get_input_data_type(0) == IODataType.IMAGE

    # Run
    wf.update_start_time()
    wf.run()
    logger.warning(f"Total elapsed time: {wf.get_total_elapsed_time()} ms")
    logger.warning(f"Elapsed time to {t.name}: {wf.get_elapsed_time_to(ids[5])} ms")

    # Run with auto-save
    output_folder = os.path.join(tests.get_test_image_directory(), "Tmp")
    wf.set_output_folder(output_folder)
    wf.set_auto_save(True)
    wf.run()
    output_folder = wf.get_last_run_folder()
    assert os.path.isdir(output_folder)
    shutil.rmtree(output_folder)

    # Add/remove input
    wf.remove_input(0)
    assert wf.get_input_count() == 0
    wf.add_input(img_in)
    assert wf.get_input_count() == 1
    wf.clear_inputs()
    assert wf.get_input_count() == 0
    wf.clear_output_data()

    # Delete edge
    wf.delete_edge(out_edges[0])
    out_edges = wf.get_out_edges(ids[5])
    assert (len(out_edges) == 0)

    # Delete task
    count_before = len(wf.get_task_ids())
    logger.warning(f"Task count before delete: {count_before}")
    wf.delete_task(ids[5])
    count_after = len(wf.get_task_ids())
    logger.warning(f"Task count after delete: {count_after}")
    assert (count_before == count_after + 1)

    # Save
    save_path = os.path.join(tests.get_test_workflow_directory(), "workflow_tmp.json")
    wf.save(save_path)
    assert os.path.isfile(save_path)
    os.remove(save_path)
    save_graphviz = os.path.join(tests.get_test_workflow_directory(), "workflow_graphviz.dot")
    wf.export_graphviz(save_graphviz)
    assert os.path.isfile(save_graphviz)
    os.remove(save_graphviz)


def test_ctor():
    logger.warning("===== Test: Workflow constructor=====")
    name = "Test workflow"

    wf = workflow.Workflow()
    assert wf.name == "Untitled"
    assert wf.registry is not None
    assert wf.output_folder
    assert wf.root_uuid
    assert len(wf.task_to_id) > 0

    wf = workflow.Workflow(name)
    assert wf.name == name
    assert wf.registry is not None
    assert wf.output_folder
    assert wf.root_uuid
    assert len(wf.task_to_id) > 0


def test_root():
    logger.warning("===== Test: Workflow::root() =====")
    name = "Test workflow"
    wf = workflow.Workflow(name)
    root = wf.root()
    assert root is not None
    assert wf.get_task_id(root) == wf.get_root_id()


def test_set_image_input():
    logger.warning("===== Test: Workflow::set_image_input() =====")
    name = "Test workflow"
    img_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    img_url = "https://helpx.adobe.com/content/dam/help/en/photoshop/using/convert-color-image-black-white/jcr_content/main-pars/before_and_after/image-before/Landscape-Color.jpg"

    # From path
    wf = workflow.Workflow(name)
    wf.set_image_input(path=img_path)
    assert wf.get_input_count() == 1
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    assert img_in.data_type == IODataType.IMAGE
    assert img_in.is_data_available()
    assert img_in.get_image() is not None

    # From URL
    wf = workflow.Workflow(name)
    wf.set_image_input(url=img_url)
    assert wf.get_input_count() == 1
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    assert img_in.data_type == IODataType.IMAGE
    assert img_in.is_data_available()
    assert img_in.get_image() is not None

    # From array
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    wf = workflow.Workflow(name)
    wf.set_image_input(array=img_array)
    assert wf.get_input_count() == 1
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    assert img_in.data_type == IODataType.IMAGE
    assert img_in.is_data_available()
    assert img_in.get_image() is not None

    # Data type
    wf = workflow.Workflow(name)
    wf.set_image_input(path=img_path, datatype=IODataType.IMAGE_LABEL)
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    assert img_in.data_type == IODataType.IMAGE_LABEL

    # Index
    wf = workflow.Workflow(name)
    wf.set_image_input(path=img_path, index=-1)
    assert wf.get_input_count() == 1
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    wf.set_image_input(path=img_path, index=0)
    assert wf.get_input_count() == 1
    img_in = wf.get_input(0)
    assert type(img_in) == CImageIO
    wf.set_image_input(path=img_path, index=1)
    assert wf.get_input_count() == 2
    img_in = wf.get_input(1)
    assert type(img_in) == CImageIO
    wf.set_image_input(path=img_path, index=10)
    assert wf.get_input_count() == 3
    img_in = wf.get_input(2)
    assert type(img_in) == CImageIO


def test_set_video_input():
    logger.warning("===== Test: Workflow::set_video_input() =====")
    name = "Test workflow"
    video_path = os.path.join(tests.get_test_video_directory(), "basketball.mp4")
    video_url = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4"

    # From path
    wf = workflow.Workflow(name)
    wf.set_video_input(path=video_path)
    assert wf.get_input_count() == 1
    video_in = wf.get_input(0)
    assert type(video_in) == CVideoIO
    assert video_in.data_type == IODataType.VIDEO
    assert video_in.has_video()

    # From URL
    wf = workflow.Workflow(name)
    wf.set_video_input(url=video_url)
    assert wf.get_input_count() == 1
    video_in = wf.get_input(0)
    assert type(video_in) == CVideoIO
    assert video_in.data_type == IODataType.VIDEO
    assert video_in.has_video()

    # Data type
    wf = workflow.Workflow(name)
    wf.set_video_input(path=video_path, datatype=IODataType.VIDEO_LABEL)
    video_in = wf.get_input(0)
    assert type(video_in) == CVideoIO
    assert video_in.data_type == IODataType.VIDEO_LABEL

    # Index
    wf = workflow.Workflow(name)
    wf.set_video_input(path=video_path, index=-1)
    assert wf.get_input_count() == 1
    video_in = wf.get_input(0)
    assert type(video_in) == CVideoIO
    wf.set_video_input(path=video_path, index=0)
    assert wf.get_input_count() == 1
    video_in = wf.get_input(0)
    assert type(video_in) == CVideoIO
    wf.set_video_input(path=video_path, index=1)
    assert wf.get_input_count() == 2
    video_in = wf.get_input(1)
    assert type(video_in) == CVideoIO
    wf.set_video_input(path=video_path, index=10)
    assert wf.get_input_count() == 3
    video_in = wf.get_input(2)
    assert type(video_in) == CVideoIO


def test_set_directory_input():
    logger.warning("===== Test: Workflow::set_directory_input() =====")
    name = "Test workflow"
    dir_path = tests.get_test_image_directory()
    wf = workflow.Workflow(name)
    wf.set_directory_input(dir_path)
    assert wf.get_input_count() == 1
    dir_in = wf.get_input(0)
    assert type(dir_in) == CPathIO
    assert dir_in.data_type == IODataType.FOLDER_PATH
    assert dir_in.is_data_available()

    # Index
    wf = workflow.Workflow(name)
    wf.set_directory_input(dir_path, index=-1)
    assert wf.get_input_count() == 1
    dir_in = wf.get_input(0)
    assert type(dir_in) == CPathIO
    wf.set_directory_input(dir_path, index=0)
    assert wf.get_input_count() == 1
    dir_in = wf.get_input(0)
    assert type(dir_in) == CPathIO
    wf.set_directory_input(dir_path, index=1)
    assert wf.get_input_count() == 2
    dir_in = wf.get_input(1)
    assert type(dir_in) == CPathIO
    wf.set_directory_input(dir_path, index=10)
    ins = wf.get_inputs()
    assert wf.get_input_count() == 3
    dir_in = wf.get_input(2)
    assert type(dir_in) == CPathIO


def test_set_parameters():
    logger.warning("===== Test: Workflow::set_parameters() =====")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTest1.json")
    wf = workflow.load(wf_path)

    # From task object
    box_filter = wf.find_task(name=ik.ocv_box_filter.name(), index=0)
    logger.warning(box_filter.get_param_object())
    params = {ik.ocv_box_filter.kSizeHeight: "11", ik.ocv_box_filter.kSizeWidth: "11"}
    wf.set_parameters(params, task_obj=box_filter)
    params_new = box_filter.get_parameters()
    assert params_new[ik.ocv_box_filter.kSizeHeight] == params[ik.ocv_box_filter.kSizeHeight]
    assert params_new[ik.ocv_box_filter.kSizeWidth] == params[ik.ocv_box_filter.kSizeWidth]

    # From task name
    bilateral_filter = wf.find_task(name=ik.ocv_bilateral_filter.name(), index=0)
    logger.warning(bilateral_filter.get_param_object())
    params = {ik.ocv_bilateral_filter.sigmaSpace: "31.0", ik.ocv_bilateral_filter.sigmaColor: "11.0"}
    wf.set_parameters(params, task_name=ik.ocv_bilateral_filter.name())
    params_new = bilateral_filter.get_parameters()
    assert float(params_new[ik.ocv_bilateral_filter.sigmaSpace]) == float(params[ik.ocv_bilateral_filter.sigmaSpace])
    assert float(params_new[ik.ocv_bilateral_filter.sigmaColor]) == float(params[ik.ocv_bilateral_filter.sigmaColor])


def test_load():
    logger.warning("===== Test: Workflow::load() =====")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTest1.json")
    wf = workflow.load(wf_path)

    logger.warning("----- Workflow information:")
    logger.warning(wf.name)
    logger.warning(wf.description)
    logger.warning(wf.keywords)
    logger.warning("Task count: " + str(wf.get_task_count()))

    assert wf.name
    assert wf.get_task_count() == 10

    ids = wf.get_task_ids()
    for uuid in wf.task_to_id:
        assert wf.task_to_id[uuid] in ids


def _check_leaf_outputs(wf):
    leaf_tasks = wf.get_final_tasks()
    for t in leaf_tasks:
        outputs = t.get_outputs()
        for output in outputs:
            assert output.is_data_available()


def test_run():
    logger.warning("===== Test: Workflow::run() =====")

    # Image
    logger.warning("Run on image...")
    img_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTest1.json")
    wf = workflow.load(wf_path)
    wf.set_image_input(path=img_path)
    wf.run()
    _check_leaf_outputs(wf)
    logger.warning("Workflow finished successfully")

    # Video
    logger.warning("Run on video...")
    video_path = os.path.join(tests.get_test_video_directory(), "basketball.mp4")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTestVideo.json")
    wf = workflow.load(wf_path)
    wf.set_video_input(path=video_path)
    wf.set_cfg_entry("WholeVideo", str(int(True)))
    wf.run()
    logger.warning("Workflow finished successfully")

    # Image folder
    logger.warning("Run on image folder...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    wf.set_auto_save(True)
    dir_path = tests.get_test_image_directory()
    wf.set_directory_input(dir_path)
    wf.run()
    logger.warning("Workflow finished successfully")

    # Video folder
    logger.warning("Run on video folder...")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTestVideo.json")
    wf = workflow.load(wf_path)
    wf.set_auto_save(True)
    wf.set_cfg_entry("WholeVideo", str(int(True)))
    dir_path = tests.get_test_video_directory()
    wf.set_directory_input(dir_path)
    wf.run()
    logger.warning("Workflow finished successfully")


def test_run_on():
    logger.warning("===== Test: Workflow::run_on() =====")

    # On array
    logger.warning("Run on image array...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    img_array = np.random.randint(low=0, high=255, size=(512, 512, 3), dtype=np.uint8)
    wf.run_on(array=img_array)
    _check_leaf_outputs(wf)
    logger.warning("Workflow finished successfully")

    # On image path
    logger.warning("Run on image path...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    img_path = tests.get_test_image_directory() + "/Lena.png"
    wf.run_on(path=img_path)
    _check_leaf_outputs(wf)
    logger.warning("Workflow finished successfully")

    # On image URL
    logger.warning("Run on image URL...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    img_url = "https://helpx.adobe.com/content/dam/help/en/photoshop/using/convert-color-image-black-white/jcr_content/main-pars/before_and_after/image-before/Landscape-Color.jpg"
    wf.run_on(url=img_url)
    _check_leaf_outputs(wf)
    logger.warning("Workflow finished successfully")

    # On video path
    logger.warning("Run on video path...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestVideo.json"
    wf = workflow.load(wf_path)
    video_path = os.path.join(tests.get_test_video_directory(), "basketball.mp4")
    wf.run_on(path=video_path)
    logger.warning("Workflow finished successfully")

    # On video URL
    logger.warning("Run on video URL...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestVideo.json"
    wf = workflow.load(wf_path)
    video_url = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/ForBiggerBlazes.mp4"
    wf.run_on(url=video_url)
    logger.warning("Workflow finished successfully")

    # On image folder
    logger.warning("Run on image folder...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestVideo.json"
    wf = workflow.load(wf_path)
    wf.set_auto_save(True)
    dir_path = tests.get_test_image_directory()
    wf.run_on(folder=dir_path)
    _check_leaf_outputs(wf)
    logger.warning("Workflow finished successfully")

    # On video folder
    logger.warning("Run on video folder...")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTestVideo.json"
    wf = workflow.load(wf_path)
    wf.set_auto_save(True)
    dir_path = tests.get_test_video_directory()
    wf.run_on(folder=dir_path)
    logger.warning("Workflow finished successfully")


def test_add_task():
    logger.warning("===== Test: Workflow::add_task() =====")

    # From name
    wf = workflow.create("FromScratch")
    box_filter = wf.add_task(name=ik.ocv_box_filter.name())
    assert box_filter is not None
    assert wf.get_task_count() == 2
    assert len(wf.get_parents(wf.get_task_id(box_filter))) == 0

    # From task object
    wf = workflow.create("FromScratch")
    box_filter = ikomia.ik_registry.create_algorithm(name=ik.ocv_box_filter.name())
    box_filter_added = wf.add_task(task=box_filter)
    assert box_filter_added == box_filter
    assert wf.get_task_count() == 2
    assert len(wf.get_parents(wf.get_task_id(box_filter))) == 0

    # Default parameters
    wf = workflow.create("FromScratch")
    params = {
        ik.ocv_box_filter.kSizeWidth: "19",
        ik.ocv_box_filter.kSizeHeight: "19",
    }
    box_filter = wf.add_task(name=ik.ocv_box_filter.name(), params=params)
    assert box_filter is not None
    assert wf.get_task_count() == 2
    assert len(wf.get_parents(wf.get_task_id(box_filter))) == 0
    task_params = box_filter.get_parameters()
    assert params[ik.ocv_box_filter.kSizeWidth] == task_params[ik.ocv_box_filter.kSizeWidth]
    assert params[ik.ocv_box_filter.kSizeHeight] == task_params[ik.ocv_box_filter.kSizeHeight]

    # Auto-connect
    wf = workflow.create("FromScratch")
    box_filter = wf.add_task(name=ik.ocv_box_filter.name(), auto_connect=True)
    assert box_filter is not None
    assert wf.get_task_count() == 2
    parents = wf.get_parents(wf.get_task_id(box_filter))
    assert len(parents) == 1
    assert parents[0] == wf.get_task_id(wf.root())

    canny = wf.add_task(name=ik.ocv_canny.name(), auto_connect=True)
    assert canny is not None
    assert wf.get_task_count() == 3
    parents = wf.get_parents(wf.get_task_id(canny))
    assert len(parents) == 1
    assert parents[0] == wf.get_task_id(box_filter)


def test_remove_task():
    logger.warning("===== Test: Workflow::remove_task() =====")
    wf_path = tests.get_test_workflow_directory() + "/WorkflowTest1.json"
    wf = workflow.load(wf_path)
    assert wf.get_task_count() == 10

    # From name
    wf.remove_task(name=ik.ocv_superpixel_lsc.name(), index=0)
    assert wf.get_task_count() == 9

    # From task object
    superpixel_seed = wf.find_task(name=ik.ocv_superpixel_seeds.name(), index=0)
    wf.remove_task(task=superpixel_seed)
    assert wf.get_task_count() == 8


def test_connect_tasks():
    logger.warning("===== Test: Workflow::connect_tasks() =====")

    # Auto-connection
    wf = workflow.create("FromScratch")
    box_filter = wf.add_task(name=ik.ocv_box_filter.name(), auto_connect=False)
    wf.connect_tasks(wf.root(), box_filter)
    parents = wf.get_parents(wf.get_task_id(box_filter))
    assert len(parents) == 1
    assert parents[0] == wf.get_task_id(wf.root())

    clahe = wf.add_task(name=ik.ocv_clahe.name(), auto_connect=False)
    wf.connect_tasks(box_filter, clahe)
    parents = wf.get_parents(wf.get_task_id(clahe))
    assert len(parents) == 1
    assert parents[0] == wf.get_task_id(box_filter)

    # Manual connection
    wf = workflow.create("FromScratch")
    bilateral = wf.add_task(name=ik.ocv_bilateral_filter.name())
    wf.connect_tasks(wf.root(), bilateral, [(0, 0)])
    parents = wf.get_parents(wf.get_task_id(bilateral))
    assert parents[0] == wf.get_task_id(wf.root())
    edge_id = wf.get_in_edges(wf.get_task_id(bilateral))
    assert len(edge_id) == 1
    edge_info = wf.get_edge_info(edge_id[0])
    assert edge_info[0] == 0
    assert edge_info[1] == 0

    equalize = wf.add_task(name=ik.ocv_equalize_histogram.name())
    wf.connect_tasks(bilateral, equalize, [(0, 0)])
    parents = wf.get_parents(wf.get_task_id(equalize))
    assert parents[0] == wf.get_task_id(bilateral)
    edge_id = wf.get_in_edges(wf.get_task_id(equalize))
    assert len(edge_id) == 1
    edge_info = wf.get_edge_info(edge_id[0])
    assert edge_info[0] == 0
    assert edge_info[1] == 0

    dtfilter = wf.add_task(name=ik.ocv_dt_filter.name())
    wf.connect_tasks(equalize, dtfilter, [(0, 0), (0, 1)])
    parents = wf.get_parents(wf.get_task_id(dtfilter))
    assert parents[0] == wf.get_task_id(equalize)
    edge_id = wf.get_in_edges(wf.get_task_id(dtfilter))
    assert len(edge_id) == 2
    edge_info = wf.get_edge_info(edge_id[0])
    assert edge_info[0] == 0
    assert edge_info[1] == 0
    edge_info = wf.get_edge_info(edge_id[1])
    assert edge_info[0] == 0
    assert edge_info[1] == 1


def test_time_metrics():
    logger.warning("===== Test::compute workflow time metrics =====")
    img_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTest1.json")
    wf = workflow.load(wf_path)
    wf.set_image_input(path=img_path)
    wf.run()
    logger.warning("Workflow running time (ms): " + str(wf.get_total_elapsed_time()))

    # manual retrieval
    ids = wf.get_task_ids()
    for task_id in ids:
        t = wf.get_task(task_id)
        logger.warning(f"{t.name}: {t.get_elapsed_time()} - From start: {wf.get_elapsed_time_to(task_id)}")

    # tool function
    logger.warning(wf.get_time_metrics())


def test_get_task_output():
    logger.warning("===== Test: Workflow::get_task_output() =====")
    img_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    wf_path = os.path.join(tests.get_test_workflow_directory(), "WorkflowTestOutput.json")
    wf = workflow.load(wf_path)
    wf.run_on(path=img_path)

    outs = wf.get_task_output(task_name=ik.ocv_split.name(), task_index=0, types=[IODataType.IMAGE], output_index=-1)
    assert len(outs) == 3
    outs = wf.get_task_output(task_name=ik.ocv_split.name(), types=[IODataType.IMAGE], output_index=0)
    assert not isinstance(outs, list)
    assert isinstance(outs, CImageIO)

    split = wf.find_task(name=ik.ocv_split.name(), index=0)
    outs = wf.get_task_output(task_obj=split, task_index=0, types=[IODataType.IMAGE], output_index=-1)
    assert len(outs) == 3
    outs = wf.get_task_output(task_obj=split, types=[IODataType.IMAGE], output_index=0)
    assert not isinstance(outs, list)
    assert isinstance(outs, CImageIO)

    outs = wf.get_task_output(task_name=ik.ocv_calc_hist.name(), types=[IODataType.NUMERIC_VALUES], output_index=0)
    assert isinstance(outs, CNumericIO)

    outs = wf.get_task_output(task_name=ik.infer_yolo_v8.name(), types=[IODataType.OBJECT_DETECTION], output_index=0)
    assert isinstance(outs, CObjectDetectionIO)

    outs = wf.get_task_output(task_name=ik.infer_yolo_v8_seg.name(), types=[IODataType.INSTANCE_SEGMENTATION],
                              output_index=0)
    assert isinstance(outs, CInstanceSegmentationIO)

    outs = wf.get_task_output(task_name=ik.infer_yolo_v8_pose_estimation.name(),
                              types=[IODataType.KEYPOINTS],
                              output_index=0)
    assert isinstance(outs, CKeypointsIO)

    outs = wf.get_task_output(task_name=ik.infer_hf_semantic_seg.name(), types=[IODataType.SEMANTIC_SEGMENTATION],
                              output_index=0)
    assert isinstance(outs, CSemanticSegmentationIO)

    outs = wf.get_task_output(task_name=ik.infer_torchvision_resnet.name(), types=[IODataType.OUTPUT_GRAPHICS],
                              output_index=0)
    assert isinstance(outs, CGraphicsOutput)


def test_video_stream():
    cap = cv2.VideoCapture(0)

    wf = workflow.create("Video Processing")
    clahe = wf.add_task(ik.ocv_clahe())
    wf.connect_tasks(wf.root(), clahe)
    canny = wf.add_task(ik.ocv_canny())
    wf.connect_tasks(clahe, canny)

    while True:
        ret, img = cap.read()
        wf.run_on(array=img)
        res_img = canny.get_output(0).get_image()
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

    if 'all' in running_tests or 'cpp_workflow' in running_tests:
        test_cpp_workflow()
    if 'all' in running_tests or 'ctor' in running_tests:
        test_ctor()
    if 'all' in running_tests or 'root' in running_tests:
        test_root()
    if 'all' in running_tests or 'set_image_input' in running_tests:
        test_set_image_input()
    if 'all' in running_tests or 'set_video_input' in running_tests:
        test_set_video_input()
    if 'all' in running_tests or 'set_directory_input' in running_tests:
        test_set_directory_input()
    if 'all' in running_tests or 'set_parameters' in running_tests:
        test_set_parameters()
    if 'all' in running_tests or 'load' in running_tests:
        test_load()
    if 'all' in running_tests or 'run' in running_tests:
        test_run()
    if 'all' in running_tests or 'run_on' in running_tests:
        test_run_on()
    if 'all' in running_tests or 'add_task' in running_tests:
        test_add_task()
    if 'all' in running_tests or 'remove_task' in running_tests:
        test_remove_task()
    if 'all' in running_tests or 'connect_tasks' in running_tests:
        test_connect_tasks()
    if 'all' in running_tests or 'time_metrics' in running_tests:
        test_time_metrics()
    if 'all' in running_tests or 'get_task_output' in running_tests:
        test_get_task_output()
    if 'all' in running_tests or 'test_video_stream' in running_tests:
        test_video_stream()
