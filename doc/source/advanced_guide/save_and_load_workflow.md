# Save and load your workflow

## Save your workflow

```python
wf.save("path/to/my_workflow.json")
```
If you want to save your workflow, you can export it in JSON format. This export will save all algorithms, parameters, model paths and connections in a descriptive file.

This file enables you to share your work or easily reuse it on any computing device.

## Load and run your workflow

Basic script for loading workflows.

```python
from ikomia.dataprocess.workflow import Workflow

# Init the workflow
wf = Workflow()

# Load the workflow
wf.load("path/to/my_workflow.json")

# Run on your image
wf.run_on(path="path/to/your/image.png")
```

## Modify the workflow

First you can get all tasks from the workflow with :
```python
print(wf.get_tasks())
```
For example, you can obtain this kind of output :
```shell
[CWorkflowTask(Root), CObjectDetectionTask(infer_yolo_v7), C2dImageTask(ocv_stack_blur, 1)]
```
Then, you can easily access each task by searching by name:
```python
task = wf.find_task("task_name")
```
Or
```python
task = wf.find_task(ik.task_name())
```
See [`ik` auto-completion](ik_namespace) for more information.

Finally, adjust the parameters :

```python
task.set_parameters({
    ik.task_name.param_1: "value",
    ik.task_name.param_2: "value"
})
```
And you're done ! 