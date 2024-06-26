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
Module providing tool functions for pytorch integration.
"""


import torch


def save_pth(model, path: str):
    """
    Save model in .pth.

    Args:
        model: pytorch model
        path: full path where model will be saved
    """
    torch.save(model.state_dict(), path)


def save_onnx(model, input_shape, device, path: str):
    """
    Save model in ONNX format.

    Args:
        model: pytorch model
        input_shape: input tensor shape
        device: torch device (cpu, cuda)
        path: full path where model will be saved
    """
    dummy_input = torch.randn(input_shape, device=device)
    torch.onnx.export(model, dummy_input, path)
