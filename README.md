<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<div align="center">
  <a href="https://github.com/Ikomia-dev/IkomiaAPI">
    <img src="https://avatars.githubusercontent.com/u/53618017?s=400&u=e9c62c77b7c33b6b7f4883b115a0d7d05dcca9ec&v=4" alt="Logo" width="100" height="100">
  </a>
  <h3 align="center">State-of-the-art Computer Vision with a few lines of code</h3>
  <a href="https://github.com/Ikomia-dev/IkomiaAPI/issues/new?assignees=&labels=bug&template=01_BUG_REPORT.md&title=bug%3A+">Report a Bug</a>
  ·
  <a href="https://github.com/Ikomia-dev/IkomiaAPI/issues/new?assignees=&labels=enhancement&template=02_FEATURE_REQUEST.md&title=feat%3A+">Request a Feature</a>
  .
  <a href="https://github.com/Ikomia-dev/IkomiaAPI/discussions">Ask a Question</a>
</div>
<br />
<p align="center">
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/stargazers">
        <img alt="Stars" src="https://img.shields.io/github/stars/Ikomia-dev/IkomiaAPI">
    </a>
    <a href="https://ikomia.com/en/computer-vision-api/">
        <img alt="Website" src="https://img.shields.io/website/http/ikomia.com/en.svg?down_color=red&down_message=offline&up_message=online">
    </a>
    <a href="">
        <img alt="Python" src="https://img.shields.io/badge/os-win%2C%20linux-9cf">
    </a>
    <a href="">
        <img alt="Python" src="https://img.shields.io/badge/python-3.7%2C%203.8%2C%203.9-blueviolet">
    </a>
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/blob/main/LICENSE.md">
        <img alt="GitHub" src="https://img.shields.io/github/license/Ikomia-dev/IkomiaAPI.svg?color=blue">
    </a>
    <a href="https://github.com/Ikomia-dev/IkomiaAPI/tags">
        <img alt="GitHub tags" src="https://img.shields.io/github/v/release/Ikomia-dev/IkomiaAPI.svg?color=red">
    </a>
    <br>
    <a href="https://join.slack.com/t/ikomia-community/shared_invite/zt-1ie9k8lw7-IsBaJ~nhGZyc0QfkYO3olA">
        <img alt="Slack community" src="https://img.shields.io/badge/Slack-white?style=social&logo=slack">
    </a> 
</p>

<p align="center">
  <kbd>
    <img src="https://user-images.githubusercontent.com/42171814/200714085-399b7625-81ae-4c71-bb39-8483bf4e204e.gif"/>
  </kbd>
</p>


<!-- ABOUT THE PROJECT -->
## About The Project

Ikomia API is an **open source tool** to easily build and deploy your Computer Vision solutions. You can mix your preferred frameworks such as **OpenCV**, **Detectron2**, **OpenMMLab** or **YOLO** with the best state-of-the-art algorithms from individual repos.

No effort, just choose what you want and Ikomia downloads it, installs the requirements and runs everything in a few lines of code.


<p align="center">
  <kbd>
    <img src="https://user-images.githubusercontent.com/42171814/203433920-3d2745c6-4dc1-4e5e-8cd9-63d215a8d80e.gif"/>
    <img src="https://user-images.githubusercontent.com/42171814/200714144-59de3910-3926-4665-b8b0-af6eadfa6ae3.png"/>
  </kbd>
</p>

<!-- GETTING STARTED -->
## Getting Started


### Installation

`pip install ikomia`

<!-- USAGE EXAMPLES -->
### Usage

Ikomia API has already more than 180 pre-integrated algorithms (mainly OpenCV) but the most interesting algorithms are in [Ikomia HUB](https://github.com/Ikomia-hub).
That's why, you need to connect to Ikomia HUB when you want to download/install these algorithms.

You can use our demo credentials below or you can get yours for free and join our community [here](https://members.ikomia.com/accounts/signup/) :)

Ikomia authentication is based on two environment variables: IKOMIA_USER=your_login and IKOMIA_PWD=your_password, so you can set these variables by command-lines or use this code snippet:

``` python
import os
import ikomia

os.environ['IKOMIA_USER'] = "demo"
os.environ['IKOMIA_PWD'] = "jH4q72DApbRPa4k"

ikomia.authenticate()

```
When you want to use an algorithm, it's always the same code pattern which is useful when you want to test multiple algorithms effortlessly.

``` python
from ikomia.dataprocess import workflow

# Init your workflow
wf = workflow.create("YOLO inference")

# Add YOLO and connect it to your input data
yolo_id, yolo = wf.add_task("infer_yolo_v7")
wf.connect_tasks(wf.getRootID(), yolo_id)
```

Then run and display your results.

``` python
import cv2

# Run directly on your image
wf.run_on(path="path/to/your/image.png")

# YOLO output image with bounding boxes
img_bbox = wf.get_image_with_graphics(yolo_id)
img_bbox = cv2.cvtColor(img_bbox, cv2.COLOR_RGB2BGR)

cv2.imshow("Ikomia Demo", img_bbox)

```
<p float="left">
  <img src="https://github.com/Ikomia-dev/notebooks/blob/main/examples/img/img_foot.png?raw=true" width="400" /> 
  <img src="https://github.com/Ikomia-dev/notebooks/blob/main/examples/img/img_foot_bbox.png?raw=true" width="400" />
</p>

You can also change each algorithms parameters.

``` python
yolo_params = {
    "custom_train": True,
    "custom_model": "path/to/your/model",
    "thr_conf": 0.25
}
wf.set_parameters(task_id=yolo_id, params=yolo_params)
```

If you don't know what are the parameters (which is often the case), just print your task !
``` python
print(yolo)
```

<details><summary>Show print</summary>
<p>


```
###################################
#	infer_yolo_v7
###################################

***********************************
*	 PARAMETERS
***********************************

cuda:True
thr_conf:0.25
iou_conf:0.5
pretrain_model:yolov7
custom_model:
img_size:640
custom_train:False

***********************************
*	 INPUTS
***********************************

-----------------------------------
Name: porsche-4795517_960_720
-----------------------------------
Description: 2D or 3D images.
Can be single frame from video or camera stream.
Save folder: 
Auto-save: 0
Data type: image
Save format: .png
Dimension count: 2
File name: 
-----------------------------------
Name: CGraphicsInput
-----------------------------------
Description: Graphics items organized in layer.
Represent shapes and types of objects in image.
Graphics can be created interactively by user.
Save folder: 
Auto-save: 0
Data type: graphics
Save format: .json
Dimension count: 0

***********************************
*	 OUTPUTS
***********************************

-----------------------------------
Name: CImageIO
-----------------------------------
Description: 2D or 3D images.
Can be single frame from video or camera stream.
Save folder: 
Auto-save: 0
Data type: image
Save format: .png
Dimension count: 2
File name: 
-----------------------------------
Name: CObjectDetectionIO
-----------------------------------
Description: Object detection data: label, confidence, box and color.
Save folder: 
Auto-save: 0
Data type: Object detection
Save format: .json
Dimension count: 0
```

</p>
</details>


Then you can easily save your workflow as JSON file for reuse.
``` python
wf.save("path/to/your/workflow.json")
```
``` python
wf.load("path/to/your/workflow.json")
wf.run_on(path="path/to/your/image.png")
```

And finally, you can also export your results as JSON files.
``` python
# Get all object detection outputs (most of the time, there is only one)
output_list = wf.get_object_detection_output(task_name="infer_yolo_v7")
output_list[0].toJson()
```
```
{
  "detections": [
    {
      "box": {
        "height": 195,
        "width": 463,
        "x": 183,
        "y": 261
      },
      "color": {
        "a": 0,
        "b": 192,
        "g": 106,
        "r": 91
      },
      "confidence": 0.90673828125,
      "id": 0,
      "label": "ball"
    }
  ]
}
```

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

Python API documentation can be found [here](https://ikomia-dev.github.io/python-api-documentation/). You will find Ikomia HUB algorithms code source in [our Ikomia HUB GitHub](https://github.com/Ikomia-hub).

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

