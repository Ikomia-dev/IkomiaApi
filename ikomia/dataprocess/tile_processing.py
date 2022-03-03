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

import numpy as np
import math


def tile_process(img, tile_size, overlap_ratio, sf, size_divisible_by, minimum_size, process):
    """

    :param img: image (numpy array) to process tile by tile
    :param tile_size: tile size in pixels
    :param overlap_ratio: overlap between 2 tiles in percentage
    :param sf: scale factor of the process function
    :param size_divisible_by: image to pass in the process function must sometimes be divisible by an integer, put 1 if not
    :param process: function to call to process 1 tile. Must take a ndarray as input and return a ndarray
    :return: smooth stitching of all processed tiles
    """
    shape = np.shape(img)
    h, w = np.shape(img)[:2]

    if len(shape) == 3:
        c = np.shape(img)[2]
        img_up = np.zeros((sf * h, sf * w, c), dtype='half')
    elif len(shape) == 2:
        img_up = np.zeros((sf * h, sf * w), dtype='half')
    else:
        print("Not an image")
        return

    img = img[:h, :w]
    tile_h = min(tile_size, h)
    tile_w = min(tile_size, w)

    # modify overlap_ratio to avoid rounding errors
    overlap_ratio_w = round(tile_w * overlap_ratio) / tile_w
    overlap_ratio_h = round(tile_h * overlap_ratio) / tile_h

    overlap_w = round(tile_w * overlap_ratio_w)
    overlap_h = round(tile_h * overlap_ratio_h)

    nx = math.ceil((w + overlap_w) / (tile_w - overlap_w))
    ny = math.ceil((h + overlap_h) / (tile_h - overlap_h))
    mask = compute_weights((sf * tile_h, sf * tile_w), overlap_ratio_w,
                           overlap_ratio_h)

    mask = np.array(mask, dtype='half')
    nb_tiles = nx * ny
    nb_done = 0
    start_j = 0
    end_j = tile_w - overlap_w
    while start_j < w:
        start_i = 0
        end_i = tile_h - overlap_h
        while start_i < h:
            img_tile = img[start_i:end_i, start_j:end_j]
            current_tile_h, current_tile_w = np.shape(img_tile)[:2]
            cropped_mask = mask

            if current_tile_h != tile_h:
                if start_i == 0:
                    if end_i >= h:
                        cropped_mask = cropped_mask[sf * overlap_h:-sf * overlap_h]
                    else:
                        cropped_mask = cropped_mask[sf * overlap_h:]
                else:
                    cropped_mask = cropped_mask[:sf * current_tile_h]

            if current_tile_w != tile_w:
                if start_j == 0:
                    if end_j >= w:
                        cropped_mask = cropped_mask[:, sf * overlap_w:-sf * overlap_w]
                    else:
                        cropped_mask = cropped_mask[:, sf * overlap_w:]
                else:
                    cropped_mask = cropped_mask[:, :sf * current_tile_w]
            # nothing is done if the area of tile is zero
            if np.prod(np.shape(img_tile)):
                is_tile_w_too_short = current_tile_w < minimum_size
                is_tile_h_too_short = current_tile_h < minimum_size
                pad_w = math.ceil(current_tile_w + max(0,
                                                       minimum_size - current_tile_w) / size_divisible_by) * size_divisible_by - current_tile_w
                pad_h = math.ceil(current_tile_h + max(0,
                                                       minimum_size - current_tile_h) / size_divisible_by) * size_divisible_by - current_tile_h

                if is_tile_h_too_short or is_tile_w_too_short:
                    img_tile = np.pad(img_tile, pad_width=((0, pad_h), (0, pad_w), (0, 0)), mode='reflect')

                upscaled_tile = process(img_tile)
                upscaled_tile = upscaled_tile[:sf * current_tile_h, :sf * current_tile_w]
                if upscaled_tile.ndim == 3:
                    # shortcut for upscaled_tile *= np.stack([cropped_mask,cropped_mask,cropped_mask],axis=2)
                    upscaled_tile = np.einsum('ijk,ij->ijk', upscaled_tile, cropped_mask)
                else:
                    upscaled_tile = upscaled_tile * cropped_mask
                img_up[sf * start_i:sf * end_i, sf * start_j:sf * end_j] += upscaled_tile
                nb_done += 1
            print(str(int(nb_done / nb_tiles * 100)) + "%")
            start_i = end_i - overlap_h
            end_i = start_i + tile_h

        start_j = end_j - overlap_w
        end_j = start_j + tile_w

    return img_up


def f(x, y):
    # function validating the following needs :
    # f(0,0) = 1
    # f(x,1) = 0
    # f(1,y) = 0
    # f(x,y) + f(1-x,y) + f(1-x,1-y) + f(x,1-y) = 1
    return (x - 1) * (y - 1)


def compute_weights(size, overlap_ratio_w, overlap_ratio_h):
    # calculate once the mask to apply to the processed tile for blending
    h, w = size
    # x and y must contain the value overlap_ratio
    x = np.linspace(0, 1, w)
    y = np.linspace(0, 1, h)
    xv, yv = np.meshgrid(x, y)
    if overlap_ratio_h == 0 or overlap_ratio_w == 0:
        return np.ones(size)
    x_01 = (xv - 1 + overlap_ratio_w) / overlap_ratio_w
    y_01 = (yv - 1 + overlap_ratio_h) / overlap_ratio_h
    # mean_y and mean_x must be 0.5, so we force it
    mean_y = np.mean(y_01[y_01 > 0])
    y_01 = y_01 - mean_y + 0.5
    mean_x = np.mean(x_01[x_01 > 0])
    x_01 = x_01 - mean_x + 0.5

    # right overlap (between 2 images)
    res = np.where(xv >= 1 - overlap_ratio_w, 1 - x_01, 1)

    # bottom overlap (between 2 images)
    res = np.where(yv >= 1 - overlap_ratio_h, 1 - y_01, res)

    # right bottom overlap (between 4 images)
    # this formula comes from the symetries, and the fact the sum of the mask on the overlapping area of 4 images
    # must be equal to 1
    res = np.where((xv >= 1 - overlap_ratio_w) & (yv >= 1 - overlap_ratio_h), f(x_01, y_01), res)

    # vertical symetry
    mx = len(res[0]) // 2
    rx = len(res[0]) % 2
    res[:, :mx + rx] = res[:, :mx - 1:-1]

    # horizontal symetry
    my = len(res) // 2
    ry = len(res) % 2
    res[:my + ry] = res[:my - 1:-1]
    return res
