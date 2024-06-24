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
Module providing dataset loaders from various source formats.
"""

import os
import json
import random
import logging
import xml.etree.ElementTree as ET
from collections import defaultdict
from PIL import Image
import numpy as np
from ikomia.core import CPointF, CGraphicsPolygon, CGraphicsConversion  # pylint: disable=E0611
import cv2

logger = logging.getLogger(__name__)

try:
    from pycocotools.coco import maskUtils
except ImportError:
    logger.info("Pycocotools package is not installed, some dataset loader may not work properly.")


_image_extensions = [".jpeg", ".jpg", ".png", ".bmp", ".tiff", ".tif", ".dib", ".jpe", ".jp2", ".webp", ".pbm", ".pgm",
                     ".ppm", ".pxm", ".pnm", ".sr", ".ras", ".exr", ".hdr", ".pic"]


def load_via_dataset(path: str) -> dict:
    """
    Load VGG Image Annotator (VIA) dataset.
    VIA dataset is a single JSON file containing all information.

    Args:
        path (str): path to the JSON file

    Returns:
        dict: Ikomia dataset structure. See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    with open(path, encoding="utf-8") as fp:
        raw_data = json.load(fp)

    data = {"images": [], "metadata": {}}
    folder = os.path.dirname(path)
    via_metadata = raw_data["_via_img_metadata"]
    category_indices = {}
    category_names = {}

    # Collect class names
    for img_id in via_metadata:
        img_obj = via_metadata[img_id]
        regions = img_obj["regions"]

        for region in regions:
            reg_attr = region['region_attributes']
            for key in reg_attr:
                category = reg_attr[key]
                if category not in category_indices:
                    category_id = len(category_indices)
                    category_indices[category] = category_id
                    category_names[category_id] = category

    data["metadata"] = {"category_names": category_names}

    # Collect annotations
    for img_id in via_metadata:
        img_data = {}
        img_obj = via_metadata[img_id]
        regions = img_obj["regions"]

        # No annotation, skip image
        if len(regions) == 0:
            continue

        # Unique id
        img_data["image_id"] = img_id
        # Full path of the image
        img_data["filename"] = folder + '/' + img_obj["filename"]
        # Read image to get width and height
        img = Image.open(img_data["filename"])
        img_data["width"], img_data["height"] = img.size
        # Annotations
        img_data["annotations"] = []

        for region in regions:
            instance = {}
            shape_attr = region["shape_attributes"]
            shape_name = shape_attr["name"]

            if shape_name == "circle":
                x = shape_attr["cx"] - shape_attr["r"]
                y = shape_attr["cy"] - shape_attr["r"]
                d = shape_attr["r"] * 2.0
                instance["bbox"] = [x, y, d, d]
            elif shape_name == "rect":
                x = shape_attr["x"]
                y = shape_attr["y"]
                w = shape_attr["width"]
                h = shape_attr["height"]
                instance["bbox"] = [x, y, w, h]
            else:
                print('Unmanaged shape attribute, object ignored.')
                continue

            reg_attr = region['region_attributes']

            if len(reg_attr) > 1:
                print("Warning: the VIA dataset loader manages only one attribute to define classes type. "
                      "The first one is used for class type, others are ignored.")

            attr_type = next(iter(reg_attr))
            instance["category_id"] = category_indices[reg_attr[attr_type]]
            img_data["annotations"].append(instance)

        if len(img_data["annotations"]) > 0:
            data["images"].append(img_data)

    return data


def read_class_names(txt_path: str) -> list:
    """
    Helper function to parse text file and extract class names.
    The text file must be structured so that one line = one class name.

    Args:
        txt_path (str): path to the text file

    Returns:
        str[]: list of class names
    """
    with open(txt_path, 'rt', encoding="utf-8") as f:
        classes = f.read().rstrip('\n').split('\n')

    return classes


def load_yolo_dataset(folder_path: str, class_path: str) -> dict:
    """
    Load YOLO dataset.
    YOLO dataset consists of a list of text files with
    the same name as the corresponding image. Each line
    represent a bounding box with the following information:

        - class number x_center y_center width height

    Note that center coordinates and box size are relative to
    the image size.

    Args:
        folder_path (str): path to the dataset folder (image + text files)
        class_path (str): path the text file containing all class names

    Returns:
        dict: Ikomia dataset structure.  See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    data = {"images": [], "metadata": {}}
    img_id = 0

    # Collect annotations
    root, _, files = next(os.walk(folder_path))
    for file in files:
        img_data = {}
        filename, extension = os.path.splitext(file)

        # Scan only txt files
        if extension != ".txt":
            continue

        lines = []
        with open(root + os.sep + file, "rt", encoding="utf-8") as f:
            lines = f.readlines()

        # No annotation, skip image
        if len(lines) == 0:
            continue

        has_image = False
        for ext in _image_extensions:
            img_path = root + os.sep + filename + ext
            if os.path.isfile(img_path):
                # Full path of the image
                img_data["filename"] = img_path
                has_image = True
                break

        if not has_image:
            continue

        # Unique id
        img_data["image_id"] = img_id
        img_id = img_id + 1

        # Open image to get width and height
        im = Image.open(img_data["filename"])
        width, height = im.size
        img_data["width"] = width
        img_data["height"] = height

        # Annotations
        img_data["annotations"] = []
        for line in lines:
            instance = {}
            str_values = line.split()

            if len(str_values) != 5:
                print('Invalid box definition in file ', file)
                continue

            instance["category_id"] = int(str_values[0])

            w = float(str_values[3]) * width
            h = float(str_values[4]) * height
            x = max(0, float(str_values[1]) * float(width) - (w / 2.0))
            y = max(0, float(str_values[2]) * float(height) - (h / 2.0))

            if x + w >= width:
                w = width - x - 1

            if y + h >= height:
                h = height - y - 1

            instance["bbox"] = [x, y, w, h]
            img_data["annotations"].append(instance)

        if len(img_data["annotations"]) > 0:
            data["images"].append(img_data)

    category_names = {}
    categories = read_class_names(class_path)

    for i, _ in enumerate(categories):
        category_names[i] = categories[i]

    data["metadata"] = {"category_names": category_names}
    return data


def ann_to_rle(img: np.ndarray, ann: dict) -> np.ndarray:
    """
    Convert annotation which can be polygons, uncompressed RLE to RLE.

    Args:
        img (numpy.ndarray): image from which annotations come
        ann (dict): annotations to compress to RLE

    Returns:
        binary mask (numpy 2D array)
    """
    h, w = img['height'], img['width']
    segm = ann['segmentation']
    if isinstance(segm, list):
        # polygon -- a single object might consist of multiple parts
        # we merge all parts into one mask rle code
        rles = maskUtils.frPyObjects(segm, h, w)
        rle = maskUtils.merge(rles)
    elif isinstance(segm['counts'], list):
        # uncompressed RLE
        rle = maskUtils.frPyObjects(segm, h, w)
    else:
        # rle
        rle = ann['segmentation']

    return rle


def load_coco_dataset(path: str, image_folder: str,
                      task: str = "instance_segmentation", output_folder: str = "") -> dict:
    """
    Load COCO dataset (2017 version).
    COCO dataset consists in a JSON file describing annotations
    and an image folder.

    Args:
        path (str): path to the JSON annotation file
        image_folder (str): path to the image folder
        task (str): task of the dataset, must be one of the following "detection", "instance_segmentation",
            "semantic_segmentation" or "keypoints"
        output_folder (str): path to output folder only for semantic segmentation

    Returns:
        dict: Ikomia dataset structure. See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    data = {"images": [], "metadata": {}}

    sem_seg = task == "semantic_segmentation"
    keypoints = task == "keypoints"

    assert not sem_seg or output_folder != "", "Output folder must be set when task is semantic segmentation"

    if not image_folder.endswith("/"):
        image_folder += "/"

    with open(path, "r", encoding="utf-8") as fp:
        dataset = json.load(fp)
        img_to_anns = defaultdict(list)
        data["metadata"] = {}
        if sem_seg:
            cat_names = {0: "background"}
            cat_map = {0: 0}
        else:
            cat_names = {}
            cat_map = {}

        if "annotations" in dataset:
            for ann in dataset["annotations"]:
                img_to_anns[ann["image_id"]].append(ann)

        if "categories" in dataset:
            for cat in dataset["categories"]:
                i = len(cat_names.keys())
                cat_names[i] = cat["name"]
                cat_map[cat["id"]] = i

                if keypoints:
                    # We don't support yet multi class keypoints dataset

                    data["metadata"]["keypoint_names"] = dataset["categories"][0]["keypoints"]
                    data["metadata"]["keypoint_connection_rules"] = [
                        (data["metadata"]["keypoint_names"][i1 - 1], data["metadata"]["keypoint_names"][i2 - 1],
                         tuple([random.randint(0, 255) for i in range(3)]))
                        for (i1, i2) in dataset["categories"][0]["skeleton"]]
                    data["metadata"]["keypoint_flip_map"] = []
                    for body_part in data["metadata"]["keypoint_names"]:
                        if body_part.startswith("left"):
                            data["metadata"]["keypoint_flip_map"].append((body_part,
                                                                          body_part.replace("left", "right")))
                        elif not body_part.startswith("right"):
                            data["metadata"]["keypoint_flip_map"].append((body_part, body_part))

        data["metadata"]["category_names"] = cat_names

        if "images" in dataset:
            for img in dataset["images"]:
                img_data = {
                    "image_id": img["id"],
                    "filename": image_folder + img["file_name"],
                    "width": img["width"],
                    "height": img["height"],
                    "annotations": []
                }
                if sem_seg:
                    mask = np.zeros((img['height'], img['width']), dtype='uint8')

                for ann in img_to_anns[img["id"]]:
                    instance = {"category_id": cat_map[ann["category_id"]], "iscrowd": ann["iscrowd"]}

                    if "bbox" in ann:
                        instance["bbox"] = ann["bbox"]

                    if "segmentation" in ann:
                        # RLE mask not managed yet
                        if isinstance(ann["segmentation"], list):
                            # Polygon
                            instance["segmentation_poly"] = ann["segmentation"]
                            if sem_seg:
                                poly = []
                                for pts in instance["segmentation_poly"]:
                                    poly.append(np.array(pts, dtype='int').reshape((-1, 2)))

                                color = int(instance["category_id"])
                                cv2.fillPoly(mask, poly, color)

                    if "keypoints" in ann:
                        instance["keypoints"] = ann["keypoints"]

                    img_data["annotations"].append(instance)

                if len(img_data["annotations"]) > 0:
                    if sem_seg:
                        img_data["semantic_seg_masks_file"] = os.path.join(output_folder,
                                                                           str(img_data["image_id"]) + ".png")
                        cv2.imwrite(img_data["semantic_seg_masks_file"], mask)
                    data["images"].append(img_data)

    return data


def load_pascalvoc_dataset(annotation_folder: str, img_folder: str, instance_seg_folder: str, class_path: str) -> dict:
    """
    Load PASCAL-VOC dataset (version 2012).
    PASCAL-VOC dataset is structured in different folders:

        - image folder
        - annotation folder: text files (same name as corresponding image files)
        - instance segmentation folder: image file (same name as corresponding image files)

    Args:
        annotation_folder (str): path to annotations folder
        img_folder (str): path to images folder
        instance_seg_folder: path to segmentation masks folder
        class_path: path to text file containing class names

    Returns:
        dict: Ikomia dataset structure. See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    obj_id = 0
    data = {"images": [], "metadata": {}}
    ann_root, _, ann_files = next(os.walk(annotation_folder))

    if instance_seg_folder and not instance_seg_folder.endswith("/"):
        instance_seg_folder += "/"

    categories = read_class_names(class_path)
    category_ids = {}

    for i, _ in enumerate(categories):
        category_ids[categories[i]] = i

    if annotation_folder != img_folder:
        img_root, _, _ = next(os.walk(img_folder))
    else:
        img_root = ann_root

    for file in ann_files:
        img_data = {}
        filename, extension = os.path.splitext(file)

        # Scan only xml files
        if extension != ".xml":
            continue

        tree = ET.parse(ann_root + "/" + file)
        root = tree.getroot()

        # Check if we must load the segmentation part of the dataset
        if instance_seg_folder:
            seg_node = root.find("segmented")
            is_segmented = int(seg_node.text)

            if is_segmented:
                img_data["instance_seg_masks_file"] = instance_seg_folder + "/" + filename + ".png"
            else:
                continue

        filename_node = root.find("filename")
        # Unique id
        img_data["image_id"] = obj_id
        obj_id = obj_id + 1
        # Full path of the image
        img_data["filename"] = img_root + "/" + filename_node.text

        # Image size
        size_node = root.find("size")
        w_node = size_node.find("width")
        img_data["width"] = int(w_node.text)
        h_node = size_node.find("height")
        img_data["height"] = int(h_node.text)

        img_data["annotations"] = []
        for obj in root.iter("object"):
            instance = {}
            # Category id
            instance["category_id"] = category_ids[obj.find("name").text]
            # Bounding box
            bbox_node = obj.find("bndbox")
            xmin = float(bbox_node.find("xmin").text)
            ymin = float(bbox_node.find("ymin").text)
            xmax = float(bbox_node.find("xmax").text)
            ymax = float(bbox_node.find("ymax").text)
            instance["bbox"] = [xmin, ymin, (xmax - xmin), (ymax - ymin)]
            img_data["annotations"].append(instance)

        if len(img_data["annotations"]) > 0:
            data["images"].append(img_data)

    category_names = {}
    for i, _ in enumerate(categories):
        category_names[i] = categories[i]

    data["metadata"] = {"category_names": category_names}
    return data


def polygon_to_mask(polygons: list, width: int, height: int) -> np.ndarray:
    """
    Helper function to generate image mask from a list of polygons.
    The function assumes that each polygon is a list of xy coordinates.

    Args:
        polygons: list of polygons
        width (int): width of the future mask
        height (int): height of the future mask

    Returns:
        numpy array: image mask
    """
    graphics = []
    for poly in polygons:
        pts = []
        i = 0
        while i < len(poly):
            x = poly[i]
            y = poly[i + 1]

            if x > width - 1:
                x = width - 1

            if y > height - 1:
                y = height - 1

            pts.append(CPointF(x, y))
            i += 2

        graphics_poly = CGraphicsPolygon(pts)
        graphics.append(graphics_poly)

    conversion = CGraphicsConversion(width, height)
    return conversion.graphics_to_binary_mask(graphics)
