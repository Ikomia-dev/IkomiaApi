Widget
======

Code example for widget implementation

.. code-block:: python

    from ikomia import core, dataprocess
    from ikomia.utils import qtconversion
    from hello_world.hello_world_process import HelloWorldParam
    #PyQt GUI framework
    from PyQt5.QtWidgets import *


    # --------------------
    # - Class which implements widget associated with the process
    # - Inherits CProtocolTaskWidget from Ikomia API
    # --------------------
    class HelloWorldWidget(core.CWorkflowTaskWidget):

        def __init__(self, param, parent):
            core.CWorkflowTaskWidget.__init__(self, parent)

            if param is None:
                self.parameters = HelloWorldParam()
            else:
                self.parameters = param

            # Create layout : QGridLayout by default
            self.gridLayout = QGridLayout()
            # PyQt -> Qt wrapping
            layoutPtr = qtconversion.PyQtToQt(self.gridLayout)

            # Set widget layout
            self.setLayout(layoutPtr)

        def onApply(self):
            # Apply button clicked slot

            # Get parameters from widget
            # Example : self.parameters.windowSize = self.spinWindowSize.value()

            # Send signal to launch the process
            self.emitApply(self.parameters)


    #--------------------
    #- Factory class to build process widget object
    #- Inherits CWidgetFactory from Ikomia API
    #--------------------
    class HelloWorldWidgetFactory(dataprocess.CWidgetFactory):

        def __init__(self):
            dataprocess.CWidgetFactory.__init__(self)
            # Set the name of the process -> it must be the same as the one declared in the process factory class
            self.name = "hello_world"

        def create(self, param):
            # Create widget object
            return HelloWorldWidget(param, None)
