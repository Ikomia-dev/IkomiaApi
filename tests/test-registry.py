import os
import logging
import ikomia
from ikomia.dataprocess import store
import cv2

logger = logging.getLogger(__name__)


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Images"


def get_test_video_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Videos"


def test_get_local_algorithms():
    registry = store.IkomiaRegistry()
    algos = registry.getAlgorithms()
    logger.info("Number of algorithms:" + str(len(algos)))
    logger.info(algos)


def test_get_online_algorithms():
    registry = store.IkomiaRegistry()
    algos = registry.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


def test_download_plugin():
    registry = store.IkomiaRegistry()
    logger.info("Downloading RAFTOpticalFlow...")
    registry._download_plugin("RAFTOpticalFlow")


def test_install_plugin():
    registry = store.IkomiaRegistry()
    logger.info("Installing RAFTOpticalFlow...")
    registry._install_plugin("RAFTOpticalFlow")
    logger.info("Installing MobileNet SSD...")
    registry._install_plugin("MobileNet SSD")


def test_local_instantiation():
    registry = store.IkomiaRegistry()
    algo_names = registry.getAlgorithms()
    algos = []

    for name in algo_names:
        algo = registry.createInstance(name)
        logger.info(algo.name)
        algos.append(algo)

    try:
        logger.info("Try running algorithm (error expected):" + algos[0].name)
        algos[0].run()
    except Exception as e:
        logger.error(e)


def test_instantiation():
    registry = store.IkomiaRegistry()
    # Local C++ algo
    logger.info("Instantiate CLAHE algorithm...")
    algo = registry.create_algorithm("CLAHE")
    assert(algo is not None)
    logger.info("Done.")
    # Local Python algo
    logger.info("Instantiate ResNet Train algorithm...")
    algo = registry.create_algorithm("ResNet Train")
    assert (algo is not None)
    logger.info("Done.")
    # Online C++ algo
    logger.info("Instantiate MobileNet SSD algorithm...")
    algo = registry.create_algorithm("MobileNet SSD")
    logger.info("Done.")
    # Online Python algo
    logger.info("Instantiate RAFTOpticalFlow algorithm...")
    algo = registry.create_algorithm("RAFTOpticalFlow")
    logger.info("Done.")


def test_update():
    registry = store.IkomiaRegistry()
    logger.info("Updating ResNet Train algorithm...")
    registry.update_algorithm("ResNet Train")
    logger.info("Updating all algorithms...")
    registry.update_algorithms()


def test_execution():
    registry = store.IkomiaRegistry()

    # Load image
    img_path = get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    cv2.imshow("Original", img)

    # OpenCV CLAHE
    algo = registry.create_algorithm("CLAHE")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output = algo.getOutput(0)
    img_out = output.getImage()
    cv2.imshow("CLAHE", img_out)

    # Marketplace FaceDetector
    algo = registry.create_algorithm("Face Detector")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_graphics = algo.getOutput(1)
    output_img.drawGraphics(output_graphics)
    img_out = output_img.getImage()
    cv2.imshow("Face Detector", img_out)
    cv2.waitKey(0)


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    # test_get_local_algorithms()
    # test_get_online_algorithms()
    # test_download_plugin()
    # test_install_plugin()
    # test_local_instantiation()
    # test_instantiation()
    # test_update()
    test_execution()
