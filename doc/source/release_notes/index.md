# Release Notes

Version history of the Ikomia API.

<details>
  <summary>0.9.0</summary>
  
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
___
</details>

* 0.7.0
* 0.6.1
* 0.6.0
* 0.5.0
* 0.4.1
* 0.4.0
* 0.3.0
