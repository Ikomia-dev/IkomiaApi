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
Module dedicated to provide default implementation of
Ikomia Deep Learning dataset structure.
Derived from :py:class:`~ikomia.dataprocess.pydataprocess.CDatasetIO`.
"""

from ikomia import core, dataprocess
import random
import json


class IkDatasetIO(dataprocess.CDatasetIO):
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
                - keypoint_connection_rules (list[tuple(str, str, (r,g,b))]): each tuple specifies a pair of connected keypoints and the color to use for the line between them.
    """

    def __init__(self, format="other"):
        """
        Constructor

        Args:
            format (str): dataset source format.
        """
        dataprocess.CDatasetIO.__init__(self, format)
        # Data are stored into a dict
        self.data = {}
        self.category_colors = {}
        self.has_bckgnd_class = False

    def getImagePaths(self):
        """
        Return the list of all images path contained in the dataset.

        Returns:
            str[]: path list
        """
        paths = []

        for img_data in self.data["images"]:
            paths.append(img_data["filename"])

        return paths

    def getCategories(self):
        """
        Return the list of categories (ie instance classes) in the dataset.

        Returns:
            :py:class:`~ikomia.dataprocess.pydataprocess.MapIntStr` list: categories (dict-like structure)
        """
        categories = core.MapIntStr()
        for category_id in self.data["metadata"]["category_names"]:
            categories[category_id] = self.data["metadata"]["category_names"][category_id]

        return categories

    def getCategoryCount(self):
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

    def getMaskPath(self, img_path):
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
                elif semantic_key in img_data:
                    return img_data[semantic_key]
                else:
                    break

        return ""

    def getGraphicsAnnotations(self, img_path):
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
                                pts.append(core.CPointF(poly[i], poly[i + 1]))
                                i += 2

                            poly_prop = core.GraphicsPolygonProperty()
                            poly_prop.category = categ_name
                            poly_prop.pen_color = color
                            # opacity
                            alpha_color = color.copy()
                            alpha_color.append(100)
                            poly_prop.brush_color = alpha_color
                            graphics_poly = core.CGraphicsPolygon(pts, poly_prop)
                            graphics.append(graphics_poly)

                    # Bounding box
                    if "bbox" in annotation:
                        bbox = annotation["bbox"]

                        if len(bbox) == 4:
                            rect_prop = core.GraphicsRectProperty()
                            rect_prop.category = categ_name
                            rect_prop.pen_color = color
                            graphics_rect = core.CGraphicsRectangle(bbox[0], bbox[1], bbox[2], bbox[3], rect_prop)
                            text_prop = core.GraphicsTextProperty()
                            text_prop.color = color
                            graphics_text = core.CGraphicsText(categ_name, bbox[0], bbox[1], text_prop)
                            graphics.append(graphics_rect)
                            graphics.append(graphics_text)

        return graphics

    def isDataAvailable(self):
        """
        Check whether the dataset structure contains data.

        Return:
            boolean: True or False
        """
        if "images" in self.data:
            return len(self.data["images"]) > 0
        else:
            return False

    def clearData(self):
        """
        Clear whole dataset structure
        """
        self.data.clear()

    def _get_random_category_colors(self):
        random.seed(1)
        colors = {}

        for categ_id in self.data["metadata"]["category_names"]:
            color = [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]
            colors[categ_id] = color

        return colors

    def save(self, path):
        """
        Save dataset structure as JSON.

        Args:
            path: file path where dataset is saved
        """
        with open(path, "w") as outfile:
            json.dump(self.data, outfile)
