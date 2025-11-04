<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/Ikomia-dev/IkomiaAPI">
    <img src="https://avatars.githubusercontent.com/u/53618017?s=400&u=e9c62c77b7c33b6b7f4883b115a0d7d05dcca9ec&v=4" alt="Logo" width="100" height="100">
  </a>
  <h3 align="center">Ikomia: Simplifying Computer Vision Deployment</h3>
</div>
<br />
<p align="center">
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/stargazers">
        <img alt="Stars" src="https://img.shields.io/github/stars/Ikomia-dev/IkomiaAPI">
    </a>
    <a href="https://www.ikomia.ai/api/">
        <img alt="Website" src="https://img.shields.io/website/https/ikomia.ai.svg?down_color=red&down_message=offline&up_message=online">
    </a>
    <a href="">
        <img alt="OS" src="https://img.shields.io/badge/os-win%2C%20linux-9cf">
    </a>
    <a href="">
        <img alt="Python" src="https://img.shields.io/badge/python-3.9%2C%203.10%2C%203.11%2C%203.12-blueviolet">
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

Welcome to Ikomia, where we transform intricate research algorithms into user-friendly, deployable solutions for computer vision enthusiasts and professionals alike.

## **🌟 Why Choose Ikomia?**

- **Research Meets Reality**: We bridge the gap between cutting-edge research and real-world applications. With Ikomia, you get access to algorithms from renowned sources like OpenCV, Detectron2, OpenMMLab, and Hugging Face.
  
- **Unified Framework**: Say goodbye to integration complexities. Craft workflows and blend algorithms seamlessly, all under one roof.
  
- **Empowerment**: We're not just about providing tools; we're about building a community. By democratizing AI and computer vision technologies, we aim to foster collaboration and innovation.

## **🚀 Getting Started**

### **Installation**
```bash
pip install ikomia
```

### **Quick Examples**

#### Object Detection
```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils.displayIO import display

wf = Workflow()
yolov7 = wf.add_task(name="infer_yolo_v7", auto_connect=True)
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")
display(yolov7.get_image_with_graphics())
```
<p float="left">
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg" width="400" /> 
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman_bbox.png" width="400" />
</p>

#### Pose Estimation
```python
# ... [similar imports]
wf = Workflow()
pose_estimation = wf.add_task(name="infer_mmlab_pose_estimation", auto_connect=True)
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")
display(pose_estimation.get_image_with_graphics())
```
<p float="left">
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg" width="400" /> 
  <img src="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman_pose.png" width="400" />
</p>

### **Discover with 'ik'**
Our auto-completion system, 'ik', is designed to assist developers in discovering available algorithms in Ikomia HUB. Dive into our detailed documentation to explore its capabilities.

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

## **📦 Exporting Your Workflow**

With Ikomia, sharing your crafted workflows is a breeze. Whether you want to collaborate with peers or integrate with Ikomia STUDIO, our export feature has got you covered.

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik

wf = Workflow("Instance Segmentation with YOLOv7")
yolov7 = wf.add_task(ik.infer_yolo_v7_instance_segmentation(), auto_connect=True)
filter_task = wf.add_task(ik.ik_instance_segmentation_filter(categories="dog", confidence="0.90"), auto_connect=True)
wf.save("path/to/your_workflow.json")
```

Once you've exported your workflow, you can easily share it with others, ensuring reproducibility and collaboration.

## **🧪 Notebooks**

You can find some notebooks [here](https://github.com/Ikomia-dev/notebooks).

We provide some Google Colab tutorials:
Notebooks | Google Colab
-- | --- 
[How to make a simple workflow](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_make_a_simple_workflow_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_make_a_simple_workflow_with_Ikomia_API.ipynb)
[How to run Neural Style Transfer](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_run_Neural_Style_Transfer_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_run_Neural_Style_Transfer_with_Ikomia_API.ipynb) 
[How to train and run YOLO v7 on your datasets](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_train_YOLO_v7_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_train_YOLO_v7_with_Ikomia_API.ipynb)
[How to use Detectron2 Object Detection](https://github.com/Ikomia-dev/notebooks/blob/main/examples/HOWTO_use_Detectron2_Object_Detection_with_Ikomia_API.ipynb) | [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/Ikomia-dev/notebooks/blob/main/examples/HOWTO_use_Detectron2_Object_Detection_with_Ikomia_API.ipynb)

## **📚 Comprehensive Documentation**

For those who love details, our [comprehensive documentation](https://ikomia-dev.github.io/python-api-documentation/) is a treasure trove of information. From basic setups to advanced configurations, we've got you covered.

## **🤝 Contributing**

We believe in the power of community. If you have suggestions, improvements, or want to contribute in any way, we're all ears! Stay tuned for our detailed contribution guidelines.

## **📜 License**

We believe in open-source. Ikomia is licensed under the Apache-2.0 License, promoting collaboration with transparency.

## **💖 Support & Feedback**

Your feedback drives our progress. If you find Ikomia useful, give us a :star:! For queries, issues, or just to say hi, drop us an email at team@ikomia.com or join our [discord channel](https://discord.com/invite/82Tnw9UGGc).

<!-- STARGAZERS -->
## They like us, we love them  :heart_eyes:

[![Stargazers repo roster for @Ikomia-dev/IkomiaApi](http://reporoster.com/stars/Ikomia-dev/IkomiaApi)](https://github.com/Ikomia-dev/IkomiaApi/stargazers)

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

