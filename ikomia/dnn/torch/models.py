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
Model providing helper funtions to create TorchVision Deep Learning models.

TorchVision pages:

- `Torchvision classication <https://pytorch.org/docs/stable/torchvision/models.html#classification>`_
- `Torchvision object detection and segmentation <https://pytorch.org/docs/stable/torchvision/models.html#object-detection-instance-segmentation-and-person-keypoint-detection>`_
"""

import torch
from torchvision import models


def set_parameter_requires_grad(model, feature_extracting):
    if feature_extracting:
        for param in model.parameters():
            param.requires_grad = False


def resnet(model_name='resnet50', train_mode=False, use_pretrained=False, feature_extract=False, classes=2):
    """
    Create Torchvision ResNet model for training or inference.

    Args:
        model_name (str): model name
        train_mode (boolean): True or False
        use_pretrained (boolean): True to do transfer learning from pre-trained model, False to train from scratch
        feature_extract (boolean): transfer learning only, True to keep pre-trained features (train last layers only), False to train all layers
        classes (int): number of classes in the dataset

    Returns:
        model object
    """
    model_ft = None

    if model_name == "resnet18":
        model_ft = models.resnet18(pretrained=use_pretrained)
    elif model_name == "resnet34":
        model_ft = models.resnet34(pretrained=use_pretrained)
    elif model_name == "resnet50":
        model_ft = models.resnet50(pretrained=use_pretrained)
    elif model_name == "resnet101":
        model_ft = models.resnet101(pretrained=use_pretrained)
    elif model_name == "resnet152":
        model_ft = models.resnet152(pretrained=use_pretrained)
    else:
        print("Invalid model name, exiting...")
        exit()

    if train_mode:
        set_parameter_requires_grad(model_ft, feature_extract)

    if train_mode or not use_pretrained:
        num_features = model_ft.fc.in_features
        model_ft.fc = torch.nn.Linear(num_features, classes)

    if not train_mode:
        model_ft.eval()

    # Print the model we just instantiated
    print(model_ft)
    return model_ft


def resnext(model_name='resnext50', train_mode=False, use_pretrained=False, feature_extract=False, classes=2):
    """
    Create Torchvision ResNeXt model for training or inference.

    Args:
        model_name (str): model name
        train_mode (boolean): True or False
        use_pretrained (boolean): True to do transfer learning from pre-trained model, False to train from scratch
        feature_extract (boolean): transfer learning only, True to keep pre-trained features (train last layers only), False to train all layers
        classes (int): number of classes in the dataset

    Returns:
        model object
    """
    model_ft = None

    if model_name =='resnext50':
        model_ft = models.resnext50_32x4d(pretrained=use_pretrained)
    elif model_name == 'resnext101':
        model_ft = models.resnext101_32x8d(pretrained=use_pretrained)
    else:
        print("Invalid model name, exiting...")
        exit()

    if train_mode:
        set_parameter_requires_grad(model_ft, feature_extract)

    if train_mode or not use_pretrained:
        num_features = model_ft.fc.in_features
        model_ft.fc = torch.nn.Linear(num_features, classes)

    if not train_mode:
        model_ft.eval()

    # Print the model we just instantiated
    print(model_ft)
    return model_ft


def mnasnet(train_mode=False, use_pretrained=False, feature_extract=False, classes=2):
    """
    Create Torchvision MnasNet model for training or inference.

    Args:
        model_name (str): model name
        train_mode (boolean): True or False
        use_pretrained (boolean): True to do transfer learning from pre-trained model, False to train from scratch
        feature_extract (boolean): transfer learning only, True to keep pre-trained features (train last layers only), False to train all layers
        classes (int): number of classes in the dataset

    Returns:
        model object
    """
    model_ft = models.mnasnet1_0(pretrained=use_pretrained)

    if train_mode:
        set_parameter_requires_grad(model_ft, feature_extract)

    if train_mode or not use_pretrained:
        num_features = model_ft.classifier[1].in_features
        model_ft.classifier[1] = torch.nn.Linear(num_features, classes)

    if not train_mode:
        model_ft.eval()

    # Print the model we just instantiated
    print(model_ft)
    return model_ft


def faster_rcnn(train_mode=False, use_pretrained=True, input_size=800, classes=2):
    """
    Create Torchvision Faster RCNN model for training or inference.

    Args:
        model_name (str): model name
        train_mode (boolean): True or False
        use_pretrained (boolean): True to do transfer learning from pre-trained model, False to train from scratch
        input_size (int): input image size
        classes (int): number of classes in the dataset

    Returns:
        model object
    """
    # Transfer learning only
    args = {"min_size": input_size}
    model_ft = models.detection.fasterrcnn_resnet50_fpn(pretrained=True, **args)

    if train_mode or not use_pretrained:
        in_features = model_ft.roi_heads.box_predictor.cls_score.in_features
        model_ft.roi_heads.box_predictor = models.detection.faster_rcnn.FastRCNNPredictor(in_features, classes)

    if not train_mode:
        model_ft.eval()

    print(model_ft)
    return model_ft


def mask_rcnn(train_mode=False, use_pretrained=True, input_size=800, classes=2):
    """
    Create Torchvision Mask RCNN model for training or inference.

    Args:
        model_name (str): model name
        train_mode (boolean): True or False
        use_pretrained (boolean): True to do transfer learning from pre-trained model, False to train from scratch
        input_size (int): input image size
        classes (int): number of classes in the dataset

    Returns:
        model object
    """
    # Transfer learning only
    args = {"min_size": input_size}
    model_ft = models.detection.maskrcnn_resnet50_fpn(pretrained=True, **args)

    if train_mode or not use_pretrained:
        in_features = model_ft.roi_heads.box_predictor.cls_score.in_features
        model_ft.roi_heads.box_predictor = models.detection.faster_rcnn.FastRCNNPredictor(in_features, classes)
        in_features_mask = model_ft.roi_heads.mask_predictor.conv5_mask.in_channels
        hidden_layer = 256
        model_ft.roi_heads.mask_predictor = models.detection.mask_rcnn.MaskRCNNPredictor(in_features_mask,
                                                                                         hidden_layer,
                                                                                         classes)

    if not train_mode:
        model_ft.eval()

    print(model_ft)
    return model_ft
