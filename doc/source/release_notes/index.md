# Release Notes

Version history of the Ikomia API.

<details>
  <summary>0.10.0</summary>
  
___
This release marks the transition to the new Ikomia Scale platform. This platform includes a new version of Ikomia HUB with more insights on how to use every algorithms with this API. 

**New features**:
- Switch to new Ikomia HUB from Ikomia Scale platform
- Add possibility to manage public and private HUBs
- Token-based authentication for private HUB
- 

**Improvements**:
- Add polygons output for instance and semantic segmentation algorithms
- Add methods in CImageIO class to get visual image from other I/O (graphics, object detection, segmentation...)
- Improve speed of auto-completion system (ik module)

**Bug fixes**:
- Export segmentation masks in PNG format for JSON serialization
- Missing Python binding for CInstanceSegIO::is_data_available()
- Windows: fix download function with correct path separators
- Avoid creating MLFlow experiment in training algorithm constructor
- ...

___
</details>

<details>
  <summary>0.9.2</summary>
  
___
**Improvements**:
- Update documentation
- Manage error if algorithm code package is not found in Ikomia HUB
- Manage mandatory parameter in IkomiaRegistry::create_algorithm()
- Update tests and make them compatible with PyTest


**Bug fixes**:
- Fix upper case extension for image and video files
- Fix I/O retrieval for composite I/O like object detection I/O, instance segmentation I/O, ...
- Fix typo error in Workflow::set_parameters()
- ...
___
</details>

<details>
  <summary>0.9.1</summary>
  
___
This minor version only includes the Ikomia domain change: ikomia.com -> ikomia.ai
___
</details>

<details>
  <summary>0.9.0</summary>
  
___
This release introduces many break changes as we change the naming convention of the Python bindings from the C++ core. The complete API is now in Snake Case style for all functions, there is no more difference between pure Python and C++ API. These changes may occur during the consolidation period of the API marked by the 0.x.x versions.

**New features**:

    OpenCV 4.7.0
    Python 3.10 support
    MLFlow 1.30
    Remove authentication to install and load algorithms from Ikomia HUB
    Algorithms lazy loading to speed up API initialization
    New base classes for common computer vision tasks:
        object detection
        semantic segmentation
        instance segmentation
        keypoints detection
    Add workflow text I/O
    Add internal algorithms: OpenCV Blur and StackBlur
    Add automatic conversion from instance segmentation I/O to semantic I/O

**Improvements**:

    New auto-completion system (ik module)
    New documentation
    Algorithm parameters are set from dict structure (not C++ structure anymore)
    Filtering process for object detection, instance segmentation and semantic segmentation outputs
    Handle print() function for many API objects
    System to find best video writer back-end and codec for video export
    displayIO module (now part of ikomia.utils)
    Warn user when an algorithm is not connected to any other algorithm in a workflow

**Bug fixes**:

    Fix object detection graphics export
    Fix elapsed time computation for training workflows
    Fix JSON serialization of image I/O
    Fix wrong output filenames on batch processing
    Fix synchronous download method for workflow tasks
    ...
___
</details>

<details>
  <summary>0.8.1</summary>
  
___
**Improvements**:
- Timeout support while writing videos
- Add Python bindings for executeActions() for classes inheriting ikomia.core.CWorkflowTask
- Add download method in ikomia.core.CWorkflowTask
- Improve logging system
- Update and fix documentation
- Let training task starts if Tensorboard initialization failed

**Bug fixes**:
- Manage invalid ID passed to ikomia.dataprocess.CWorkflow.getTask()
- Fix legend image for semantic segmentation output
- Auto-completion process skip invalid plugins
- ...
___
</details>

<details>
  <summary>0.8.0</summary>
  
___
**New features**:
- New algorithms from the Ikomia HUB: YoloV7,  Open MMlab object detection, SparseInst (training and inference)
- Add filtering tasks for object detection, instance segmentation and semantic segmentation
- Add display for text data (Python data dict output for example)
- Add feature to blacklist Python packages that confict with Ikomia built-in packages
- Manage new workflow I/O: object detection, instance segmentation, semantic segmentation
- Add automatic I/O conversion between different types (ex: object detection output -> graphics input)


**Improvements**:
- Manage compilation architecture for C++ algorithms from Ikomia HUB


**Bug fixes**:
- Disable Tensorboard auto-start to avoid algorithm installation failure
- ...
___
</details>

* 0.7.0
* 0.6.1
* 0.6.0
* 0.5.0
* 0.4.1
* 0.4.0
* 0.3.0
