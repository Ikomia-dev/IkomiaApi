import logging
import os
import shutil
import pytest
import argparse
import ikomia
import cv2
from ikomia.utils import tests

logger = logging.getLogger(__name__)
_ikomia_token = os.environ["IKOMIA_TEST_TOKEN"]


def test_get_local_algorithms():
    logger.info("===== Test::get list of local algorithms =====")
    algos = ikomia.ik_registry.get_algorithms()
    logger.warning("Number of algorithms:" + str(len(algos)))
    logger.warning(algos)


def test_get_public_hub_algorithms():
    logger.info("===== Test::get list of public online algorithms =====")
    algos = ikomia.ik_registry.get_public_hub_algorithms(force=False)
    assert algos is not None
    logger.warning("Number of public algorithms:" + str(len(algos)))

    for algo in algos:
        logger.warning(algo["name"])


def test_get_private_hub_algorithms():
    logger.info("===== Test::get list of all online algorithms =====")

    if ikomia.ik_api_session.token is None:
        # Without authentication
        with pytest.raises(PermissionError) as e:
            algos = ikomia.ik_registry.get_private_hub_algorithms()

    # With authentication
    ikomia.authenticate(token=_ikomia_token)
    algos = ikomia.ik_registry.get_private_hub_algorithms()
    assert algos is not None
    logger.warning("Number of private algorithms:" + str(len(algos)))

    for algo in algos:
        logger.warning(algo["name"])


def test_download_algorithm():
    logger.warning("===== Test::download hub algorithm =====")
    logger.warning("Downloading dataset_coco...")
    algo_name = "dataset_coco"
    algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(algo_name)
    ikomia.ik_registry._download_algorithm(algo_name, public_hub=True, private_hub=False)
    shutil.rmtree(path=algo_dir, ignore_errors=True)

    if ikomia.ik_api_session.token is None:
        with pytest.raises(RuntimeError):
            ikomia.ik_registry._download_algorithm(algo_name, public_hub=False, private_hub=True)

    ikomia.authenticate(token=_ikomia_token)
    ikomia.ik_registry._download_algorithm(algo_name, public_hub=False, private_hub=True)
    shutil.rmtree(path=algo_dir, ignore_errors=True)

    ikomia.ik_registry._download_algorithm(algo_name, public_hub=True, private_hub=True)
    shutil.rmtree(path=algo_dir, ignore_errors=True)


def test_install_public_algorithm():
    logger.warning("===== Test::install online algorithms =====")
    logger.warning("Installing dataset_coco...")
    algo_name = "dataset_coco"
    ikomia.ik_registry.install_algorithm(algo_name, public_hub=True, private_hub=False)
    local_algos = ikomia.ik_registry.get_algorithms()
    assert algo_name in local_algos

    algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(algo_name)
    shutil.rmtree(path=algo_dir, ignore_errors=True)


def test_install_private_algorithm():
    logger.warning("===== Test::install online algorithms =====")
    logger.warning("Installing dataset_yolo...")
    algo_name = "dataset_yolo"

    if ikomia.ik_api_session.token is None:
        with pytest.raises(RuntimeError):
            ikomia.ik_registry.install_algorithm(algo_name, public_hub=False, private_hub=True)

    ikomia.authenticate(token=_ikomia_token)
    ikomia.ik_registry.install_algorithm(algo_name, public_hub=False, private_hub=True)
    local_algos = ikomia.ik_registry.get_algorithms()
    assert algo_name in local_algos

    algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(algo_name)
    shutil.rmtree(path=algo_dir, ignore_errors=True)


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
    logger.warning("===== Test::instantiate online algorithms =====")
    # Local C++ algo
    algo_name = "ocv_clahe"
    logger.warning(f"Instantiate {algo_name} algorithm...")
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=False)
    assert(algo is not None)

    # Public Python algo
    algo_name = "dataset_coco"
    logger.warning(f"Instantiate public {algo_name} algorithm...")
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=True, private_hub=False)
    assert (algo is not None)
    algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(algo_name)
    shutil.rmtree(path=algo_dir, ignore_errors=True)

    # Private Python algo
    algo_name = "dataset_yolo"
    logger.warning(f"Instantiate private {algo_name} algorithm...")

    if ikomia.ik_api_session.token is None:
        with pytest.raises(RuntimeError):
            algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=True)

    ikomia.authenticate(token=_ikomia_token)
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=True)
    assert (algo is not None)

    # Public C++ algo
    # algo_name = "infer_facemark_lbf"
    # logger.info("Instantiate public {algo_name} algorithm...")
    # algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=True, private_hub=False)
    # assert (algo is not None)

    # Private C++ algo
    # algo_name = "infer_facemark_lbf"
    # logger.info("Instantiate private {algo_name} algorithm...")
    # algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=True)
    # assert (algo is not None)

    # Local Python algo
    algo_name = "dataset_yolo"
    logger.warning(f"Instantiate local {algo_name} algorithm...")
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=False)
    assert (algo is not None)

    algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(algo_name)
    shutil.rmtree(path=algo_dir, ignore_errors=True)


def test_update():
    logger.warning("===== Test::update algorithms =====")
    algo_names = ["dataset_coco", "dataset_yolo"]
    logger.warning(f"Updating not installed {algo_names[0]} algorithm...")
    ikomia.ik_registry.update_algorithm(algo_names[0])

    logger.warning(f"Updating public {algo_names[0]} algorithm...")
    ikomia.ik_registry.install_algorithm(name=algo_names[0], public_hub=True, private_hub=False)
    ikomia.ik_registry.update_algorithm(algo_names[0])

    logger.warning(f"Updating private {algo_names[1]} algorithm...")
    ikomia.authenticate(token=_ikomia_token)
    ikomia.ik_registry.install_algorithm(name=algo_names[1], public_hub=False, private_hub=True)
    ikomia.ik_registry.update_algorithm(algo_names[1], public_hub=False, private_hub=True)

    logger.warning("Updating all algorithms...")
    ikomia.ik_registry.update_algorithms(public_hub=True, private_hub=True)

    for name in algo_names:
        algo_dir, _ = ikomia.ik_registry._get_algorithm_directory(name)
        shutil.rmtree(path=algo_dir, ignore_errors=True)


def test_execution():
    logger.info("===== Test::execute local and online algorithms =====")

    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    assert img is not None

    # OpenCV CLAHE (C++)
    algo_name = "ocv_clahe"
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=False)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # OpenCV Canny (C++)
    algo_name = "ocv_canny"
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=False, private_hub=False)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # Ikomia HUB scikit_threshold (Python)
    algo_name = "skimage_threshold"
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=True, private_hub=False)
    input_img = algo.get_input(0)
    input_img.set_image(img)
    algo.run()
    output_img = algo.get_output(0)
    assert output_img.is_data_available()

    # Ikomia HUB infer_yolov8 (Python)
    algo_name = "infer_yolo_v8"
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = ikomia.ik_registry.create_algorithm(algo_name, public_hub=True, private_hub=False)
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

    if 'all' in running_tests or 'get_local_algorithms' in running_tests:
        test_get_local_algorithms()
    if 'all' in running_tests or 'get_public_hub_algorithms' in running_tests:
        test_get_public_hub_algorithms()
    if 'all' in running_tests or 'get_private_hub_algorithms' in running_tests:
        test_get_private_hub_algorithms()
    if 'all' in running_tests or 'download_algorithm' in running_tests:
        test_download_algorithm()
    if 'all' in running_tests or 'install_public_algorithm' in running_tests:
        test_install_public_algorithm()
    if 'all' in running_tests or 'install_private_algorithm' in running_tests:
        test_install_private_algorithm()
    if 'all' in running_tests or 'local_instantiation' in running_tests:
        test_local_instantiation()
    if 'all' in running_tests or 'instantiation' in running_tests:
        test_instantiation()
    if 'all' in running_tests or 'update' in running_tests:
        test_update()
    if 'all' in running_tests or 'execution' in running_tests:
        test_execution()
