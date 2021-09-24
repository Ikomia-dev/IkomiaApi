import logging
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
    ikomia.ik_registry._download_plugin(ik.infer_raft_optical_flow)


def test_install_plugin():
    logger.info("===== Test::install online algorithms =====")
    logger.info("Installing RAFTOpticalFlow...")
    ikomia.ik_registry._install_plugin(ik.infer_raft_optical_flow)
    logger.info("Installing MobileNet SSD...")
    ikomia.ik_registry._install_plugin(ik.infer_mobilenet_ssd)


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

    # Marketplace YoloV5Predict (Python)
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = ikomia.ik_registry.create_algorithm(ik.infer_yolo_v5)
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_img.drawGraphics(algo.getOutput(1))
    displayIO.display(algo, algo.name)


if __name__ == "__main__":
    ikomia.authenticate()
    test_get_local_algorithms()
    test_get_online_algorithms()
    test_download_plugin()
    test_install_plugin()
    test_local_instantiation()
    test_instantiation()
    test_update()
    test_execution()
