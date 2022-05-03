Widget
======

Code example for OCVBasics widget implementation

.. code-block:: python

    from ikomia import core, dataprocess
    from ikomia.utils import qtconversion
    from ocv_basics.ocv_basics_process import OCVBasicsParam
    #PyQt GUI framework
    from PyQt5.QtWidgets import *


    # --------------------
    # - Class which implements widget associated with the process
    # - Inherits core.CProtocolTaskWidget from Ikomia API
    # --------------------
    class OCVBasicsWidget(core.CWorkflowTaskWidget):

        def __init__(self, param, parent):
            core.CWorkflowTaskWidget.__init__(self, parent)

            if param is None:
                self.parameters = OCVBasicsParam()
            else:
                self.parameters = param

            # Create layout : QGridLayout by default
            self.gridLayout = QGridLayout()
            
            # Kernel size
            label_kernel_size = QLabel('Kernel size:')
            
            self.spin_kernel_x = QSpinBox()
            self.spin_kernel_x.setRange(1, 99)
            self.spin_kernel_x.setSingleStep(2)
            self.spin_kernel_x.setValue(self.parameters.kernel_size[0])

            self.spin_kernel_y = QSpinBox()
            self.spin_kernel_y.setRange(1, 99)
            self.spin_kernel_y.setSingleStep(2)
            self.spin_kernel_y.setValue(self.parameters.kernel_size[1])

            self.gridLayout.addWidget(label_kernel_size, 0, 0)
            self.gridLayout.addWidget(self.spin_kernel_x, 0, 1)
            self.gridLayout.addWidget(self.spin_kernel_y, 0, 2)

            # Sigma X
            label_sigma_x = QLabel('Sigma X:')
            self.spin_sigma_x = QDoubleSpinBox()
            self.spin_sigma_x.setRange(0.0, 255.0)
            self.spin_sigma_x.setSingleStep(0.1)
            self.spin_sigma_x.setValue(self.parameters.sigma_x)

            self.gridLayout.addWidget(label_sigma_x, 1, 0)
            self.gridLayout.addWidget(self.spin_sigma_x, 1, 1)

            # Sigma Y
            label_sigma_y = QLabel('Sigma Y:')
            self.spin_sigma_y = QDoubleSpinBox()
            self.spin_sigma_y.setRange(0.0, 255.0)
            self.spin_sigma_y.setSingleStep(0.1)
            self.spin_sigma_y.setValue(self.parameters.sigma_y)

            self.gridLayout.addWidget(label_sigma_y, 2, 0)
            self.gridLayout.addWidget(self.spin_sigma_y, 2, 1)

            # PyQt -> Qt wrapping (C++ handle)
            layoutPtr = qtconversion.PyQtToQt(self.gridLayout)

            # Set widget layout
            self.setLayout(layoutPtr)

        def onApply(self):
            # Apply button has been pressed
            # Get parameters value from widget components
            self.parameters.kernel_size = (self.spin_kernel_x.value(), self.spin_kernel_y.value())
            self.parameters.sigma_x = self.spin_sigma_x.value()
            self.parameters.sigma_y = self.spin_sigma_y.value()

            # Send signal to launch the process
            self.emitApply(self.parameters)


    #--------------------
    #- Factory class to build process widget object
    #- Inherits dataprocess.CWidgetFactory from Ikomia API
    #--------------------
    class OCVBasicsWidgetFactory(dataprocess.CWidgetFactory):

        def __init__(self):
            dataprocess.CWidgetFactory.__init__(self)
            # Set the name of the process -> it must be the same as the one declared in the process factory class
            self.name = "ocv_basics"

        def create(self, param):
            # Create widget object
            return OCVBasicsWidget(param, None)
