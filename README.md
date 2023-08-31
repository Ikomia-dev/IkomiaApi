<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/Ikomia-dev/IkomiaAPI">
    <img src="https://avatars.githubusercontent.com/u/53618017?s=400&u=e9c62c77b7c33b6b7f4883b115a0d7d05dcca9ec&v=4" alt="Logo" width="100" height="100">
  </a>
  <h3 align="center">Build and deploy Computer Vision solutions with a few lines of code</h3>
</div>
<br />
<p align="center">
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/stargazers">
        <img alt="Stars" src="https://img.shields.io/github/stars/Ikomia-dev/IkomiaAPI">
    </a>
    <a href="https://www.ikomia.ai/api/">
        <img alt="Website" src="https://img.shields.io/website/http/ikomia.ai/en.svg?down_color=red&down_message=offline&up_message=online">
    </a>
    <a href="">
        <img alt="OS" src="https://img.shields.io/badge/os-win%2C%20linux-9cf">
    </a>
    <a href="">
        <img alt="Python" src="https://img.shields.io/badge/python-3.7%2C%203.8%2C%203.9%2C%203.10-blueviolet">
    </a>
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/blob/main/LICENSE.md">
        <img alt="GitHub" src="https://img.shields.io/github/license/Ikomia-dev/IkomiaAPI.svg?color=blue">
    </a>
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/tags">
        <img alt="GitHub tags" src="https://img.shields.io/github/v/release/Ikomia-dev/IkomiaAPI.svg?color=red">
    </a>
    <br>
    <a href="https://discord.com/invite/82Tnw9UGGc">
        <img alt="Discord community" src="https://img.shields.io/badge/Discord-white?style=social&logo=discord">
    </a> 
</p>

<p align="center">
  <kbd>
    <img src="https://user-images.githubusercontent.com/42171814/200714085-399b7625-81ae-4c71-bb39-8483bf4e204e.gif"/>
  </kbd>
</p>


<!-- ABOUT THE PROJECT -->
## About The Project

At Ikomia, we deeply believe that sharing scientific knowledge is the key to success. This belief drives us to make research-based algorithms ready-to-use for developers through our Python library (Ikomia API) and desktop software ([Ikomia STUDIO](https://github.com/Ikomia-dev/IkomiaStudio)).

Our main goal is to convert existing Python code into easily accessible and deployable algorithms. This approach enables us to integrate repositories from researchers, labs, and renowned frameworks like [OpenCV](https://github.com/opencv/opencv), [Detectron2](https://github.com/facebookresearch/detectron2), [OpenMMLab](https://github.com/open-mmlab) and [Hugging Face](https://github.com/huggingface). 
Within a unified framework, developers can craft workflows and seamlessly blend these cutting-edge algorithms, streamlining the integration process by eliminating its complexities.

By democratizing advanced technologies, we strive to accelerate innovation in AI, computer vision, and machine learning. Our platform empowers developers to leverage cutting-edge algorithms effortlessly, thereby fostering collaboration and transformative applications.

<!-- GETTING STARTED -->
## Getting Started


### Installation

`pip install ikomia`

<!-- USAGE EXAMPLES -->
### Usage 1 : Object Detection Example

With Ikomia, when you want to use an algorithm, it's always the same code pattern which is useful when you want to test multiple algorithms effortlessly.

``` python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils.displayIO import display

# Init your workflow
wf = Workflow()

# Add YOLO and connect it to your input data
yolov7 = wf.add_task(name="infer_yolo_v7", auto_connect=True)

# Run directly on your image
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")

# YOLO output image with bounding boxes
display(yolov7.get_image_with_graphics())
```
<p float="left">
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg" width="400" /> 
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman_bbox.png" width="400" />
</p>


And finally, you can also export your results as JSON files.
``` python
# Get all object detection
json_results = yolov7.get_results().to_json()
print(json_results)
```
### Usage 2 : Pose Estimation Example

``` python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils.displayIO import display

# Init your workflow
wf = Workflow()

# Add YOLO and connect it to your input data
yolov7 = wf.add_task(name="infer_mmlab_pose_estimation", auto_connect=True)

# Run directly on your image
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")

# YOLO output image with bounding boxes
display(yolov7.get_image_with_graphics())
```
<p float="left">
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg" width="400" /> 
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman_pose.png" width="400" />
</p>


### Usage with the `ik` auto-completion system

`ik` is an auto-completion system designed to help developers to find available algorithms in [Ikomia HUB](https://github.com/Ikomia-hub). See the documentation for more explanations [here](https://ikomia-dev.github.io/python-api-documentation/getting_started.html).

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik
from ikomia.utils.displayIO import display

wf = Workflow()

yolov7 = wf.add_task(ik.infer_yolo_v7_instance_segmentation(), auto_connect=True)

# wf.run_on(path="path/to/your/image.png")
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_dog.png")

display(yolov7.get_image_with_graphics())
display(yolov7.get_image_with_mask())
display(yolov7.get_image_with_mask_and_graphics())
```
![](https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/display_inst_seg.png)

### Export your workflow

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik

wf = Workflow("Dog instance segmentation with YOLOv7")

yolov7 = wf.add_task(ik.infer_yolo_v7_instance_segmentation(), auto_connect=True)
filter = wf.add_task(ik.ik_instance_segmentation_filter(categories="dog", confidence="0.90"), auto_connect=True)

wf.save("path/to/your_workflow.json")
```
Once you export your workflow, you can share it with others, use it with [Ikomia STUDIO](https://github.com/Ikomia-dev/IkomiaStudio) or push it on our SaaS platform for deployment (contact us at team@ikomia.ai for more information).

<!-- EXAMPLES -->
### Examples

You can find some notebooks [here](https://github.com/Ikomia-dev/notebooks).

We provide some Google Colab tutorials:
Notebooks | Google Colab
-- | --- 
[How to make a simple workflow](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_make_a_simple_workflow_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_make_a_simple_workflow_with_Ikomia_API.ipynb)
[How to run Neural Style Transfer](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_run_Neural_Style_Transfer_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_run_Neural_Style_Transfer_with_Ikomia_API.ipynb) 
[How to train and run YOLO v7 on your datasets](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_train_YOLO_v7_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_train_YOLO_v7_with_Ikomia_API.ipynb)
[How to use Detectron2 Object Detection](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_use_Detectron2_Object_Detection_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_use_Detectron2_Object_Detection_with_Ikomia_API.ipynb)
 

<!-- DOCUMENTATION -->
## Documentation

Python API documentation can be found [here](https://ikomia-dev.github.io/python-api-documentation/). 

You will find Ikomia HUB algorithms code source in [our Ikomia HUB GitHub](https://github.com/Ikomia-hub) and all algorithms on [our website](https://app.ikomia.ai/hub/).

<!-- CONTRIBUTING -->
## Contributing

This part is coming soon...:)

<!-- LICENSE -->
## License

Distributed under the Apache-2.0 License. See `LICENSE.md` for more information.


<!-- STARGAZERS -->
## They like us, we love them  :heart_eyes:

[![Stargazers repo roster for @Ikomia-dev/IkomiaApi](https://reporoster.com/stars/Ikomia-dev/IkomiaApi)](https://github.com/Ikomia-dev/IkomiaApi/stargazers)

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=Ikomia-dev/IkomiaAPI&type=Date)](https://star-history.com/#Ikomia-dev/IkomiaAPI&Date)

<!-- CITATION -->
## Citing Ikomia

If you use Ikomia in your research, please use the following BibTeX entry.

```BibTeX
@misc{DeBa2019Ikomia,
  author =       {Guillaume Demarcq and Ludovic Barusseau},
  title =        {Ikomia},
  howpublished = {\url{https://github.com/Ikomia-dev/IkomiaAPI}},
  year =         {2019}
}
```

<!-- SUPPORT -->
## Support

Contributions, issues, and feature requests are welcome!
Give a :star: if you like this project!


<!-- CONTACT -->
## Contact

Ikomia - [@IkomiaOfficial](https://twitter.com/ikomiaofficial) - team@ikomia.com

Project Link: [https://github.com/Ikomia-dev/IkomiaAPI](https://github.com/Ikomia-dev/IkomiaAPI)

