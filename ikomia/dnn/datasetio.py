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
Module dedicated to provide default implementation of
Ikomia Deep Learning dataset structure.
Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CDatasetIO`.
"""

import random
import json
from ikomia.core import (  # pylint: disable=E0611
    CPointF, GraphicsPolygonProperty, CGraphicsPolygon, GraphicsRectProperty, CGraphicsRectangle,
    GraphicsTextProperty, CGraphicsText, GraphicsPointProperty, CGraphicsPoint,
    GraphicsPolylineProperty, CGraphicsPolyline
)
from ikomia.dataprocess import CDatasetIO  # pylint: disable=E0611


class IkDatasetIO(CDatasetIO):
    """
    Define task input or output containing deep learning dataset structure.
    Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CDatasetIO`.

    Instances can be added as input or output of a :py:class:`~ikomia.core.pycore.CWorkflowTask` or derived object.
    Such input or output is required for deep learning training task.
    Dataset structure is composed of a global dict with 2 main entries
    'images' and 'metadata'.

    It follows the specifications below (mandatory fields may vary depending on the training goal).

        - images (list[dict]): image information and corresponding annotations.

            - filename (str): full path of the image file.
            - height, width (int): size of the image.
            - image_id (int): unique image identifier.
            - annotations (list[dict]): each dict corresponds to annotations of one instance in this image.

                - bbox (list[float]): x, y, width, height of the bounding box.
                - category_id (int): integer representing the category label.
                - segmentation_poly (list[list[float]]): list of polygons, one for each connected component.
                - keypoints (list[float]).
                - iscrowd (boolean): whether the instance is labelled as a crowd region (COCO).

            - segmentation_masks_np (numpy array [N, H, W]).
            - instance_seg_masks_file: full path of the ground truth instance segmentation image file.
            - semantic_seg_masks_file: full path of the ground truth semantic segmentation image file.

        - metadata (dict): key-value mapping that contains information that's shared among the entire dataset.

            - category_names (dict(id, name)).
            - category_colors (list[tuple(r,g,b)]).
            - keypoint_names (list[str]).
            - keypoint_connection_rules (list[tuple(str, str, (r,g,b))]): each tuple specifies a pair of connected
            keypoints and the color to use for the line between them.
    """

    def __init__(self, dataset_format: str = "other"):
        """
        Constructor

        Args:
            dataset_format (str): dataset source format.
        """
        CDatasetIO.__init__(self, dataset_format)
        # Data are stored into a dict
        self.data = {}
        self.category_colors = {}
        self.has_bckgnd_class = False

    def get_image_paths(self) -> list:
        """
        Return the list of all images path contained in the dataset.

        Returns:
            str[]: path list
        """
        paths = []

        for img_data in self.data["images"]:
            paths.append(img_data["filename"])

        return paths

    def get_categories(self) -> dict:
        """
        Return the list of categories (ie instance classes) in the dataset.

        Returns:
            categories (dict)
        """
        categories = {}
        for category_id in self.data["metadata"]["category_names"]:
            categories[category_id] = self.data["metadata"]["category_names"][category_id]

        return categories

    def get_category_count(self) -> int:
        """
        Return the number of categories (ie instance classes) in the dataset.

        Returns:
            int
        """
        category_max = 0
        for category_id in self.data["metadata"]["category_names"]:
            if category_id > category_max:
                category_max = category_id

        return category_max + 1

    def get_mask_path(self, img_path: str) -> str:
        """
        Return the file path of the segmentation mask for the given image.

        Args:
            img_path (str): path of the image from which segmentation mask is requested

        Returns:
            str: file path of the segmentation mask
        """
        instance_key = "instance_seg_masks_file"
        semantic_key = "semantic_seg_masks_file"

        for img_data in self.data["images"]:
            if img_data["filename"] == img_path:
                if instance_key in img_data:
                    return img_data[instance_key]
                if semantic_key in img_data:
                    return img_data[semantic_key]

                break

        return ""

    def get_graphics_annotations(self, img_path: str) -> list:
        """
        Return a list of Ikomia graphics items corresponding
        to the annotations of a given image (bounding box, polygons).

        Args:
            img_path (str): path of the image from which we want annotations

        Returns:
            :py:class:`~ikomia.core.pycore.CGraphicsItem` list: graphics items
        """
        graphics = []
        if len(self.category_colors) == 0:
            self.category_colors = self._get_random_category_colors()

        for img_data in self.data["images"]:
            if img_data["filename"] == img_path:
                for annotation in img_data["annotations"]:
                    # Category name
                    categ_id = annotation["category_id"]
                    color = self.category_colors[categ_id]

                    if categ_id in self.data["metadata"]["category_names"]:
                        categ_name = self.data["metadata"]["category_names"][categ_id]
                    else:
                        categ_name = ""

                    # Polygons
                    if "segmentation_poly" in annotation:
                        polygons = annotation["segmentation_poly"]

                        for poly in polygons:
                            pts = []
                            i = 0
                            while i < len(poly):
                                pts.append(CPointF(poly[i], poly[i + 1]))
                                i += 2

                            poly_prop = GraphicsPolygonProperty()
                            poly_prop.category = categ_name
                            poly_prop.pen_color = color
                            # opacity
                            alpha_color = color.copy()
                            alpha_color.append(100)
                            poly_prop.brush_color = alpha_color
                            graphics_poly = CGraphicsPolygon(pts, poly_prop)
                            graphics.append(graphics_poly)

                    # Bounding box
                    if "bbox" in annotation:
                        bbox = annotation["bbox"]

                        if len(bbox) == 4:
                            rect_prop = GraphicsRectProperty()
                            rect_prop.category = categ_name
                            rect_prop.pen_color = color
                            graphics_rect = CGraphicsRectangle(bbox[0], bbox[1], bbox[2], bbox[3], rect_prop)
                            text_prop = GraphicsTextProperty()
                            text_prop.color = color
                            graphics_text = CGraphicsText(categ_name, bbox[0], bbox[1], text_prop)
                            graphics.append(graphics_rect)
                            graphics.append(graphics_text)

                    if "keypoints" in annotation:
                        kp = annotation["keypoints"]
                        kp_names_id = {pt_name: pt_id for pt_id, pt_name in
                                       enumerate(self.data["metadata"]["keypoint_names"])}
                        kp_id_names = {pt_id: pt_name for pt_id, pt_name in
                                       enumerate(self.data["metadata"]["keypoint_names"])}

                        num_kp = len(kp_names_id)
                        kp_connection_rules = self.data["metadata"]["keypoint_connection_rules"]
                        assert num_kp * 3 == len(kp)
                        pt_prop = GraphicsPointProperty()
                        pt_prop.pen_color = color
                        for i, (x, y, v) in enumerate(zip(kp[0::3], kp[1::3], kp[2::3])):
                            if v != 0:
                                pt_prop.category = kp_id_names[i]
                                graphics_point = CGraphicsPoint(CPointF(x, y), pt_prop)
                                graphics.append(graphics_point)
                        for named_pt1, named_pt2, c in kp_connection_rules:
                            id_pt1 = kp_names_id[named_pt1]
                            id_pt2 = kp_names_id[named_pt2]
                            if kp[id_pt1 * 3 + 2] != 0 and kp[id_pt2 * 3 + 2] != 0:
                                line_prop = GraphicsPolylineProperty()
                                line_prop.pen_color = list(c)
                                pt1 = CPointF(kp[id_pt1 * 3], kp[id_pt1 * 3 + 1])
                                pt2 = CPointF(kp[id_pt2 * 3], kp[id_pt2 * 3 + 1])
                                graphics_kp_poly = CGraphicsPolyline([pt1, pt2], line_prop)
                                graphics.append(graphics_kp_poly)

        return graphics

    def is_data_available(self) -> bool:
        """
        Check whether the dataset structure contains data.

        Returns:
            boolean: True or False
        """
        if "images" in self.data:
            return len(self.data["images"]) > 0

        return False

    def clear_data(self):
        """
        Clear whole dataset structure
        """
        self.data.clear()

    def _get_random_category_colors(self) -> dict:
        random.seed(1)
        colors = {}
        for categ_id in self.data["metadata"]["category_names"]:
            color = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]
            colors[categ_id] = color
        return colors

    def save(self, path: str):
        """
        Save dataset structure as JSON.

        Args:
            path: file path where dataset is saved
        """
        with open(path, "w", encoding="utf-8") as outfile:
            json.dump(self.data, outfile)

    def load(self, path: str):
        """
        Load JSON as dataset structure.

        Args:
            path: file path where dataset is saved
        """
        with open(path, "r", encoding="utf-8") as infile:
            self.data = json.load(infile)
            if "category_names" in self.data["metadata"]:
                self.data["metadata"]["category_names"] = {int(k): v for k, v in
                                                           self.data["metadata"]["category_names"].items()}

    def to_json(self, options: list = None) -> str:
        """
        Convert dataset i/o data to JSON formatted string.

        Returns:
            str: JSON formatted string
        """
        del options
        return json.dumps(self.data)

    def from_json(self, json_string: str):
        """
        Set dataset i/o data from JSON formatted string.

        Args:
            json_string (str): data as JSON formatted string
        """
        self.data = json.loads(json_string)
