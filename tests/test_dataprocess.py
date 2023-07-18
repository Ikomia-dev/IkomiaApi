import argparse
import logging
import os.path
import pytest
from ikomia import utils, core, dataprocess

logger = logging.getLogger(__name__)


def test_cpp_task_info():
    logger.warning("===== Test: CTaskInfo =====")
    info = dataprocess.CTaskInfo()

    # Print
    logger.warning(info)

    # Set attributes
    info.name = "my_algo"
    info.path = "detection/face"
    info.short_description = "Blabla"
    info.description = "BlaBlaBlaBla"
    info.documentation_link = "https://docs.com"
    info.icon_path = "images/icon.png"
    info.keywords = "pytorch,face,cnn"
    info.authors = "me"
    info.article = "The best face detector"
    info.journal = "ECCV"
    info.version = "1.2.3"
    info.year = 2023
    info.language = utils.ApiLanguage.PYTHON
    info.license = "MIT"
    info.repository = "https://github.com/facedetector"
    info.created_date = "01/01/2023"
    info.modified_date = "19/01/2023"
    info.os = utils.OSType.ALL
    info.internal = False
    logger.warning(info)


def test_cpp_object_measure():
    logger.warning("===== Test: CObjectMeasure =====")
    # Default ctor
    obj_measure = dataprocess.CObjectMeasure()
    measure = obj_measure.get_measure_info()
    assert type(measure) == core.CMeasure
    assert len(obj_measure.values) == 0
    assert obj_measure.graphics_id == 0
    assert obj_measure.label == ""

    # Ctor with specific measure and value
    measure = core.CMeasure(core.MeasureId.SURFACE)
    value = 18.6
    graphics_id = 1001
    label = "Bone surface"
    obj_measure = dataprocess.CObjectMeasure(measure, value, graphics_id, label)
    measure = obj_measure.get_measure_info()
    assert measure.id == core.MeasureId.SURFACE
    assert obj_measure.values[0] == value
    assert obj_measure.graphics_id == graphics_id
    assert obj_measure.label == label

    # Ctor with specific measure and values
    measure = core.CMeasure(core.MeasureId.BBOX)
    values = [0, 0, 125, 215]
    graphics_id = 1002
    label = "Object box"
    obj_measure = dataprocess.CObjectMeasure(measure, values, graphics_id, label)
    measure = obj_measure.get_measure_info()
    assert measure.id == core.MeasureId.BBOX
    assert len(obj_measure.values) == 4
    assert obj_measure.graphics_id == graphics_id
    assert obj_measure.label == label


def test_cpp_enums():
    logger.warning("===== Test: enums =====")
    # NumericOutputType
    out_type = dataprocess.NumericOutputType.NONE
    out_type = dataprocess.NumericOutputType.TABLE
    out_type = dataprocess.NumericOutputType.PLOT
    out_type = dataprocess.NumericOutputType.NUMERIC_FILE

    # PlotType
    plot_type = dataprocess.PlotType.CURVE
    plot_type = dataprocess.PlotType.BAR
    plot_type = dataprocess.PlotType.MULTIBAR
    plot_type = dataprocess.PlotType.HISTOGRAM
    plot_type = dataprocess.PlotType.PIE


def test_cpp_registry():
    logger.warning("===== Test: CIkomiaRegistry =====")
    registry = dataprocess.CIkomiaRegistry()

    # Plugins directory
    default_dir = registry.get_plugins_directory()
    registry.set_plugins_directory(os.path.curdir)
    new_dir = registry.get_plugins_directory()
    assert new_dir == os.path.curdir
    registry.set_plugins_directory(default_dir)
    assert registry.get_plugins_directory() == default_dir

    # Get local algorithms
    algos = registry.get_algorithms()
    assert len(algos) > 0
    info = registry.get_algorithm_info(algos[0])
    logger.warning(f"Algorithm info: {info}")

    with pytest.raises(RuntimeError):
        info = registry.get_algorithm_info("toto")

    # Algorithm instantiation
    t = registry.create_instance(algos[0])
    assert t is not None
    logger.warning(f"Algorithm instance: {t}")
    param_obj = t.get_param_object()
    t = registry.create_instance(algos[0], param_obj)
    assert t is not None

    # Register new task and io
    # TODO

    # Load C++ plugins
    plugin_path = os.path.join(default_dir, "C++", "infer_yolo_v3")
    registry.load_cpp_algorithm(plugin_path)

    # Black-listed dependency package (static)
    blacklist = dataprocess.CIkomiaRegistry.get_black_listed_packages()
    logger.warning(f"Package black list: {blacklist}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if "all" in running_tests or "cpp_task_info" in running_tests:
        test_cpp_task_info()
    if "all" in running_tests or "cpp_object_measure" in running_tests:
        test_cpp_object_measure()
    if "all" in running_tests or "cpp_enums" in running_tests:
        test_cpp_enums()
    if "all" in running_tests or "cpp_registry" in running_tests:
        test_cpp_registry()
