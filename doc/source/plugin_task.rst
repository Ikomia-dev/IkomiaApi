Task management
===============

In Ikomia platform, a task is the implementation of a specific algorithm. We use inheritance to set a 
functional framework and simplify customization. Basically, any new Ikomia plugin must implement a 
task class derived from the base class :py:mod:`~ikomia.core.pycore.CWorkflowTask` or from a 
specialization class which itself inherits from this base class.

- :ref:`Mandatory methods<Mandatory methods to implement>`
- :ref:`Recommended methods<Recommended methods to implement>`
- :ref:`Metadata<Metadata>`


Mandatory methods to implement
------------------------------

- **Constructor**: base class contructor must be called explicitly. This is also the place where you define inputs and outputs.

.. code-block:: python

    class MyAlgorithm(dataprocess.CWorkflowTask):

        def __init__(self, name, param):
            dataprocess.CWorkflowTask.__init__(self, name)
            # Add image input
            self.addOutput(dataprocess.CImageIO())
            # Add graphics output
            self.addOutput(dataprocess.CGraphicsOutput())
            # Add numeric output
            self.addOutput(dataprocess.CNumericIO())

- **run()**: launch the algorithm execution. When the method is called, inputs data are set and ready to process. The method must begin with a call to **beginTaskRun()** and end with a call to **endTaskRun()**. In the code snippet below, you will find the most useful methods that you will need while implementing your own plugin:

.. code-block:: python

    class MyAlgorithm(dataprocess.CWorkflowTask):

        def run(self):
            
            # Execution initialization
            self.beginTaskRun()

            # Get inputs
            input1 = self.getInput(0)
            input2 = self.getInput(1)

            # Get parameters
            param = self.getParam()

            # Get outputs
            output1 = self.getOutput(0)
            output1 = self.getOutput(1)

            # Execution finalization
            self.endTaskRun()

.. note:: See :py:mod:`~ikomia.core.pycore.CWorkflowTask` to have details around all available methods.


Recommended methods to implement
--------------------------------

- **getProgressSteps()**: Ikomia Studio only. For time consuming algorithms, it could be relevant to split execution into separate parts and give progression feedback after each part (if possible). If so, you have to first implement this method and return the number of execution parts. Then, in your **run()** method, you have to place a call to **self.emitStepProgress()** after each part to notify the main progress bar of Ikomia Studio.
- **stop()**: method called when a user requests the process to stop. For time consuming algorithm it's highly recommended to integrate a stop mechanism inside your algorithm execution code. This method is somehow just a callback.


Available specializations
-------------------------

To ease implementation of common image processing tasks, we provide some specialization classes from 
which you can inherit. They integrate useful features to speed your developpment.


Simple image processing algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:py:class:`~ikomia.dataprocess.pydataprocess.C2dImageTask` : base class dedicated to algorithms processing image and producing at least a result image.

    - Inputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`) and graphics (:py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`).
    - Outputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`).
    - Important features:
        - create binary mask from graphics. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageTask.createGraphicsMask`.
        - apply a binary mask to the desired output image so that only masked areas are processed. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageTask.applyGraphicsMask` and :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageTask.applyGraphicsMaskToBinary`.
        - forward input image to the desired output without modification: :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageTask.forwardInputImage`.
        - attach a color overlay mask to the desired output (Ikomia Studio only). See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageTask.setOutputColorMap`.
        
.. note:: If any of these features are useful for your own algorithm, inherit your class from this base class instead of CWorkflowTask. Please consult :doc:`this tutorial <opencv_plugin>` for details.


Interactive image processing algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:py:class:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask`: choose it as a base class 
if you need user interaction for your algorithm. The class embeds a graphics interaction layer. 
Every changes made by users on this layer are notified and corresponding actions can be implemented. 
**Ikomia Studio only**.

    - Inputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`) and graphics (:py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`).
    - Outputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`).
    - Important features:
        - callback to manage interaction layer modification. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask.graphcisChanged`.
        - create binary mask from the graphics interaction layer. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask.createInteractionMask`.
        - compute connected components from the binary mask generated from the interaction layer. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask.computeBlobs`.
        - get connected components as a list of polygons (list of points). See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask.getBlobs`.
        - clear interaction layer. See :py:meth:`~ikomia.dataprocess.pydataprocess.C2dImageInteractiveTask.clearInteractionLayer`.

.. note:: If any of these features are useful for your own algorithm, inherit your class from this base class instead of CWorkflowTask.


Video processing algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^
:py:class:`~ikomia.dataprocess.pydataprocess.CVideoTask`: choose it as a base class 
if your algorithm is dedicated to video or stream. The class inherits from 
:py:class:`~ikomia.dataprocess.pydataprocess.C2dImageTask` adding only a mechanism to handle  
start and stop events.

    - Inputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`) and graphics (:py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`).
    - Outputs: image (:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`).
    - Important features:
        - callback to manage video start event. See :py:meth:`~ikomia.dataprocess.pydataprocess.CVideoTask.notifyVideoStart`.
        - callback to manage video stop event. See :py:meth:`~ikomia.dataprocess.pydataprocess.CVideoTask.notifyVideoStop`.

.. note:: Please consult source code of `infer_raft_optical_flow <https://github.com/Ikomia-hub/infer_raft_optical_flow>`_ for implementation example.


Deep Learning training algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

:py:class:`~ikomia.dnn.dnntrain.TrainProcess`: choose it as a base class for any deep learning 
training algorithms. This base class integrated training monitoring features for **MLflow** and 
**Tensorboard**.

    - Inputs: Ikomia dataset (:py:mod:`~ikomia.dnn.datasetio.IkDatasetIO`).
    - Outputs: None
    - Important features:
        - MLflow: a local tracking server is automatically started and ready to use at http://localhost:5000. By default, logging data are saved in the Ikomia folder in *mlflow* subdirectory. The class offers a thin wrapper to access logging methods: model hyper-parameters with :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_param` and :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_params`, model metrics with :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_metric` and :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_metrics` and model artifacts with :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_artifact` and :py:meth:`~ikomia.dnn.dnntrain.TrainProcess.log_artifacts`.
        - Tensorboard: a local tracking server is automatically started and ready to use at http://localhost:6006. By default, logging data are saved in the Ikomia folder in *tensorboard* subdirectory. Use classic Tensorboard functions to log your data.

.. note:: Please consult source code of `train_yolov5 <https://github.com/Ikomia-hub/train_yolo_v5>`_ for implementation example.


Metadata
--------

Ikomia platform allows you to add additionnal information to your plugin. This will help users to 
know more about your algorithm and give essential visual elements in Ikomia HUB.

These information are store in a factory class inherited from :py:class:`~ikomia.dataprocess.pydataprocess.CTaskFactory`. 
It aims to create instance of your process class. Plugin metadata are set in the constructor within the **CTaskInfo** member variable. 
You will find he full list of information in the :py:class:`~ikomia.dataprocess.pydataprocess.CTaskInfo` documentation.

For Ikomia Studio, you can customize the documentation page of your plugin. It can be interesting if you want to put more 
information than the provided fields. You just need to place your documentation page in the root folder of the plugin. 
File format can be either Markdown or HTML. Ikomia Studio will automatically search for files with the following names:
*doc.md, doc.html, doc.htm, documentation.md, documentation.html, documentation.htm, info.md, info.html, info.htm*.
