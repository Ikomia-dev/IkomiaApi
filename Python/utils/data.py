# Copyright (C) 2021 Ikomia SAS
# Contact: https://www.ikomia.com
#
# This file is part of the Ikomia API libraries.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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
