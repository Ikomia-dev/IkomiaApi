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
