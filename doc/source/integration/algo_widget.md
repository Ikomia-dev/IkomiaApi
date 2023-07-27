# How to make a nice widget for your algorithm

In Ikomia API, you don't need any widget but in Ikomia STUDIO, it can be very useful for easily edit your parameters.

Let's take the same example as in the [previous part](index.md).

## Base implementation

By default, the file `my_first_algo_widget.py` looks like this:

```python

from ikomia import core, dataprocess
from ikomia.utils import pyqtutils, qtconversion
from my_first_algo.my_first_algo_process import (
    MyFirstAlgoParam,
)
from PyQt5.QtWidgets import *


# --------------------
# - Class which implements widget associated with the process
# - Inherits PyCore.CWorkflowTaskWidget from Ikomia API
# --------------------
class MyFirstAlgoWidget(core.CWorkflowTaskWidget):

    def __init__(self, param, parent):
        core.CWorkflowTaskWidget.__init__(self, parent)

        if param is None:
            self.parameters = MyFirstAlgoParam()
        else:
            self.parameters = param

        # Create layout : QGridLayout by default
        self.grid_layout = QGridLayout()
        # PyQt -> Qt wrapping
        layout_ptr = qtconversion.PyQtToQt(self.grid_layout)

        # Set widget layout
        self.set_layout(layout_ptr)

    def on_apply(self):
        # Apply button clicked slot

        # Get parameters from widget
        # Example : self.parameters.windowSize = self.spinWindowSize.value()

        # Send signal to launch the process
        self.emit_apply(self.parameters)


# --------------------
# - Factory class to build process widget object
# - Inherits PyDataProcess.CWidgetFactory from Ikomia API
# --------------------
class MyFirstAlgoWidgetFactory(dataprocess.CWidgetFactory):

    def __init__(self):
        dataprocess.CWidgetFactory.__init__(self)
        # Set the name of the process -> it must be the same as the one declared in the process factory class
        self.name = "my_first_algo"

    def create(self, param):
        # Create widget object
        return MyFirstAlgoWidget(param, None)

```
There is no widgets but there is already a layout and an apply button.

In the previous example, we have 2 parameters named `threshold1` and `threshold2`, and we want to edit these parameters with spin boxes.

## Adding widgets to the algorithm

```python
class MyFirstAlgoWidget(core.CWorkflowTaskWidget):

    def __init__(self, param, parent):
        core.CWorkflowTaskWidget.__init__(self, parent)

        if param is None:
            self.parameters = MyFirstAlgoParam()
        else:
            self.parameters = param

        # Create layout : QGridLayout by default
        self.grid_layout = QGridLayout()

        self.spin_threshold1 = pyqtutils.add_spin(self.grid_layout, 0, "threshold1", 100, min=0, max=255)
        self.spin_threshold2 = pyqtutils.add_spin(self.grid_layout, 1, "threshold2", 200, min=0, max=255)

        # PyQt -> Qt wrapping
        layout_ptr = qtconversion.PyQtToQt(self.grid_layout)

        # Set widget layout
        self.set_layout(layout_ptr)
```
In this code, we use a small utility package called {py:mod}`~ikomia.utils.pyqtutils` which implements basic widgets from PyQt.
We add a first spin box called `spin_threshold1` to the layout `grid_layout`. We put this widget in the first row `0` with a label, a default value and a range.
Then we add a second spin box called `spin_threshold2` in the second row `1`.

## Connecting widget to process

Now, we have to connect value changed in the widget with the main process. To do that, we just edit the `on_apply()` function:
```python
    def on_apply(self):
        # Apply button clicked slot

        # Get parameters from widget
        # Example : self.parameters.windowSize = self.spinWindowSize.value()
        self.parameters.threshold1 = self.spin_threshold1.value()
        self.parameters.threshold2 = self.spin_threshold2.value()

        # Send signal to launch the process
        self.emit_apply(self.parameters)
```
As you can see, we just set values thanks to the widget values. Now you can try your algorithm with its widget in Ikomia STUDIO. Enjoy !