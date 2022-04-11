# Ikomia API

![](https://ikomia.com/static/showcase/img/home/plugin.png)

This project provides Python API to leverage state of the art algorithms for your own computer vision application. Ikomia API is framework-agnostic, algorithms can be implemented in your favorite Python library (PyTorch, TensorFlow, Scikit-image ...).

You will be led to use this API for two reasons:
- **Create plugins for Ikomia platform**: with a little wrap around your Python code, you are able to use your algorithm from a user friendly software [Ikomia Studio](https://github.com/Ikomia-dev/IkomiaStudio) and a flexible workflow API (see below). Moreover, from Ikomia Studio you can publish your work to the Marketplace in one click and share it with the community.
- **Use Ikomia algorithms and workflows in your custom applications**: leverage built-in algorithms, yours and those from the Marketplace directly in your application. Run single algorithm or workflows with few lines of code.


> **_Note:_**  This Python API is built on top of a C++ core framework bundled in the provided wheels. This core framework handles plugins connection, workflow management, generic execution runtime and Python bindings.


## Installation

You simply install Ikomia API via pip for Linux and Windows (CPU only):

`pip install ikomia`

With GPU enabled:

`pip install ikomia==0.6.2+cu11 -f https://ikomia-packages.s3-website.fr-par.scw.cloud/ikomia/index.html`

>**_Note:_** we provide Python wheel for Python 3.7, 3.8 and 3.9. It includes OpenCV package (v4.5.2) with CUDA enabled (v11), CuDNN 8. Thus, you must have CUDA 11 drivers for your graphic card and install machine learning framework compatible with CUDA 11. For Linux users, the wheel targets the platform manylinux2014 so that it should be compatible with most of recent distributions. For Windows, the wheel is only compatible with Windows 10.


>**Important**: A GPU-enabled version of OpenCV is bundled within the Ikomia GPU package. It includes all the core and contribution librairies with CUDA support. There is only one important constraint to respect, **you must import Ikomia module before importing cv2 module**. As a result, the package *opencv-python* from Pypi repository must not be installed in 
the same Python environment. This will lead to incompatibility issues.


## Documentation

Python API documentation can be found [here](https://ikomia-dev.github.io/python-api-documentation/). You will find plugin examples in [our main GitHub account](https://github.com/Ikomia-dev).
