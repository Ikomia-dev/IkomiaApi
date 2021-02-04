import torch


def save_pth(model, path):
    torch.save(model.state_dict(), path)


def save_onnx(model, input_shape, device, path):
    dummy_input = torch.randn(input_shape, device=device)
    torch.onnx.export(model, dummy_input, path)
