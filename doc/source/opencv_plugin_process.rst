Process
=======

Code example for OCVBasics process implementation

.. code-block:: python

    from ikomia import core, dataprocess
    import copy
    # Your imports below
    import cv2

    # --------------------
    # - Class to handle the process parameters
    # - Inherits core.CProtocolTaskParam from Ikomia API
    # --------------------
    class OCVBasicsParam(core.CWorkflowTaskParam):

        def __init__(self):
            core.CWorkflowTaskParam.__init__(self)
            # Place default value initialization here
            self.kernel_size = (3, 3)
            self.sigma_x = 1.0
            self.sigma_y = 1.0

        def set_values(self, params):
            # Set parameters values from Ikomia application (user inputs)
            # Parameters values are stored as string and accessible like a python dict
            self.kernel_size = (int(params["kernel_size_x"]), int(params["kernel_size_y"]))
            self.sigma_x = int(params["sigma_x"])
            self.sigma_y = int(params["sigma_y"])

        def get_values(self):
            # Send parameters values to Ikomia application (workflow)
            # Create the specific dict structure (string container)
            params = {}
            params["kernel_size_x"] = str(self.kernel_size[0])
            params["kernel_size_y"] = str(self.kernel_size[1])
            params["sigma_x"] = str(self.sigma_x)
            params["sigma_y"] = str(self.sigma_y)
            return params


    # --------------------
    # - Class which implements the process
    # - Inherits core.CProtocolTask or derived from Ikomia API
    # --------------------
    class OCVBasics(dataprocess.C2dImageTask):

        def __init__(self, name, param):
            dataprocess.C2dImageTask.__init__(self, name)
            # Add input/output of the process here
            # Example :  self.add_input(core.CImageProcessIO())
            #           self.add_output(core.CImageProcessIO())

            #Create parameters class
            if param is None:
                self.set_param_object(OCVBasicsParam())
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

            # Get parameters :
            param = self.get_param_object()

            # Get input :
            input_img = self.get_input(0)

            # Get image from input (numpy array):
            src_image = input_img.get_image()

            # Call to the process main routine
            # Grayscale conversion
            proc_img = cv2.cvtColor(src_image, cv2.COLOR_RGB2GRAY)
            # Gaussian blur
            proc_img = cv2.GaussianBlur(proc_img, param.kernel_size, param.sigma_x, param.sigma_y)
            # Canny filter
            proc_img = cv2.Canny(proc_img, 0, 255)

            # Get output :
            output = self.get_output(0)

            # Set image of output (numpy array):
            output.set_image(proc_img)

            # Step progress bar:
            self.emit_step_progress()

            # Call end_task_run to finalize process
            self.end_task_run()


    # --------------------
    # - Factory class to build process object
    # - Inherits dataprocess.CTaskFactory from Ikomia API
    # --------------------
    class OCVBasicsFactory(dataprocess.CTaskFactory):

        def __init__(self):
            dataprocess.CTaskFactory.__init__(self)
            # Set process information as string here
            self.info.name = "ocv_basics"
            self.info.shortDescription = "OpenCV Canny"
            self.info.description = "Simple OpenCV pipeline that computes Canny filter"
            self.info.authors = "Ikomia team"
            self.info.article = ""
            self.info.journal = ""
            self.info.year = 2020
            self.info.license = "MIT License"
            self.info.version = "1.0.0"
            self.info.repo = "https://github.com/Ikomia-dev"
            self.info.documentationLink = "https://ikomia.com"
            # relative path -> as displayed in Ikomia application process tree
            self.info.path = "Plugins/Python/Ikomia/Examples"
            # If you want to customize plugin icon
            self.info.iconPath = ""
            # Associated keywords, for search
            self.info.keywords = "OpenCV,blur,grayscale,canny,edge,gaussian"

        def create(self, param=None):
            # Create process object
            return OCVBasics(self.info.name, param)
