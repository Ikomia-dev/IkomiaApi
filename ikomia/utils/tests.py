import os


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../../cpp/UnitTests/Data/Images"


def get_test_video_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../../cpp/UnitTests/Data/Videos"


def get_test_workflow_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return test_dir + "/../../cpp/UnitTests/Data/Workflows"
