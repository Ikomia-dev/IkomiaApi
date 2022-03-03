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
Internal use only
"""

import os
import numpy as np


class NumpyImage:
    def __init__(self, path, key):
        self.path = path
        self.data = None
        filename, extension = os.path.splitext(path)

        if extension != ".npz":
            raise Exception("File format not supported")

        self.data = np.load(self.path)[key]

    def get_dims(self):
        return self.data.shape

    def set_dim_order(self, dims):
        self.data = np.transpose(self.data, dims)

    def get_data(self, dtype=None):
        if dtype is not None:
            return self.data.astype(dtype)
        else:
            return self.data

    def get_2d_data(self, index, dtype=None):
        dim_count = len(self.data.shape)
        data = None

        if dim_count == 1 or dim_count == 2:
            data = self.data
        elif dim_count == 3:
            data = self.data[index]
        else:
            raise Exception("Array structure not supported")

        if dtype is not None:
            return data.astype(dtype)
        else:
            return data
