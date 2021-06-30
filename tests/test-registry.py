import os
import logging
import ikomia
from ikomia.dataprocess import registry
import cv2

logger = logging.getLogger(__name__)


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Images"


def get_test_video_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../cpp/UnitTests/Data/Videos"


def test_get_local_algorithms():
    reg = registry.IkomiaRegistry()
    algos = reg.getAlgorithms()
    logger.info("Number of algorithms:" + str(len(algos)))
    logger.info(algos)


def test_get_online_algorithms():
    reg = registry.IkomiaRegistry()
    algos = reg.get_online_algorithms()
    logger.info("Number of online algorithms:" + str(len(algos)))

    for algo in algos:
        logger.info(algo["name"])


def test_download_plugin():
    reg = registry.IkomiaRegistry()
    logger.info("Downloading RAFTOpticalFlow...")
    reg._download_plugin("RAFTOpticalFlow")


def test_install_plugin():
    reg = registry.IkomiaRegistry()
    logger.info("Installing RAFTOpticalFlow...")
    reg._install_plugin("RAFTOpticalFlow")
    logger.info("Installing MobileNet SSD...")
    reg._install_plugin("MobileNet SSD")


def test_local_instantiation():
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
    reg = registry.IkomiaRegistry()
    logger.info("Updating ResNet Train algorithm...")
    reg.update_algorithm("ResNet Train")
    logger.info("Updating all algorithms...")
    reg.update_algorithms()


def test_execution():
    reg = registry.IkomiaRegistry()

    # Load image
    img_path = get_test_image_directory() + "/Lena.png"
    img = cv2.imread(img_path)
    cv2.imshow("Original", img)

    # OpenCV CLAHE (C++)
    algo = reg.create_algorithm("CLAHE")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output = algo.getOutput(0)
    img_out = output.getImage()
    cv2.imshow("CLAHE", img_out)

    # OpenCV Canny (C++)
    algo = reg.create_algorithm("Canny")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output = algo.getOutput(0)
    img_out = output.getImage()
    cv2.imshow("Canny", img_out)

    # Marketplace FaceDetector (C++)
    algo = reg.create_algorithm("Face Detector")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_graphics = algo.getOutput(1)
    output_img.drawGraphics(output_graphics)
    img_out = output_img.getImage()
    cv2.imshow("Face Detector", img_out)

    # Marketplace scikit_threshold (Python)
    algo = reg.create_algorithm("scikit_threshold")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output = algo.getOutput(0)
    img_out = output.getImage()
    cv2.imshow("scikit_threshold", img_out)

    # Marketplace YoloV5Predict (Python)
    img_path = get_test_image_directory() + "/example_05.jpg"
    img = cv2.imread(img_path)
    algo = reg.create_algorithm("YoloV5Predict")
    input_img = algo.getInput(0)
    input_img.setImage(img)
    algo.run()
    output_img = algo.getOutput(0)
    output_img.drawGraphics(algo.getOutput(1))
    cv2.imshow("YoloV5Predict", output_img.getImage())
    cv2.waitKey(0)


if __name__ == "__main__":
    ikomia.initialize("Ludo", "ludo?imageez")
    test_get_local_algorithms()
    # test_get_online_algorithms()
    # test_download_plugin()
    # test_install_plugin()
    # test_local_instantiation()
    # test_instantiation()
    # test_update()
    test_execution()
