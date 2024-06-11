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
The dataio module manages data source from various formats of image and videos.
It is in fact a Python binding of the corresponding C++ library from Ikomia Core.
"""

try:
    # Valid for Ikomia Studio or Linux platform
    from ikomia.dataio.pydataio import *
except ImportError:
    # Valid for Windows standalone API
    from ikomia.lib.pydataio import *
