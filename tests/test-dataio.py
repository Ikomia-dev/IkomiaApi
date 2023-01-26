import os
import argparse
import logging
from ikomia import dataio
from ikomia.utils import tests

logger = logging.getLogger(__name__)


def test_cpp_bindings():
    # Image IO
    is_image_format = dataio.CDataImageIO.is_image_format(".jpg")
    assert is_image_format
    is_image_format = dataio.CDataImageIO.is_image_format(".txt")
    assert not is_image_format

    image_path = os.path.join(tests.get_test_image_directory(), "Lena.png")
    image_io = dataio.CDataImageIO(image_path)
    image_data = image_io.read()
    assert image_data is not None
    logger.info(image_data.shape)

    image_path = os.path.join(tests.get_test_image_directory(), "tmp.png")
    image_io = dataio.CDataImageIO(image_path)
    image_io.write(image_data)
    assert os.path.isfile(image_path)
    os.remove(image_path)

    # Video IO
    is_video_format = dataio.CDataVideoIO.is_video_format(".avi", True)
    assert is_video_format
    is_video_format = dataio.CDataVideoIO.is_video_format(".jpg", True)
    assert not is_video_format
    is_video_format = dataio.CDataVideoIO.is_video_format(".jpg", False)
    assert is_video_format

    video_path = os.path.join(tests.get_test_video_directory(), "basketball.mp4")
    video_io = dataio.CDataVideoIO(video_path)
    frame_data = video_io.read()
    assert frame_data is not None
    logger.info(frame_data.shape)
    video_io.stop_read()

    video_path = os.path.join(tests.get_test_video_directory(), "tmp.mp4")
    video_io = dataio.CDataVideoIO(video_path)
    video_io.write(frame_data)
    video_io.wait_write_finished(1000)
    video_io.stop_write(1000)
    assert os.path.isfile(video_path)
    os.remove(video_path)

    video_path_as = os.path.join(tests.get_test_video_directory(), "tmp2.mp4")
    video_io.write(frame_data, video_path_as)
    video_io.wait_write_finished(1000)
    video_io.stop_write(1000)
    assert os.path.isfile(video_path_as)
    os.remove(video_path_as)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if "all" in running_tests or "cpp_bindings" in running_tests:
        test_cpp_bindings()
