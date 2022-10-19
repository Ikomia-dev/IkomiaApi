Process
=======

Code example for process implementation

.. code-block:: python

    from ikomia import core, dataprocess
    import copy
    # Your imports below

    # --------------------
    # - Class to handle the process parameters
    # - Inherits CProtocolTaskParam from Ikomia API
    # --------------------
    class HelloWorldParam(core.CWorkflowTaskParam):

        def __init__(self):
            core.CWorkflowTaskParam.__init__(self)
            # Place default value initialization here
            # Example : self.windowSize = 25

        def setParamMap(self, paramMap):
            # Set parameters values from Ikomia application
            # Parameters values are stored as string and accessible like a python dict
            # Example : self.windowSize = int(paramMap["windowSize"])
            pass

        def getParamMap(self):
            # Send parameters values to Ikomia application
            # Create the specific dict structure (string container)
            paramMap = core.ParamMap()
            # Example : paramMap["windowSize"] = str(self.windowSize)
            return paramMap


    # --------------------
    # - Class which implements the process
    # - Inherits CProtocolTask or derived from Ikomia API
    # --------------------
    class HelloWorld(dataprocess.C2dImageTask):

        def __init__(self, name, param):
            dataprocess.C2dImageTask.__init__(self, name)
            # Add input/output of the process here
            # Example :  self.addInput(core.CImageProcessIO())
            #           self.addOutput(core.CImageProcessIO())

            #Create parameters class
            if param is None:
                self.setParam(HelloWorldParam())
            else:
                self.setParam(copy.deepcopy(param))

        def getProgressSteps(self):
            # Function returning the number of progress steps for this process
            # This is handled by the main progress bar of Ikomia application
            return 1

        def run(self):
            # Core function of your process
            # Call beginTaskRun for initialization
            self.beginTaskRun()

            # Examples :
            # Get input :
            # input = self.getInput(indexOfInput)

            # Get output :
            # output = self.getOutput(indexOfOutput)

            # Get parameters :
            # param = self.getParam()

            # Get image from input/output (numpy array):
            # srcImage = input.getImage()

            # Call to the process main routine
            # dstImage = ...

            # Set image of input/output (numpy array):
            # output.setImage(dstImage)

            # Step progress bar:
            self.emitStepProgress()

            # Call endTaskRun to finalize process
            self.endTaskRun()


    # --------------------
    # - Factory class to build process object
    # - Inherits CTaskFactory from Ikomia API
    # --------------------
    class HelloWorldFactory(dataprocess.CTaskFactory):

        def __init__(self):
            dataprocess.CTaskFactory.__init__(self)
            # Set process information as string here
            self.info.name = "hello_world"
            self.info.description = "your description"
            self.info.authors = "Plugin authors"
            # relative path -> as displayed in Ikomia application process tree
            self.info.path = "Plugins/Python"
            # self.info.iconPath = "your path to a specific icon"
            # self.info.keywords = "your keywords" -> for search

        def create(self, param=None):
            # Create process object
            return HelloWorld(self.info.name, param)
