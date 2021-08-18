import logging
import ikomia
import cv2
from ikomia.dataprocess import registry, displayIO
from ikomia.utils import tests

logger = logging.getLogger(__name__)


def test_get_local_algorithms():
    logger.info("===== Test::get list of local algorithms =====")
    reg = registry.IkomiaRegistry()
    algos = reg.getAlgorithms()
    logger.info("Number of algorithms:" + str(len(algos)))
    logger.info(algos)


def test_get_online_algorithms():
    logger.info("===== Test::get list of online algorithms =====")
    reg = registry.IkomiaRegistry()
    algos = reg.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


def test_download_plugin():
    logger.info("===== Test::download online algorithm =====")
    reg = registry.IkomiaRegistry()
    logger.info("Downloading RAFTOpticalFlow...")
    reg._download_plugin("RAFTOpticalFlow")


def test_install_plugin():
    logger.info("===== Test::install online algorithms =====")
    reg = registry.IkomiaRegistry()
    logger.info("Installing RAFTOpticalFlow...")
    reg._install_plugin("RAFTOpticalFlow")
    logger.info("Installing MobileNet SSD...")
    reg._install_plugin("MobileNet SSD")


def test_local_instantiation():
    logger.info("===== Test::instanciate local algorithms =====")
    reg = registry.IkomiaRegistry()
    algo_names = reg.getAlgorithms()
    algos = []

    for name in algo_names:
        algo = reg.createInstance(name)
        logger.info(algo.name)
        algos.append(algo)

    try:
        logger.info("Try running algorithm (error expected):" + algos[0].name)
        algos[0].run()
    except Exception as e:
        logger.error(e)


def test_instantiation():
    logger.info("===== Test::instanciate online algorithms =====")
    reg = registry.IkomiaRegistry()
    # Local C++ algo
    logger.info("Instantiate CLAHE algorithm...")
    algo = reg.create_algorithm("CLAHE")
    assert(algo is not None)
    logger.info("Done.")
    # Local Python algo
    logger.info("Instantiate ResNet Train algorithm...")
    algo = reg.create_algorithm("ResNet Train")
    assert (algo is not None)
    logger.info("Done.")
    # Online C++ algo
    logger.info("Instantiate MobileNet SSD algorithm...")
    algo = reg.create_algorithm("MobileNet SSD")
    logger.info("Done.")
    # Online Python algo
    logger.info("Instantiate RAFTOpticalFlow algorithm...")
    algo = reg.create_algorithm("RAFTOpticalFlow")
    logger.info("Done.")


def test_update():
    logger.info("===== Test::update algorithms =====")
    reg = registry.IkomiaRegistry()
    logger.info("Updating ResNet Train algorithm...")
    reg.update_algorithm("ResNet Train")
    logger.info("Updating all algorithms...")
    reg.update_algorithms()


def test_execution():
    logger.info("===== Test::execute local and online algorithms =====")
    reg = registry.IkomiaRegistry()

    # Load image
    img_path = tests.get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    cv2.imshow("Original", img)

    # OpenCV CLAHE (C++)
    algo = reg.create_algorithm("CLAHE")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # OpenCV Canny (C++)
    algo = reg.create_algorithm("Canny")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # Marketplace FaceDetector (C++)
    algo = reg.create_algorithm("Face Detector")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_graphics = algo.getOutput(1)
    output_img.drawGraphics(output_graphics)
    displayIO.display(algo, algo.name)

    # Marketplace scikit_threshold (Python)
    algo = reg.create_algorithm("scikit_threshold")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    displayIO.display(algo, algo.name)

    # Marketplace YoloV5Predict (Python)
    img_path = tests.get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = reg.create_algorithm("YoloV5Predict")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_img.drawGraphics(algo.getOutput(1))
    displayIO.display(algo, algo.name)


if __name__ == "__main__":
    ikomia.initialize()
    # test_get_local_algorithms()
    # test_get_online_algorithms()
    # test_download_plugin()
    # test_install_plugin()
    # test_local_instantiation()
    # test_instantiation()
    # test_update()
    test_execution()
