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
Module providing implementation of a PyTorch dataset mapper for
Ikomia dataset structure. It converts the Ikomia dataset structure to a
PyTorch compatible dataset so that it is possible to use Ikomia dataset
loaders with PyTorch Deep Learning models.
"""

import torch
import os
from torch.utils.data import Dataset
from PIL import Image
import numpy as np
import ikomia.dnn.dataset as ikdataset


class TorchDatasetMapper(Dataset):
    """
    Class derived from torch.utils.data.Dataset.
    """

    def __init__(self, ik_dataset, has_bckgnd_class, transforms):
        self.ik_dataset = ik_dataset
        self.has_bckgnd_class = has_bckgnd_class
        self.transforms = transforms

    def __len__(self):
        return len(self.ik_dataset["images"])

    def __getitem__(self, idx):
        img_data = self.ik_dataset["images"][idx]
        # Load image
        img = Image.open(img_data["filename"]).convert("RGB")
        # Get bounding boxes
        boxes = []
        labels = []
        target = {}
        iscrowd = []
        np_masks = None
        instance_index = 0

        if "annotations" in img_data:
            for annotation in img_data["annotations"]:
                xmin = annotation["bbox"][0]
                ymin = annotation["bbox"][1]
                w = annotation["bbox"][2]
                h = annotation["bbox"][3]
                boxes.append([xmin, ymin, xmin+w, ymin+h])

                if self.has_bckgnd_class:
                    labels.append(annotation["category_id"])
                else:
                    # 0 = background
                    labels.append(annotation["category_id"] + 1)

                if "iscrowd" in annotation:
                    iscrowd.append(annotation["iscrowd"])
                else:
                    iscrowd.append(0)

                if "segmentation_poly" in annotation:
                    w = img_data["width"]
                    h = img_data["height"]

                    if np_masks is None:
                        np_masks = np.empty((len(img_data["annotations"]), h, w), dtype=np.uint8)

                    mask = ikdataset.polygon_to_mask(annotation["segmentation_poly"], w, h)
                    mask.reshape((1, h, w))
                    np_masks[instance_index, :, :] = mask

                instance_index += 1

            # Convert everything into a torch.Tensor
            boxes = torch.as_tensor(boxes, dtype=torch.float32)
            labels = torch.as_tensor(labels, dtype=torch.int64)
            image_id = torch.tensor([idx])
            area = (boxes[:, 3] - boxes[:, 1]) * (boxes[:, 2] - boxes[:, 0])
            iscrowd = torch.as_tensor(iscrowd, dtype=torch.int64)

            # Put everything in a dict
            target = {"boxes": boxes, "labels": labels, "image_id": image_id, "area": area, "iscrowd": iscrowd}

        # Segmentation masks
        if np_masks is not None:
            target["masks"] = torch.tensor(np_masks, dtype=torch.uint8)
        elif "segmentation_masks_np" in img_data:
            masks = torch.tensor(img_data["segmentation_masks_np"], dtype=torch.uint8)
            target["masks"] = masks
        elif "instance_seg_masks_file" in img_data:
            masks = self._load_instance_seg_masks(img_data["instance_seg_masks_file"])
            target["masks"] = torch.tensor(masks, dtype=torch.uint8)
        elif "semantic_seg_masks_file" in img_data:
            masks = self._load_semantic_seg_masks(img_data["semantic_seg_masks_file"])
            target["masks"] = torch.tensor(masks, dtype=torch.uint8)

        if self.transforms is not None:
            img, target = self.transforms(img, target)

        return img, target

    def _load_instance_seg_masks(self, path):
        # Load mask as [N H W] numpy array
        filename, extension = os.path.splitext(path)

        if extension == ".npz":
            # 3D numpy array saves as compressed npz format
            mask = np.transpose(np.load(path)['arr_0'], (2, 0, 1))
            mask8 = mask.astype(np.uint8)
            return mask8
        elif extension == ".png":
            # Labelled mask (one pixel value per object)
            labelled_mask = np.array(Image.open(path))
            labels = np.unique(labelled_mask)
            mask = np.empty((len(labels), labelled_mask.shape[0], labelled_mask.shape[1]), dtype=np.uint8)
            index = 0

            for label in labels:
                if label != 255:
                    mask[index, :, :] = labelled_mask == label
                index += 1

            return mask
        else:
            print("Error: segmentation mask format not supported")
            return None

    def _load_semantic_seg_masks(self, path):
        # Load mask as [N H W] numpy array
        filename, extension = os.path.splitext(path)

        if extension == ".png":
            # Labelled mask (one pixel value per category)
            labelled_mask = np.array(Image.open(path))
            mask = labelled_mask.reshape(1, labelled_mask.shape[0], labelled_mask.shape[1])
            return mask
        else:
            print("Error: segmentation mask format not supported")
            return None
