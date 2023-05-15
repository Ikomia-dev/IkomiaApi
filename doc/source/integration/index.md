# How to create and integrate your algorithm in workflows

```{toctree}
:hidden:

dependencies
naming
```

```{toctree}
:caption: Examples
:hidden:

hello_world_plugin
opencv_plugin
interactive_plugin
```

Creating and integrating your own algorithms into workflows can be a powerful way to automate and streamline your data processing tasks. 
While there are many pre-existing algorithms available, there may be times when you need to develop your own algorithm to meet specific requirements. 
Fortunately, there are tools available, such as the Ikomia API, that can help simplify the process of creating and integrating your own algorithms.

In this document, we will provide an overview of the steps involved in creating and integrating your own algorithm into a workflow using the Ikomia API. 
We will cover topics such as the structure of an algorithm file, tips for editing algorithms, and how to integrate your algorithm into a workflow. 
We will also provide examples of different types of algorithms that can be created and integrated into workflows.

## Algorithm creation with `ikomia-cli`

The Ikomia API provides a command-line interface tool called `ikomia-cli` that can be used to create a new algorithm. Here's how to get started:

1. First, make sure you have the `ikomia-cli` tool installed. You can do this by running the following command in your terminal:
```shell
pip install ikomia-cli
```
2. Once you have installed `ikomia-cli`, you can create a new algorithm by running the following command:
```shell
ikcli algo create my_first_algo
```
This will create a new directory called "my_first_algo" in "~/Ikomia/Plugins/Python/" directory. Inside this directory, you will find several files that make up your algorithm.

3. The main file of your algorithm is "my_first_algo_process.py". This file contains the Python code that implements your algorithm. You can open this file in a text editor or IDE to begin editing it.
4. The "my_first_algo_process.py" file is structured using the Ikomia API's `DataProcess` class. This class provides a standard structure for algorithms and makes it easy to define inputs, outputs, and other properties of your algorithm. Here's an example of what your "my_first_algo_process.py" file might look like:
```python
from ikomia import core, dataprocess
import copy

class MyFirstAlgo(core.CWorkflowTask):

    def __init__(self, name, param):
        core.CWorkflowTask.__init__(self, name)

        # Define your inputs and outputs here
        self.add_input(dataprocess.CImageIO())
        self.add_output(dataprocess.CImageIO())
        
        # Create parameters class
        if param is None:
            self.set_param_object(MyFirstAlgoParam())
        else:
            self.set_param_object(copy.deepcopy(param))

    def run(self):
        # Implement your algorithm here
        input_img = self.get_input(0)
        output_img = input_img.clone()
        self.get_output(0).set_image(output_img)
```
In this example, we are defining an algorithm called MyFirstAlgo. 
This algorithm has one input and one output, both of which are images (CImageIO). 
In the run method, we implement our algorithm by reading in the input image, making a copy of it, and then setting the output image to be the copy.

Note that you can customize your algorithm's inputs and outputs to fit your specific use case. For example, if your algorithm requires text inputs or outputs, you could use CTextIO instead.

5. Once you have defined your algorithm in the "my_first_algo.py" file, you can save the file and exit your text editor or IDE. 
Your algorithm is now ready to be integrated into a workflow.

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
5. Run the workflow on your input data:
```python
wf.run_on(path="/path/to/input")
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
my_algorithm = wf.add_task(ik.my_first_algo(), auto_connect=True)

# Run on your image
wf.run_on(path="/path/to/image.png")

# Inspect your results
output_data = my_algorithm.get_output(0).get_image()
display(output_data)
```
By following these steps, you can easily integrate your algorithm into a larger workflow and process your data with other algorithms. You can also use the Ikomia STUDIO to create and visualize your workflow, and save your workflows for later use.

[comment]: <> (### Graphics)

[comment]: <> (Graphics represent all vectorial items &#40;line, polygon, text...&#41; that )

[comment]: <> (bring additionnal information to images. They can be stored as input &#40;{py:mod}`~ikomia.dataprocess.pydataprocess.CGraphicsInput`&#41; )

[comment]: <> (or output {py:mod}`~ikomia.dataprocess.pydataprocess.CGraphicsOutput`&#41;. Different types of graphics )

[comment]: <> (are provided, each one being implemented in a dedicated class:)

[comment]: <> (- Point: {py:mod}`~ikomia.core.pycore.CGraphicsPoint`)

[comment]: <> (- Polyline: {py:mod}`~ikomia.core.pycore.CGraphicsPolyline`)

[comment]: <> (- Rectangle/Square: {py:mod}`~ikomia.core.pycore.CGraphicsRectangle`)

[comment]: <> (- Ellipse/Circle: {py:mod}`~ikomia.core.pycore.CGraphicsEllipse`)

[comment]: <> (- Polygon: {py:mod}`~ikomia.core.pycore.CGraphicsPolygon`)

[comment]: <> (- Polygon with hole&#40;s&#41;: {py:mod}`~ikomia.core.pycore.CGraphicsComplexPolygon`)

[comment]: <> (- Text: {py:mod}`~ikomia.core.pycore.CGraphicsText`)

[comment]: <> (You can access to every graphics with :)

[comment]: <> (```python)

[comment]: <> (obj_list = output_obj.get_items&#40;&#41;)

[comment]: <> (```)

[comment]: <> ({py:meth}`~ikomia.dataprocess.pydataprocess.CGraphicsOutput.get_items` returns a list of graphical objects so you can iterate on it. )

[comment]: <> (When you have a {py:mod}`~ikomia.dataprocess.pydataprocess.CImageIO` and a {py:mod}`~ikomia.dataprocess.pydataprocess.CGraphicsOutput` as outputs, you can easily burn graphics on your image with {py:func}`~ikomia.dataprocess.pydataprocess.CImageIO.get_image_with_graphics`)

[comment]: <> (```python)

[comment]: <> (from ikomia.utils.displayIO import display)

[comment]: <> (img = yolov7.get_image_with_graphics&#40;&#41;)

[comment]: <> (display&#40;img&#41;)

[comment]: <> (```)