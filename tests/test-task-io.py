import os
import argparse
import logging
import numpy as np
from ikomia import core, dataio, dataprocess
from ikomia.utils import tests

logger = logging.getLogger(__name__)


def _create_graphics_list():
    graphics = []
    pt = core.CGraphicsPoint(core.CPointF(150, 80))
    graphics.append(pt)

    ellipse = core.CGraphicsEllipse(0, 0, 100, 50)
    graphics.append(ellipse)

    rect = core.CGraphicsRectangle(0, 60, 100, 50)
    graphics.append(rect)

    polyline = core.CGraphicsPolyline([core.CPointF(0, 0),
                                       core.CPointF(30, 30),
                                       core.CPointF(100, 30),
                                       core.CPointF(130, 100)])
    graphics.append(polyline)

    polygon = core.CGraphicsPolygon([core.CPointF(50, 10),
                                     core.CPointF(100, 100),
                                     core.CPointF(80, 150),
                                     core.CPointF(20, 120),
                                     core.CPointF(20, 60)])
    graphics.append(polygon)

    text = core.CGraphicsText("This is a text", 200, 10)
    graphics.append(text)

    complex_polygon = core.CGraphicsComplexPolygon(
        [
            core.CPointF(250, 10),
            core.CPointF(300, 100),
            core.CPointF(280, 150),
            core.CPointF(220, 120),
            core.CPointF(220, 60)],
        [
            [core.CPointF(250, 100), core.CPointF(270, 70), core.CPointF(240, 40)]
        ]
    )
    graphics.append(complex_polygon)
    return graphics


def test_cpp_base_class():
    logger.info("===== Test: I/O base class =====")
    # Default ctor
    io = core.CWorkflowTaskIO()
    logger.info(f"I/O name: {io.name}")
    logger.info(f"I/O data type: {io.data_type}")
    elt_count = io.get_unit_element_count()
    assert type(elt_count) == int
    data_available = io.is_data_available()
    assert data_available is False
    composite = io.is_composite()
    assert composite is False
    assert io.displayable is True
    assert io.auto_save is False
    io.clear_data()
    json_data = io.to_json()
    assert len(json_data) == 0

    # Ctor with data type
    io1 = core.CWorkflowTaskIO(core.IODataType.IMAGE)
    assert io1.data_type == core.IODataType.IMAGE
    io1.copy_static_data(io)

    # Ctor with data type and name
    io_name = "Mask input"
    io = core.CWorkflowTaskIO(core.IODataType.IMAGE_BINARY, io_name)
    assert io.data_type == core.IODataType.IMAGE_BINARY
    assert io.name == io_name

    # Attributes
    io.description = "My description"
    logger.info(f"I/O description: {io.description}")
    io.dim_count = 3
    logger.info(f"I/O dim count: {io.dim_count}")
    logger.info(f"I/O source path: {io.source_file_path}")

    # Print
    logger.info(io)


def test_cpp_blob_measure():
    logger.info("===== Test: CBlobMeasureIO =====")
    # Default ctor
    io = dataprocess.CBlobMeasureIO()
    assert not io.is_data_available()

    # Ctor with name
    name = "Object measures"
    io = dataprocess.CBlobMeasureIO(name)
    assert io.name == name

    # Add measures
    measure = dataprocess.CObjectMeasure(core.CMeasure(core.MeasureId.SURFACE), 18.6, 1001, "Bone surface")
    io.add_object_measure(measure)
    assert io.is_data_available()

    measures = []
    for i in range(5):
        measures.append(measure)

    io.add_object_measures(measures)
    io.set_object_measure(0, dataprocess.CObjectMeasure(core.CMeasure(core.MeasureId.ELONGATION), 0.0, 1, "Form"))

    # Get measures
    inserted_measures = io.get_measures()
    assert len(inserted_measures) == 2
    assert inserted_measures[0][0].get_measure_info().id == core.MeasureId.ELONGATION
    assert len(inserted_measures[1]) == 5

    # Export
    json_data = io.to_json()
    logger.info(json_data)
    path = os.path.join(tests.get_test_image_directory(), "blob.csv")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CBlobMeasureIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CBlobMeasureIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_graphics_in():
    logger.info("===== Test: CGraphicsInput =====")

    # Default ctor
    io = dataprocess.CGraphicsInput()
    assert not io.is_data_available()

    # Ctor with name
    name = "Graphics input"
    io = dataprocess.CGraphicsInput(name)
    assert io.name == name

    # Set items
    items = _create_graphics_list()
    io.set_items(items)
    assert io.is_data_available()

    # Get items
    inserted_items = io.get_items()
    assert len(inserted_items) == len(items)

    # Export
    json_data = io.to_json()
    logger.info(json_data)
    path = os.path.join(tests.get_test_image_directory(), "graphics.json")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CGraphicsInput()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CGraphicsInput()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_graphics_out():
    logger.info("===== Test: CGraphicsOutput =====")

    # Default ctor
    io = dataprocess.CGraphicsOutput()
    assert not io.is_data_available()

    # Ctor with name
    name = "Graphics input"
    io = dataprocess.CGraphicsOutput(name)
    assert io.name == name

    # Associated image output index
    io.set_image_index(1)
    assert io.get_image_index() == 1

    # Set items
    items = _create_graphics_list()
    io.set_items(items)
    assert io.is_data_available()

    # Get items
    inserted_items = io.get_items()
    assert len(inserted_items) == len(items)

    # Add specific items
    io.set_new_layer("NewLayer")
    assert not io.is_data_available()

    io.add_item(core.CGraphicsPoint(core.CPointF(150, 80)))
    io.add_point(core.CPointF(150, 100))
    io.add_point(core.CPointF(150, 150), core.GraphicsPointProperty())
    io.add_rectangle(0.0, 0.0, 200.0, 150.0)
    io.add_rectangle(0.0, 0.0, 120, 120, core.GraphicsRectProperty())
    io.add_ellipse(0.0, 0.0, 200.0, 150.0)
    io.add_ellipse(0.0, 0.0, 200.0, 150.0, core.GraphicsEllipseProperty())
    points = [core.CPointF(50, 10),
              core.CPointF(100, 100),
              core.CPointF(80, 150),
              core.CPointF(20, 120),
              core.CPointF(20, 60)]
    io.add_polygon(points)
    io.add_polygon(points, core.GraphicsPolygonProperty())
    io.add_polyline(points)
    io.add_polyline(points, core.GraphicsPolylineProperty())
    inners = [
        [core.CPointF(250, 100), core.CPointF(270, 70), core.CPointF(240, 40)],
        [core.CPointF(50, 100), core.CPointF(170, 70), core.CPointF(40, 40)]
    ]
    io.add_complex_polygon(points, inners)
    io.add_complex_polygon(points, inners, core.GraphicsPolygonProperty())
    io.add_text("Title", 20, 50)
    io.add_text("Title", 20, 50, core.GraphicsTextProperty())
    assert len(io.get_items()) == 15

    # Export
    json_data = io.to_json()
    logger.info(json_data)
    path = os.path.join(tests.get_test_image_directory(), "graphics.json")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CGraphicsOutput()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CGraphicsOutput()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_image_io():
    logger.info("===== Test: CImageIO =====")
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()

    # Default ctor
    io = dataprocess.CImageIO()
    assert not io.is_data_available()
    assert not io.is_overlay_available()

    # Ctor with data type
    io = dataprocess.CImageIO(core.IODataType.IMAGE_BINARY)
    assert io.data_type == core.IODataType.IMAGE_BINARY

    # Ctor with data type and name
    io = dataprocess.CImageIO(core.IODataType.IMAGE, "Source image")
    assert io.data_type == core.IODataType.IMAGE
    assert io.name == "Source image"

    # Ctor with data type, name and image path
    io = dataprocess.CImageIO(core.IODataType.IMAGE, "Source image", image_path)
    assert io.data_type == core.IODataType.IMAGE
    assert io.name == "Source image"
    assert io.get_image() is not None

    # Ctor with data type and image
    io = dataprocess.CImageIO(core.IODataType.IMAGE, image_data)
    assert io.data_type == core.IODataType.IMAGE
    assert io.get_image() is not None

    # Ctor with data type, image and name
    io = dataprocess.CImageIO(core.IODataType.IMAGE, image_data, "Source image")
    assert io.data_type == core.IODataType.IMAGE
    assert io.get_image() is not None
    assert io.name == "Source image"

    # Set image
    io = dataprocess.CImageIO()
    io.set_image(image_data)
    assert io.get_image() is not None
    assert io.get_data() is not None
    assert io.get_channel_count() == 3
    assert io.get_unit_element_count() == 1
    assert io.is_data_available()

    # Set overlay mask
    h, w, c = io.get_image().shape
    mask = np.ones((h, w))
    io.set_overlay_mask(mask)
    assert io.get_overlay_mask() is not None
    assert io.is_overlay_available()

    # Graphics
    items = _create_graphics_list()
    graphics_in = dataprocess.CGraphicsInput()
    graphics_in.set_items(items)
    assert io.get_image_with_graphics(graphics_in) is not None

    graphics_out = dataprocess.CGraphicsOutput()
    graphics_out.set_items(items)
    assert io.get_image_with_graphics(graphics_out) is not None

    io.draw_graphics(graphics_in)
    io.draw_graphics(graphics_out)

    # Export
    json_data = io.to_json()
    path = os.path.join(tests.get_test_image_directory(), "test_image.png")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CImageIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    assert loaded_io.get_image() is not None
    os.remove(path)
    loaded_io = dataprocess.CImageIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()
    assert loaded_io.get_image() is not None

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_numeric_io():
    logger.info("===== Test: CNumericIO =====")

    # Default ctor
    io = dataprocess.CNumericIO()
    assert not io.is_data_available()
    assert io.get_output_type() == dataprocess.NumericOutputType.TABLE
    assert io.get_plot_type() == dataprocess.PlotType.CURVE

    # Ctor with name
    name = "Output values"
    io = dataprocess.CNumericIO(name)
    assert io.name == name

    # Output type
    io.set_output_type(dataprocess.NumericOutputType.PLOT)
    assert io.get_output_type() == dataprocess.NumericOutputType.PLOT

    # Plot type
    io.set_plot_type(dataprocess.PlotType.HISTOGRAM)
    assert io.get_plot_type() == dataprocess.PlotType.HISTOGRAM

    # Add values
    values = [1.0, 2.0, 3.0, 4.0, 5.0]
    io.add_value_list(values)
    assert len(io.get_value_list(0)) == len(values)
    io.add_value_list(values, "Measure")
    assert len(io.get_all_value_list()) == 2
    assert io.get_all_header_labels()[0] == "Measure"
    labels = ["obj1", "obj2", "obj3", "obj4", "obj5"]
    io.add_value_list(values, labels)
    assert len(io.get_all_value_list()) == 3
    assert len(io.get_all_label_list()) == 1
    assert len(io.get_all_label_list()[0]) == 5
    io.add_value_list(values, "Surface", labels)
    assert len(io.get_all_value_list()) == 4
    assert len(io.get_all_label_list()) == 2
    assert io.get_all_header_labels()[1] == "Surface"
    assert io.is_data_available()

    # Export
    json_data = io.to_json()
    path = os.path.join(tests.get_test_image_directory(), "test_numeric.csv")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CNumericIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CNumericIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_data_string_io():
    logger.info("===== Test: CNumericIO =====")

    # Default ctor
    io = dataprocess.CDataStringIO()
    assert not io.is_data_available()
    assert io.get_output_type() == dataprocess.NumericOutputType.TABLE
    assert io.get_plot_type() == dataprocess.PlotType.CURVE

    # Ctor with name
    name = "Output values"
    io = dataprocess.CDataStringIO(name)
    assert io.name == name

    # Output type
    io.set_output_type(dataprocess.NumericOutputType.PLOT)
    assert io.get_output_type() == dataprocess.NumericOutputType.PLOT

    # Plot type
    io.set_plot_type(dataprocess.PlotType.HISTOGRAM)
    assert io.get_plot_type() == dataprocess.PlotType.HISTOGRAM

    # Add values
    values = ["Pierre", "Paul", "Jacques", "Edith", "Simone"]
    io.add_value_list(values)
    assert len(io.get_value_list(0)) == len(values)
    io.add_value_list(values, "Name")
    assert len(io.get_all_value_list()) == 2
    assert io.get_all_header_labels()[0] == "Name"
    labels = ["person1", "person2", "person3", "person4", "person5"]
    io.add_value_list(values, labels)
    assert len(io.get_all_value_list()) == 3
    assert len(io.get_all_label_list()) == 1
    assert len(io.get_all_label_list()[0]) == 5
    io.add_value_list(values, "Surname", labels)
    assert len(io.get_all_value_list()) == 4
    assert len(io.get_all_label_list()) == 2
    assert io.get_all_header_labels()[1] == "Surname"
    assert io.is_data_available()

    # Export
    json_data = io.to_json()
    path = os.path.join(tests.get_test_image_directory(), "test_numeric.csv")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CDataStringIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CDataStringIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_video_io():
    logger.info("===== Test: CVideoIO =====")
    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    video_path = os.path.join(tests.get_test_video_directory(), "basketball.mp4")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()
    name = "Sport video"

    # Default ctor
    io = dataprocess.CVideoIO()
    assert not io.is_data_available()

    # Ctor with data type
    io = dataprocess.CVideoIO(core.IODataType.VIDEO_LABEL)
    assert io.data_type == core.IODataType.VIDEO_LABEL

    # Ctor with data type and name
    io = dataprocess.CVideoIO(core.IODataType.VIDEO_LABEL, name)
    assert io.data_type == core.IODataType.VIDEO_LABEL
    assert io.name == name

    # Ctor with data type and frame
    io = dataprocess.CVideoIO(core.IODataType.VIDEO, image_data)
    assert io.data_type == core.IODataType.VIDEO
    assert io.is_data_available()

    # Ctor with data type, frame and name
    io = dataprocess.CVideoIO(core.IODataType.VIDEO, image_data, name)
    assert io.data_type == core.IODataType.VIDEO
    assert io.is_data_available()
    assert io.name == name

    # Ctor with data type, name and path
    io = dataprocess.CVideoIO(core.IODataType.VIDEO, name, video_path)
    assert io.data_type == core.IODataType.VIDEO
    assert not io.is_data_available()
    assert io.name == name
    assert io.has_video()
    assert io.get_video_path() == video_path
    logger.info(f"Video frame count: {io.get_video_frame_count()}")
    assert io.get_current_pos() == 0
    frame = io.get_snapshot(io.get_current_pos())
    assert frame is not None
    h, w, _ = frame.shape
    logger.info(f"Video frame size: {frame.shape}")

    # Set video pos
    io.set_video_pos(25)
    assert io.get_current_pos() == 25
    frame = io.get_snapshot(io.get_current_pos())
    assert frame is not None

    # Snapshot
    frame = io.get_snapshot(100)
    assert frame is not None
    assert io.get_current_pos() == 100 + 1

    # Read - write
    save_video_path = os.path.join(tests.get_test_video_directory(), "test.mp4")
    io_to = dataprocess.CVideoIO(core.IODataType.VIDEO, name, save_video_path)
    io.set_video_pos(0)
    io.start_video(1000)
    io_to.start_video_write(w, h, 50, 25, -1, 5000)

    for i in range(50):
        io_to.write_image(io.get_image())

    io.stop_video()
    io_to.stop_video_write(5000)
    assert os.path.isfile(save_video_path)
    os.remove(save_video_path)

    # Clear (just the last frame read -> video manager is not closed so is_data_available is still true)
    io.clear_data()

    # Images sequence
    io.add_video_image(image_data)
    io.add_video_image(image_data)
    assert len(io.get_video_images()) == 2


def test_cpp_path_io():
    logger.info("===== Test: CPathIO =====")
    # Default ctor
    io = dataprocess.CPathIO()
    assert not io.is_data_available()
    assert io.data_type == dataprocess.IODataType.FILE_PATH

    # Ctor with data type
    io = dataprocess.CPathIO(dataprocess.IODataType.FOLDER_PATH)
    assert not io.is_data_available()
    assert io.data_type == dataprocess.IODataType.FOLDER_PATH

    # Ctor with data type and path
    path = tests.get_test_image_directory()
    io = dataprocess.CPathIO(dataprocess.IODataType.FOLDER_PATH, path)
    assert io.data_type == dataprocess.IODataType.FOLDER_PATH
    assert io.is_data_available()
    assert io.get_path() == path

    # Ctor with data type, path and name
    name = "Dataset folder"
    io = dataprocess.CPathIO(dataprocess.IODataType.FOLDER_PATH, path, name)
    assert io.data_type == dataprocess.IODataType.FOLDER_PATH
    assert io.is_data_available()
    assert io.get_path() == path
    assert io.name == name

    # Set/get path
    path = tests.get_test_video_directory()
    io.set_path(path)
    assert io.get_path() ==  path

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_dataset_io():
    logger.info("===== Test: CDatasetIO =====")
    # Default ctor
    io = dataprocess.CDatasetIO()
    assert not io.is_data_available()

    # Ctor with name
    name = "COCO"
    io = dataprocess.CDatasetIO(name)
    assert not io.is_data_available()
    assert io.name == name

    # Ctor with name and source format
    name = "YOLO"
    dataset_format = "yolo_format"
    io = dataprocess.CDatasetIO(name, dataset_format)
    assert not io.is_data_available()
    assert io.name == name
    assert io.get_source_format() == dataset_format

    # Methods to override
    assert len(io.get_image_paths()) == 0
    assert len(io.get_categories()) == 0
    assert io.get_category_count() == 0
    assert io.get_mask_path("") == ""
    assert len(io.get_graphics_annotations("")) == 0
    assert io.to_json() == ""
    io.from_json("")
    # io.save()
    # io.load()
    io.clear_data()


def test_cpp_array_io():
    logger.info("===== Test: CArrayIO =====")
    # Default ctor
    io = dataprocess.CArrayIO()
    assert not io.is_data_available()

    # Ctor with name
    name = "Optical flow"
    io = dataprocess.CArrayIO(name)
    assert not io.is_data_available()
    assert io.name == name

    # Ctor with name and array
    dims = (768, 1024, 1)
    flow = np.ones(dims)
    io = dataprocess.CArrayIO(flow, name)
    assert io.is_data_available()
    assert io.name == name
    assert io.get_array().shape == dims

    # Set array
    dims = (1024, 1024, 1)
    flow = np.ones(dims)
    io.set_array(flow)
    assert io.is_data_available()
    assert io.get_array().shape == dims

    # Clear
    io.clear_data()
    assert not io.is_data_available()


def test_cpp_object_detection_io():
    logger.info("===== Test: CObjectDetectionIO =====")
    io = dataprocess.CObjectDetectionIO()
    assert not io.is_data_available()

    # Add objects
    io.add_object(0, "Car", 0.96, 0, 0, 65, 56, [255, 0, 100, 255])
    io.add_object(1, "Bus", 0.99, 125, 80, 125, 200, [0, 100, 100, 255])
    assert io.is_data_available()
    assert io.get_object_count() == 2
    assert len(io.get_objects()) == 2
    obj = io.get_object(1)
    assert type(obj) == dataprocess.CObjectDetection
    assert obj.id == 1
    assert obj.label == "Bus"
    assert obj.confidence == 0.99
    assert obj.box == [125, 80, 125, 200]
    assert obj.color == [0, 100, 100, 255]
    graphics_io = io.get_graphics_io()
    assert type(graphics_io) == dataprocess.CGraphicsOutput

    # Export
    json_data = io.to_json()
    logger.info(f"Detected objects: {json_data}")
    path = os.path.join(tests.get_test_image_directory(), "test_object_detection.json")
    io.save(path)
    assert os.path.isfile(path)

    # Init
    io.init("Yolo V4", 0)
    assert not io.is_data_available()

    # Import
    loaded_io = dataprocess.CObjectDetectionIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CObjectDetectionIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    loaded_io.clear_data()
    assert not loaded_io.is_data_available()


def test_cpp_instance_segmentation_io():
    logger.info("===== Test: CInstanceSegmentationIO =====")
    io = dataprocess.CInstanceSegmentationIO()
    assert not io.is_data_available()

    # Init
    w = 1024
    h = 1024
    io.init("Mask RCNN", 0, w, h)
    assert not io.is_data_available()

    # Add instances
    x1 = 25
    y1 = 60
    w1 = 150
    h1 = 80
    mask1 = np.zeros((h, w))
    mask1[y1:y1 + h1, x1:x1 + w1] = 1
    io.add_instance(0, 0, 0, "Car", 0.86, x1, y1, w1, h1, mask1, [255, 0, 100, 255])
    x2 = 125
    y2 = 160
    w2 = 130
    h2 = 90
    mask2 = np.zeros((h, w))
    mask2[y2:y2 + h2, x2:x2 + w2] = 1
    io.add_instance(1, 0, 1, "Bus", 0.92, x2, y2, w2, h2, mask2, [100, 0, 255, 255])

    assert io.is_data_available()
    assert io.get_instance_count() == 2
    assert len(io.get_instances()) == 2
    instance = io.get_instance(0)
    assert type(instance) == dataprocess.CInstanceSegmentation
    assert instance.id == 0
    assert instance.type == 0
    assert instance.class_index == 0
    assert instance.label == "Car"
    assert instance.confidence == 0.86
    assert instance.box == [x1, y1, w1, h1]
    assert instance.mask is not None
    logger.info(f"Mask shape: {instance.mask.shape}")
    assert instance.color == [255, 0, 100, 255]
    assert io.get_merge_mask() is not None

    # Export
    json_data = io.to_json()
    path = os.path.join(tests.get_test_image_directory(), "test_instance_segmentation.json")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CInstanceSegmentationIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CInstanceSegmentationIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    loaded_io.clear_data()
    assert not loaded_io.is_data_available()


def test_cpp_semantic_segmentation_io():
    logger.info("===== Test: CSemanticSegmentationIO =====")
    io = dataprocess.CSemanticSegmentationIO()
    assert not io.is_data_available()

    # Set mask
    x = 25
    y = 60
    w = 150
    h = 325
    mask = np.zeros((1024, 1024), dtype=np.uint8)
    mask[y:y + h, x:x + w] = 1
    io.set_mask(mask)
    assert io.is_data_available()
    m = io.get_mask()
    assert m is not None
    assert m.shape == mask.shape

    # Set classes and colors
    classes = ["sky", "mountain"]
    colors = [[255, 0, 100], [0, 100, 255]]
    io.set_class_names(classes)
    io.set_class_colors(colors)
    assert io.get_class_names() == classes
    assert io.get_colors() == colors

    # Export
    json_data = io.to_json()
    path = os.path.join(tests.get_test_image_directory(), "test_semantic_segmentation.json")
    io.save(path)
    assert os.path.isfile(path)

    # Import
    loaded_io = dataprocess.CSemanticSegmentationIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CSemanticSegmentationIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    loaded_io.clear_data()
    assert not loaded_io.is_data_available()


def test_cpp_text_io():
    logger.info("===== Test: CTextIO =====")
    io = dataprocess.CTextIO()
    assert not io.is_data_available()

    # Add objects
    io.add_text_field(0, "Name", "Mickael Jordan", 0.96, 0, 0, 65, 56, [255, 0, 100, 255])
    polygon = [core.CPointF(0, 0), core.CPointF(30, 30), core.CPointF(100, 30), core.CPointF(130, 100)]
    io.add_text_field(1, "Name", "Magic Johnson", 0.99, polygon, [0, 100, 100, 255])
    assert io.is_data_available()
    assert io.get_text_field_count() == 2
    assert len(io.get_text_fields()) == 2
    obj = io.get_text_field(1)
    assert type(obj) == dataprocess.CTextField
    assert obj.id == 1
    assert obj.label == "Name"
    assert obj.text == "Magic Johnson"
    assert obj.confidence == 0.99
    assert len(obj.polygon) == len(polygon)
    assert obj.color == [0, 100, 100, 255]
    graphics_io = io.get_graphics_io()
    assert type(graphics_io) == dataprocess.CGraphicsOutput

    # Export
    json_data = io.to_json()
    logger.info(f"Detected text fields: {json_data}")
    path = os.path.join(tests.get_test_image_directory(), "test_text_field_detection.json")
    io.save(path)
    assert os.path.isfile(path)

    # Init
    io.init("MMOcr", 0)
    assert not io.is_data_available()

    # Import
    loaded_io = dataprocess.CTextIO()
    loaded_io.load(path)
    assert loaded_io.is_data_available()
    os.remove(path)
    loaded_io = dataprocess.CTextIO()
    loaded_io.from_json(json_data)
    assert loaded_io.is_data_available()

    # Clear
    loaded_io.clear_data()
    assert not loaded_io.is_data_available()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'cpp_base_class' in running_tests:
        test_cpp_base_class()
    if 'all' in running_tests or 'cpp_blob_measure' in running_tests:
        test_cpp_blob_measure()
    if 'all' in running_tests or 'cpp_graphics_in' in running_tests:
        test_cpp_graphics_in()
    if 'all' in running_tests or 'cpp_graphics_out' in running_tests:
        test_cpp_graphics_out()
    if 'all' in running_tests or 'cpp_image_io' in running_tests:
        test_cpp_image_io()
    if 'all' in running_tests or 'cpp_numeric_io' in running_tests:
        test_cpp_numeric_io()
    if 'all' in running_tests or 'cpp_data_string_io' in running_tests:
        test_cpp_data_string_io()
    if 'all' in running_tests or 'cpp_video_io' in running_tests:
        test_cpp_video_io()
    if 'all' in running_tests or 'cpp_path_io' in running_tests:
        test_cpp_path_io()
    if 'all' in running_tests or 'cpp_dataset_io' in running_tests:
        test_cpp_dataset_io()
    if 'all' in running_tests or 'cpp_array_io' in running_tests:
        test_cpp_array_io()
    if 'all' in running_tests or 'cpp_object_detection_io' in running_tests:
        test_cpp_object_detection_io()
    if 'all' in running_tests or 'cpp_instance_segmentation_io' in running_tests:
        test_cpp_instance_segmentation_io()
    if 'all' in running_tests or 'cpp_semantic_segmentation_io' in running_tests:
        test_cpp_semantic_segmentation_io()
    if 'all' in running_tests or 'cpp_text_io' in running_tests:
        test_cpp_text_io()
