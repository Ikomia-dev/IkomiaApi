# How to add parameters to your algorithm

Most of the time, you need to add parameters to control your algorithm behaviour.

Let's take the same example as in the [previous part](index.md).

## Add parameters

In the `MyFirstAlgoParam` class, you have to do the following :
```python
# --------------------
# - Class to handle the process parameters
# - Inherits core.CWorkflowTaskParam from Ikomia API
# --------------------
class MyFirstAlgoParam(core.CWorkflowTaskParam):

    def __init__(self):
        core.CWorkflowTaskParam.__init__(self)
        # Place default value initialization here
        self.threshold1 = 100
        self.threshold2 = 200

    def set_values(self, params):
        # Set parameters values from Ikomia application
        # Parameters values are stored as string and accessible like a python dict
        self.threshold1 = int(params["threshold1"])
        self.threshold2 = int(params["threshold2"])

    def get_values(self):
        # Send parameters values to Ikomia application
        # Create the specific dict structure (string container)
        params = {
            "threshold1": str(self.threshold1),
            "threshold2": = str(self.threshold2),
        }
        return params
```

## Use parameters in your code

Then, you can use your parameters in the core algorithm :

```python
import cv2
...
def run(self):
    # Core function of your process
    # Call begin_task_run() for initialization
    self.begin_task_run()

    # Examples :
    # Get input :
    task_input = self.get_input(0) 

    # Get output :
    task_output = self.get_output(0) 

    # Get parameters :
    param = self.get_param_object() # <-- Get the parameters

    # Get image from input/output (numpy array):
    src_image = task_input.get_image() 

    # Call to the process main routine
    dst_image = cv2.Canny(src_image, threshold1=param.threshold1, threshold2=param.threshold2)

    # Set image of input/output (numpy array):
    task_output.set_image(dst_image)

    # Step progress bar:
    self.emit_step_progress()

    # Call end_task_run() to finalize process
    self.end_task_run()
```

## Use parameters in your workflows

Now you can change parameters in your workflows :

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils.displayIO import display
from ikomia.utils import ik

# Init the workflow
wf = Workflow()

# Add your algorithm
my_algo = wf.add_task(ik.my_first_algo(threshold1="100", threshold2="200"), auto_connect=True)

# Run on your image
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")

# Inspect your results
output_data = my_algo.get_output(0).get_image()
display(output_data)
```