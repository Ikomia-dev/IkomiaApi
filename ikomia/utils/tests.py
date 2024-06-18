# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
The module tests provides features to ease test automation of Ikomia algorithms.
"""
import os
import traceback
import logging
from ikomia.core import CWorkflowTask  # pylint: disable=E0611

logger = logging.getLogger(__name__)


def run_for_test(t: CWorkflowTask) -> tuple:
    """
    Test run() function for the given task.

    Args:
         t (CWorkflowTask): task (ie algorithm) to test

    Returns:
        tuple: (task parameters, status)
    """
    logger.info("Running once...")
    try:
        import torch.cuda
        logger.info("Cleaning cuda torch cache...")
        torch.cuda.empty_cache()
        logger.info("Cuda torch cache cleaned...")
    except ImportError:
        pass

    try:
        t.run()
        tb = "OK"
        logger.info("Run succeeded")
    except Exception:
        tb = traceback.format_exc()
        logger.error("Run failed")

    return t.get_parameters(), tb


def get_test_image_directory() -> str:
    """
    Get sample's images directory.

    Returns:
        str: path to the directory
    """
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/images")


def get_test_video_directory() -> str:
    """
    Get sample's videos directory.

    Returns:
        str: path to the directory
    """
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/videos")


def get_test_workflow_directory() -> str:
    """
    Get test's workflows directory.

    Returns:
        str: path to the directory
    """
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/workflows")
