import numpy as np
import math


def tile_process(img, tile_size, overlap_ratio, sf, size_divisible_by, process):
    """

    :param img: image to process tile by tile
    :param tile_size: tile size
    :param overlap_ratio: overlap between 2 tiles in percentage
    :param sf: scale factor of the process function
    :param size_divisible_by: image to pass in the process function must sometimes be divisible by an integer, put 1 if not
    :param process: function to call to process 1 tile
    :return: smooth stitching of all processed tiles
    """
    shape = np.shape(img)
    h, w = np.shape(img)[:2]

    # modify tile_size to fit with the needs of the process function
    h = h * sf // (sf * size_divisible_by) * size_divisible_by
    w = w * sf // (sf * size_divisible_by) * size_divisible_by

    if len(shape) == 3:
        c = np.shape(img)[2]
        img_up = np.zeros((sf * h, sf * w, c), dtype='half')
    elif len(shape) == 2:
        img_up = np.zeros((sf * h, sf * w), dtype='half')
    else:
        print("Not an image")
        return

    img = img[:h, :w]
    tile_size = min(h, w, tile_size)
    while h % tile_size * sf % size_divisible_by != 0 and w % tile_size * sf % size_divisible_by != 0:
        tile_size += 1
    # modify overlap_ratio to avoid rounding errors
    overlap_ratio = round(tile_size * overlap_ratio) / tile_size
    print("Effective tile size : " + str(tile_size))
    print("Effective overlap ratio : " + str(overlap_ratio))
    overlap = int(tile_size * overlap_ratio)
    stride = int(tile_size * (1 + 2 * overlap_ratio))
    nx = math.ceil(w / tile_size)
    ny = math.ceil(h / tile_size)
    mask = compute_weights((sf * int(stride), sf * int(stride)), overlap_ratio / (1 + 2 * overlap_ratio))
    mask = np.array(mask, dtype='half')
    nb_tiles = nx * ny
    nb_done = 0
    for j in range(nx):
        if j == 0:
            start_j = 0
            end_j = stride - 2 * overlap
        else:
            start_j = end_j - overlap
            end_j = start_j + stride

        for i in range(ny):
            if i == 0:
                start_i = 0
                end_i = stride - 2 * overlap
            else:
                start_i = end_i - overlap
                end_i = start_i + stride

            img_tile = img[start_i:end_i, start_j:end_j]

            tile_h, tile_w = np.shape(img_tile)[:2]
            cropped_mask = mask
            if tile_h != int(stride):
                if i == 0:
                    if ny == 1:
                        cropped_mask = cropped_mask[sf * overlap:-sf * overlap]
                    else:
                        cropped_mask = cropped_mask[2 * sf * overlap:]
                else:
                    cropped_mask = cropped_mask[:sf * tile_h]

            if tile_w != int(stride):
                if j == 0:
                    if nx == 1:
                        cropped_mask = cropped_mask[:, sf * overlap:-sf * overlap]
                    else:
                        cropped_mask = cropped_mask[:, 2 * sf * overlap:]
                else:
                    cropped_mask = cropped_mask[:, :sf * tile_w]
            # nothing done if the area of tile is zero
            if np.prod(np.shape(img_tile)):
                upscaled_tile = process(img_tile)
                if upscaled_tile.ndim == 3:
                    # shortcut for upscaled_tile *= np.stack([cropped_mask,cropped_mask,cropped_mask],axis=2)
                    upscaled_tile = np.einsum('ijk,ij->ijk', upscaled_tile, cropped_mask)
                else:
                    upscaled_tile = upscaled_tile * cropped_mask
                img_up[sf * start_i:sf * end_i, sf * start_j:sf * end_j] += upscaled_tile
            nb_done += 1
            print(str(int(nb_done / nb_tiles * 100)) + "%")

    return img_up


def f(x, y):
    # just a function that returns 0 for x=1, 0 for y=1 and 1 for (x,y)=(0,0)
    return (x - 1) * (y - 1)


def compute_weights(size, overlap_ratio):
    # calculate once the mask to apply to the processed tile for blending
    if overlap_ratio == 0:
        return np.ones(size)
    h, w = size
    # x and y must contain the value overlap_ratio
    x = np.linspace(0, 1, w)
    y = np.linspace(0, 1, h)
    xv, yv = np.meshgrid(x, y)

    x_01 = (xv - 1 + overlap_ratio) / overlap_ratio
    y_01 = (yv - 1 + overlap_ratio) / overlap_ratio
    # mean_y and mean_x must be 0.5, so we force it
    mean_y = np.mean(y_01[y_01 > 0])
    y_01 = y_01 - mean_y + 0.5
    mean_x = np.mean(x_01[x_01 > 0])
    x_01 = x_01 - mean_x + 0.5

    # right overlap (between 2 images)
    res = np.where(xv >= 1 - overlap_ratio, 1 - x_01, 1)

    # bottom overlap (between 2 images)
    res = np.where(yv >= 1 - overlap_ratio, 1 - y_01, res)

    # right bottom overlap (between 4 images)
    # this formula comes from the symetries, and the fact the sum of the mask on the overlapping area of 4 images
    # must be equal to 1
    res = np.where((xv >= 1 - overlap_ratio) & (yv >= 1 - overlap_ratio), f(x_01, y_01) /
                   (f(x_01, y_01) + f(y_01, 1 - x_01) + f(1 - x_01, 1 - y_01) + f(1 - y_01, x_01))
                   , res)

    # vertical symetry
    mx = len(res[0]) // 2
    rx = len(res[0]) % 2
    res[:, :mx + rx] = res[:, :mx - 1:-1]

    # horizontal symetry
    my = len(res) // 2
    ry = len(res) % 2
    res[:my + ry] = res[:my - 1:-1]
    return res
