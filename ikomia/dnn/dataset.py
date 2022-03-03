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
Module providing dataset loaders from different source format.
"""

import os
import json
from ikomia import core
from PIL import Image
from collections import defaultdict
import xml.etree.ElementTree as ET

_image_extensions = [".jpeg", ".jpg", ".png", ".bmp", ".tiff", ".tif", ".dib", ".jpe", ".jp2", ".webp", ".pbm", ".pgm",
                     ".ppm", ".pxm", ".pnm", ".sr", ".ras", ".exr", ".hdr", ".pic"]


def load_via_dataset(path):
    """
    Load VGG Image Annotator (VIA) dataset.
    VIA dataset is a single JSON file containing all information.

    Args:
        path (str): path to the JSON file

    Returns:
        dict: Ikomia dataset structure. See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    with open(path) as fp:
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


def read_class_names(txt_path):
    """
    Helper function to parse text file and extract class names.
    The text file must be structured so that one line = one class name.

    Args:
        txt_path (str): path to the text file

    Returns:
        str[]: list of class names
    """
    with open(txt_path, 'rt') as f:
        classes = f.read().rstrip('\n').split('\n')

    return classes


def load_yolo_dataset(folder_path, class_path):
    """
    Load YOLO dataset.
    YOLO dataset consists of a list of text files with
    the same name as the corresponding image. Each line
    represent a bounding box with the following information:

        - class number x_center y_center width height

    Note that center coordinates and box size are relative to
    the image size.

    Args:
        path (str): path to the dataset folder (image + text files)
        class_path (str): path the text file containing all class names

    Returns:
        dict: Ikomia dataset structure.  See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    data = {"images": [], "metadata": {}}
    img_id = 0

    # Collect annotations
    root, dirs, files = next(os.walk(folder_path))
    for file in files:
        img_data = {}
        filename, extension = os.path.splitext(file)

        # Scan only txt files
        if extension != ".txt":
            continue

        lines = []
        with open(root + os.sep + file, "rt") as f:
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

    for i in range(len(categories)):
        category_names[i] = categories[i]

    data["metadata"] = {"category_names": category_names}
    return data


def load_coco_dataset(path, image_folder):
    """
    Load COCO dataset (2017 version).
    COCO dataset consists in a JSON file describing annotations
    and an image folder.

    Args:
        path (str): path to the JSON annotation file
        image_folder (str): path to the image folder

    Returns:
        dict: Ikomia dataset structure. See :py:class:`~ikomia.dnn.datasetio.IkDatasetIO`.
    """
    data = {"images": [], "metadata": {}}

    if not image_folder.endswith("/"):
        image_folder += "/"

    with open(path, "r") as fp:
        dataset = json.load(fp)
        img_to_anns = defaultdict(list)
        cat_names = {0: "background"}

        if "annotations" in dataset:
            for ann in dataset["annotations"]:
                img_to_anns[ann["image_id"]].append(ann)

        if "categories" in dataset:
            for cat in dataset["categories"]:
                cat_names[cat["id"]] = cat["name"]

        data["metadata"] = {"category_names": cat_names}

        if "images" in dataset:
            for img in dataset["images"]:
                img_data = {}
                img_data["image_id"] = img["id"]
                img_data["filename"] = image_folder + img["file_name"]
                img_data["width"] = img["width"]
                img_data["height"] = img["height"]
                img_data["annotations"] = []

                for ann in img_to_anns[img["id"]]:
                    instance = {}
                    instance["category_id"] = ann["category_id"]
                    instance["iscrowd"] = ann["iscrowd"]

                    if "bbox" in ann:
                        instance["bbox"] = ann["bbox"]

                    if "segmentation" in ann:
                        # RLE mask not managed yet
                        if type(ann["segmentation"]) == list:
                            # Polygon
                            instance["segmentation_poly"] = ann["segmentation"]

                    img_data["annotations"].append(instance)

                if len(img_data["annotations"]) > 0:
                    data["images"].append(img_data)

    return data


def load_pascalvoc_dataset(annotation_folder, img_folder, instance_seg_folder, class_path):
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
    id = 0
    data = {"images": [], "metadata": {}}
    ann_root, ann_dirs, ann_files = next(os.walk(annotation_folder))

    if instance_seg_folder and not instance_seg_folder.endswith("/"):
        instance_seg_folder += "/"

    categories = read_class_names(class_path)
    category_ids = {}

    for i in range(len(categories)):
        category_ids[categories[i]] = i

    if annotation_folder != img_folder:
        img_root, img_dirs, img_files = next(os.walk(img_folder))
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
        img_data["image_id"] = id
        id = id + 1
        # Full path of the image
        img_data["filename"] = img_root + "/" + filename_node.text

        # Image size
        size_node = root.find("size")
        w_node = size_node.find("width")
        img_data["width"] = int(w_node.text)
        h_node = size_node.find("height")
        img_data["height"] = int(h_node.text)

        img_data["annotations"] = []
        for object in root.iter("object"):
            instance = {}
            # Category id
            instance["category_id"] = category_ids[object.find("name").text]
            # Bounding box
            bbox_node = object.find("bndbox")
            xmin = float(bbox_node.find("xmin").text)
            ymin = float(bbox_node.find("ymin").text)
            xmax = float(bbox_node.find("xmax").text)
            ymax = float(bbox_node.find("ymax").text)
            instance["bbox"] = [xmin, ymin, (xmax - xmin), (ymax - ymin)]
            img_data["annotations"].append(instance)

        if len(img_data["annotations"]) > 0:
            data["images"].append(img_data)

    category_names = {}
    for i in range(len(categories)):
        category_names[i] = categories[i]

    data["metadata"] = {"category_names": category_names}
    return data


def polygon_to_mask(polygons, width, height):
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
            y = poly[i+1]

            if x > width - 1:
                x = width - 1

            if y > height - 1:
                y = height - 1

            pts.append(core.CPointF(x, y))
            i += 2

        graphics_poly = core.CGraphicsPolygon(pts)
        graphics.append(graphics_poly)

    conversion = core.CGraphicsConversion(width, height)
    return conversion.graphicsToBinaryMask(graphics)
