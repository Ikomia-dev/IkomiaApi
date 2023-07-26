# Create and integrate your algorithm in workflows

```{toctree}
:hidden:

algo_parameters
algo_widget
algo_IO
```

Creating and integrating your own algorithms into workflows can be a powerful way to automate and streamline your data processing tasks. 

In this document, we will provide an overview of the steps involved in creating and integrating your own algorithm into a workflow using the Ikomia API.

## Algorithm creation with `ikomia-cli`

The Ikomia API provides a command-line interface tool called `ikomia-cli` that can be used to create a new algorithm. Here's how to get started:

1. First, make sure you have the `ikomia-cli` tool installed. You can do this by running the following command in your terminal:
```shell
pip install 'ikomia-cli[full]'
```
2. Once you have installed `ikomia-cli`, you can create a new algorithm by running the following command:
```bash
ikcli algo local create my_first_algo
```
This will create a new directory called **"my_first_algo"** in "Ikomia/Plugins/Python/" directory. Inside this directory, you will find several files that make up your algorithm.

3. The main file of your algorithm is **"my_first_algo_process.py"**. This file contains the Python code that implements your algorithm. You can open this file in a text editor or IDE to begin editing it.
Here's an example of what this file might look like:
```python
import copy

from ikomia import core, dataprocess


# --------------------
# - Class to handle the process parameters
# - Inherits core.CWorkflowTaskParam from Ikomia API
# --------------------
class MyFirstAlgoParam(core.CWorkflowTaskParam):

    def __init__(self):
        core.CWorkflowTaskParam.__init__(self)
        # Place default value initialization here
        # Example : self.windowSize = 25

    def set_values(self, params):
        # Set parameters values from Ikomia application
        # Parameters values are stored as string and accessible like a python dict
        # Example : self.windowSize = int(params["windowSize"])
        pass

    def get_values(self):
        # Send parameters values to Ikomia application
        # Create the specific dict structure (string container)
        params = {}
        # Example : params["windowSize"] = str(self.windowSize)
        return params


# --------------------
# - Class which implements the process
# - Inherits core.CWorkflowTask or derived from Ikomia API
# --------------------
class MyFirstAlgo(core.CWorkflowTask):

    def __init__(self, name, param):
        core.CWorkflowTask.__init__(self, name)
        # Add input/output of the process here
        # Example :  self.add_input(dataprocess.CImageIO())
        #           self.add_output(dataprocess.CImageIO())

        # Create parameters class
        if param is None:
            self.set_param_object(MyFirstAlgoParam())
        else:
            self.set_param_object(copy.deepcopy(param))

    def get_progress_steps(self):
        # Function returning the number of progress steps for this process
        # This is handled by the main progress bar of Ikomia application
        return 1

    def run(self):
        # Core function of your process
        # Call begin_task_run() for initialization
        self.begin_task_run()

        # Examples :
        # Get input :
        # task_input = self.get_input(index_of_input)

        # Get output :
        # task_output = self.get_output(index_of_output)

        # Get parameters :
        # param = self.get_param_object()

        # Get image from input/output (numpy array):
        # src_image = task_input.get_image()

        # Call to the process main routine
        # dst_image = ...

        # Set image of input/output (numpy array):
        # task_output.set_image(dst_image)

        # Step progress bar:
        self.emit_step_progress()

        # Call end_task_run() to finalize process
        self.end_task_run()


# --------------------
# - Factory class to build process object
# - Inherits dataprocess.CTaskFactory from Ikomia API
# --------------------
class MyFirstAlgoFactory(dataprocess.CTaskFactory):

    def __init__(self):
        dataprocess.CTaskFactory.__init__(self)
        # Set process information as string here
        self.info.name = "my_first_algo"
        self.info.short_description = "your short description"
        self.info.description = "your description"
        # relative path -> as displayed in Ikomia Studio process tree
        self.info.path = "Plugins/Python"
        self.info.version = "1.0.0"
        # self.info.icon_path = "your path to a specific icon"
        self.info.authors = "algorithm author"
        self.info.article = "title of associated research article"
        self.info.journal = "publication journal"
        self.info.year = 2022
        self.info.license = "MIT License"
        # URL of documentation
        self.info.documentation_link = ""
        # Code source repository
        self.info.repository = ""
        # Keywords used for search
        self.info.keywords = "your,keywords,here"

    def create(self, param=None):
        # Create process object
        return MyFirstAlgo(self.info.name, param)
```
As you can see, there are 3 classes: 
* One for the parameters
* One for the core algorithm
* One for the meta-data description.

4. By default, this algorithm does nothing. Let's create an algorithm that takes 1 input image and outputs a Canny edge detection of it.

To do this, you need to edit the `__init__()` function in `MyFirstAlgo` class:
```python
class MyFirstAlgo(core.CWorkflowTask):
    
    def __init__(self, name, param):
        core.CWorkflowTask.__init__(self, name)
        # Add input/output of the process here
        self.add_input(dataprocess.CImageIO()) # <-- Add input object of type image
        self.add_output(dataprocess.CImageIO()) # <-- Add output object of type image
    
        # Create parameters class
        if param is None:
            self.set_param_object(MyFirstAlgoParam())
        else:
            self.set_param_object(copy.deepcopy(param))
```
You now have an algorithm which takes 1 input image and outputs 1 image.


Then, you can edit the `run()` function like this (don't forget to import `cv2`) :
```python
import cv2
...
def run(self):
    # Core function of your process
    # Call begin_task_run() for initialization
    self.begin_task_run()

    # Examples :
    # Get input :
    task_input = self.get_input(0) # <-- Get first input object

    # Get output :
    task_output = self.get_output(0) # <-- Get first output object

    # Get input image (numpy array):
    src_image = task_input.get_image() # <-- Get image from your input object

    # Call to the process main routine
    dst_image = cv2.Canny(src_image, threshold1=100, threshold2=200) # <-- Use OpenCV Canny edge detector

    # Set output image (numpy array):
    task_output.set_image(dst_image) # <-- Set processed image to your output object

    # Step progress bar:
    self.emit_step_progress()

    # Call end_task_run() to finalize process
    self.end_task_run()
```

6. Your algorithm is now ready to be integrated into an Ikomia workflow.

That's it! With just a few simple steps, you can create your own algorithm using the Ikomia API and `ikomia-cli`.

## Algorithm integration

Once you have created and edited your algorithm, you can integrate it into a larger workflow using the Ikomia API. 

Here are the basic steps for integrating your algorithm:

1. Import the necessary modules from the Ikomia API:
```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik
from ikomia.utils.displayIO import display
```
2. Create a new workflow object:
```python
wf = Workflow()
```
3. Add your algorithm to the workflow:
```python
my_algorithm = wf.add_task(ik.my_first_algo(), auto_connect=True)
```
4. Connect the inputs and outputs of your algorithm to other tasks in the workflow as needed. This can be done using the `auto_connect=True` argument when adding your algorithm to the workflow, or by manually connecting inputs and outputs using the `connect_tasks()` method.
5. Run the workflow on any input image:
```python
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")
```
6. Retrieve the output of your algorithm and display it:
```python
output_data = my_algorithm.get_output(0).get_image()
display(output_data)
```
The full code:
```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils.displayIO import display
from ikomia.utils import ik

# Init the workflow
wf = Workflow()

# Add your algorithm
my_algo = wf.add_task(ik.my_first_algo(), auto_connect=True)

# Run on your image
wf.run_on(url="https://raw.githubusercontent.com/Ikomia-dev/notebooks/main/examples/img/img_fireman.jpg")

# Inspect your results
output_data = my_algo.get_output(0).get_image()
display(output_data)
```
By following these steps, you can easily integrate your algorithm into a larger workflow and process your data with other algorithms. You can also use the Ikomia STUDIO to create and visualize your workflow, and save your workflows for later use.