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

        def set_values(self, paramMap):
            # Set parameters values from Ikomia application
            # Parameters values are stored as string and accessible like a python dict
            # Example : self.windowSize = int(paramMap["windowSize"])
            pass

        def get_values(self):
            # Send parameters values to Ikomia application
            # Create the specific dict structure (string container)
            paramMap = {}
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
            # Example :  self.add_input(core.CImageProcessIO())
            #           self.add_output(core.CImageProcessIO())

            #Create parameters class
            if param is None:
                self.set_param_object(HelloWorldParam())
            else:
                self.set_param_object(copy.deepcopy(param))

        def get_progress_steps(self):
            # Function returning the number of progress steps for this process
            # This is handled by the main progress bar of Ikomia application
            return 1

        def run(self):
            # Core function of your process
            # Call begin_task_run for initialization
            self.begin_task_run()

            # Examples :
            # Get input :
            # input = self.get_input(indexOfInput)

            # Get output :
            # output = self.get_output(indexOfOutput)

            # Get parameters :
            # param = self.get_parameters()

            # Get image from input/output (numpy array):
            # srcImage = input.getImage()

            # Call to the process main routine
            # dstImage = ...

            # Set image of input/output (numpy array):
            # output.setImage(dstImage)

            # Step progress bar:
            self.emit_step_progress()

            # Call end_task_run to finalize process
            self.end_task_run()


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
