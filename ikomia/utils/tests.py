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

import os
from ikomia.core import task
import traceback
import logging

logger = logging.getLogger(__name__)


def run_for_test(t):
    logger.info("Running once...")
    try:
        import torch.cuda
        logger.info("Cleaning cuda torch cache...")
        torch.cuda.empty_cache()
        logger.info("Cuda torch cache cleaned...")
    except:
        pass
    try:
        t.run()
    except Exception as e:
        tb = traceback.format_exc()
        logger.error("Run failed")
    else:
        tb = "OK"
        logger.info("Run succeeded")
    finally:
        return t.get_parameters(), tb


def get_test_image_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/images")


def get_test_video_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/videos")


def get_test_workflow_directory():
    test_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(test_dir,  "../../tests/data/workflows")
