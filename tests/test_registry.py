import logging
import pytest
import argparse
import ikomia
import cv2
from ikomia.utils import tests, ik

logger = logging.getLogger(__name__)


def test_get_local_algorithms():
    logger.info("===== Test::get list of local algorithms =====")
    algos = ikomia.ik_registry.get_algorithms()
    logger.warning("Number of algorithms:" + str(len(algos)))
    logger.warning(algos)


def test_get_online_algorithms():
    logger.info("===== Test::get list of online algorithms =====")
    algos = ikomia.ik_registry.get_online_algorithms()
    logger.warning("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.warning(algo["name"])


def test_download_plugin():
    logger.warning("===== Test::download online algorithm =====")
    logger.warning("Downloading RAFTOpticalFlow...")
    ikomia.ik_registry._download_algorithm(ik.infer_raft_optical_flow().name)


def test_install_plugin():
    logger.warning("===== Test::install online algorithms =====")
    logger.warning("Installing RAFTOpticalFlow...")
    ikomia.ik_registry.install_algorithm(ik.infer_raft_optical_flow())


def test_local_instantiation():
    logger.info("===== Test::instanciate local algorithms =====")
    algo_names = ikomia.ik_registry.get_algorithms()
    algos = []

    for name in algo_names:
        logger.warning(name)
        algo = ikomia.ik_registry.create_instance(name)
        algos.append(algo)

    with pytest.raises(RuntimeError):
        logger.warning("Try running algorithm (error expected):" + algos[0].name)
        algos[0].run()


def test_instantiation():
    logger.warning("===== Test::instanciate online algorithms =====")
    # Local C++ algo
    logger.warning("Instantiate CLAHE algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_clahe().name)
    assert(algo is not None)
    logger.warning("Done.")
    # Online Python algo
    logger.warning("Instantiate ResNet Train algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.train_torchvision_resnet().name)
    assert (algo is not None)
    logger.warning("Done.")
    # Online C++ algo
    # logger.info("Instantiate MobileNet SSD algorithm...")
    # algo = ikomia.ik_registry.create_algorithm(ik.infer_mobilenet_ssd)
    # assert (algo is not None)
    # logger.info("Done.")
    # Local Python algo
    logger.warning("Instantiate RAFTOpticalFlow algorithm...")
    ikomia.ik_registry.install_algorithm(ik.infer_raft_optical_flow().name)
    algo = ikomia.ik_registry.create_algorithm(ik.infer_raft_optical_flow().name, hub=False)
    assert (algo is not None)
    logger.warning("Done.")


def test_update():
    logger.warning("===== Test::update algorithms =====")
    logger.warning("Updating ResNet Train algorithm...")
    ikomia.ik_registry.update_algorithm(ik.train_torchvision_resnet)
    logger.warning("Updating all algorithms...")
    ikomia.ik_registry.update_algorithms()


def test_execution():
    logger.info("===== Test::execute local and online algorithms =====")

    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    assert img is not None

    # OpenCV CLAHE (C++)
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_clahe().name)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # OpenCV Canny (C++)
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_canny().name)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # Ikomia HUB scikit_threshold (Python)
    algo = ikomia.ik_registry.create_algorithm(ik.skimage_threshold().name)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # Ikomia HUB infer_yolov8 (Python)
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = ikomia.ik_registry.create_algorithm(ik.infer_yolo_v8().name)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    ikomia.authenticate()

    if 'all' in running_tests or 'get_local_algorithms' in running_tests:
        test_get_local_algorithms()
    if 'all' in running_tests or 'get_online_algorithms' in running_tests:
        test_get_online_algorithms()
    if 'all' in running_tests or 'get_download_plugin' in running_tests:
        test_download_plugin()
    if 'all' in running_tests or 'install_plugin' in running_tests:
        test_install_plugin()
    if 'all' in running_tests or 'local_instantiation' in running_tests:
        test_local_instantiation()
    if 'all' in running_tests or 'instantiation' in running_tests:
        test_instantiation()
    if 'all' in running_tests or 'update' in running_tests:
        test_update()
    if 'all' in running_tests or 'execution' in running_tests:
        test_execution()
