import logging
import sys
import argparse
import ikomia
import cv2
from ikomia.dataprocess import displayIO
from ikomia.utils import tests, ik

logger = logging.getLogger(__name__)


def test_get_local_algorithms():
    logger.info("===== Test::get list of local algorithms =====")
    algos = ikomia.ik_registry.getAlgorithms()
    logger.info("Number of algorithms:" + str(len(algos)))
    logger.info(algos)


def test_get_online_algorithms():
    logger.info("===== Test::get list of online algorithms =====")
    algos = ikomia.ik_registry.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


def test_download_plugin():
    logger.info("===== Test::download online algorithm =====")
    logger.info("Downloading RAFTOpticalFlow...")
    ikomia.ik_registry._download_algorithm(ik.infer_raft_optical_flow)


def test_install_plugin():
    logger.info("===== Test::install online algorithms =====")
    logger.info("Installing RAFTOpticalFlow...")
    ikomia.ik_registry.install_algorithm(ik.infer_raft_optical_flow)

    logger.info("Installing MobileNet SSD...")
    if sys.version_info[1] == 7:
        ikomia.ik_registry.install_algorithm(ik.infer_mobilenet_ssd)


def test_local_instantiation():
    logger.info("===== Test::instanciate local algorithms =====")
    algo_names = ikomia.ik_registry.getAlgorithms()
    algos = []

    for name in algo_names:
        logger.info(name)
        algo = ikomia.ik_registry.createInstance(name)
        algos.append(algo)

    try:
        logger.info("Try running algorithm (error expected):" + algos[0].name)
        algos[0].run()
    except Exception as e:
        logger.error(e)


def test_instantiation():
    logger.info("===== Test::instanciate online algorithms =====")
    # Local C++ algo
    logger.info("Instantiate CLAHE algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_clahe)
    assert(algo is not None)
    logger.info("Done.")
    # Local Python algo
    logger.info("Instantiate ResNet Train algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.train_torchvision_resnet)
    assert (algo is not None)
    logger.info("Done.")
    # Online C++ algo
    logger.info("Instantiate MobileNet SSD algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.infer_mobilenet_ssd)
    assert (algo is not None)
    logger.info("Done.")
    # Online Python algo
    logger.info("Instantiate RAFTOpticalFlow algorithm...")
    algo = ikomia.ik_registry.create_algorithm(ik.infer_raft_optical_flow)
    assert (algo is not None)
    logger.info("Done.")


def test_update():
    logger.info("===== Test::update algorithms =====")
    logger.info("Updating ResNet Train algorithm...")
    ikomia.ik_registry.update_algorithm(ik.train_torchvision_resnet)
    logger.info("Updating all algorithms...")
    ikomia.ik_registry.update_algorithms()


def test_execution():
    logger.info("===== Test::execute local and online algorithms =====")

    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    cv2.imshow("Original", img)

    # OpenCV CLAHE (C++)
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_clahe)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # OpenCV Canny (C++)
    algo = ikomia.ik_registry.create_algorithm(ik.ocv_canny)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # Marketplace FaceDetector (C++)
    algo = ikomia.ik_registry.create_algorithm(ik.infer_face_detector)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_graphics = algo.getOutput(1)
    output_img.drawGraphics(output_graphics)
    displayIO.display(algo, algo.name)

    # Marketplace scikit_threshold (Python)
    algo = ikomia.ik_registry.create_algorithm(ik.skimage_threshold)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # Marketplace infer_yolov5 (Python)
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = ikomia.ik_registry.create_algorithm(ik.infer_yolo_v5)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_img.drawGraphics(algo.getOutput(1))
    displayIO.display(algo, algo.name)


def test_auto_completion():
    # Algorithm infer_torchvision_mnasnet must not be installed for this tes
    logger.info("===== Test::hot auto-completion update =====")

    try:
        algo_name = ik.infer_torchvision_mnasnet
        algo_param = ik.infer_torchvision_mnasnet_param
    except Exception as e:
        logger.info(e)

    ikomia.ik_registry.create_algorithm(ik.infer_torchvision_mnasnet)
    algo_name = ik.infer_torchvision_mnasnet
    algo_param = ik.infer_torchvision_mnasnet_param
    logger.info(algo_name)
    logger.info(algo_param)


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
    if 'all' in running_tests or 'auto_completion' in running_tests:
        test_auto_completion()
