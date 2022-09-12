C2dImageTask
============

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: C2dImageTask

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import C2dImageTask


   .. rubric:: Methods
   .. autosummary::
      ~C2dImageTask.__init__
      ~C2dImageTask.applyGraphicsMask
      ~C2dImageTask.applyGraphicsMaskToBinary
      ~C2dImageTask.createInputGraphicsMask
      ~C2dImageTask.createGraphicsMask
      ~C2dImageTask.emitAddSubProgressSteps
      ~C2dImageTask.emitGraphicsContextChanged
      ~C2dImageTask.emitOutputChanged
      ~C2dImageTask.emitStepProgress
      ~C2dImageTask.forwardInputImage
      ~C2dImageTask.getGraphicsMask
      ~C2dImageTask.isMaskAvailable
      ~C2dImageTask.setOutputColorMap

   .. rubric:: Overridden methods
   .. autosummary::
      ~C2dImageTask.beginTaskRun
      ~C2dImageTask.endTaskRun
      ~C2dImageTask.graphicsChanged
      ~C2dImageTask.setActive
      ~C2dImageTask.updateStaticOutputs

   .. rubric:: Inherited methods
   .. autosummary::
      ~C2dImageTask.addInput
      ~C2dImageTask.addOutput
      ~C2dImageTask.executeActions
      ~C2dImageTask.getElapsedTime
      ~C2dImageTask.getInput
      ~C2dImageTask.getInputCount
      ~C2dImageTask.getInputDataType
      ~C2dImageTask.getInputs
      ~C2dImageTask.getOutput
      ~C2dImageTask.getOutputCount
      ~C2dImageTask.getOutputDataType
      ~C2dImageTask.getOutputs
      ~C2dImageTask.getParam
      ~C2dImageTask.getProgressSteps
      ~C2dImageTask.globalInputChanged      
      ~C2dImageTask.isGraphicsChangedListening      
      ~C2dImageTask.parametersModified
      ~C2dImageTask.removeInput
      ~C2dImageTask.run
      ~C2dImageTask.setActionFlag      
      ~C2dImageTask.setInput
      ~C2dImageTask.setInputDataType
      ~C2dImageTask.setInputs
      ~C2dImageTask.setOutput      
      ~C2dImageTask.setOutputDataType
      ~C2dImageTask.setOutputs
      ~C2dImageTask.setParam
      ~C2dImageTask.stop      
   
   .. rubric:: Attributes
   .. autosummary::
      ~C2dImageTask.name
      ~C2dImageTask.type
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: addInput
   .. automethod:: addOutput
   .. automethod:: applyGraphicsMask
   .. automethod:: applyGraphicsMaskToBinary
   .. automethod:: beginTaskRun
   .. automethod:: createInputGraphicsMask
   .. automethod:: createGraphicsMask
   .. automethod:: emitAddSubProgressSteps
   .. automethod:: emitGraphicsContextChanged
   .. automethod:: emitOutputChanged
   .. automethod:: emitStepProgress
   .. automethod:: endTaskRun
   .. automethod:: executeActions
   .. automethod:: forwardInputImage
   .. automethod:: getElapsedTime
   .. automethod:: getGraphicsMask
   .. automethod:: getInput
   .. automethod:: getInputCount
   .. automethod:: getInputDataType
   .. automethod:: getInputs
   .. automethod:: getOutput
   .. automethod:: getOutputCount
   .. automethod:: getOutputDataType
   .. automethod:: getOutputs
   .. automethod:: getParam
   .. automethod:: getProgressSteps
   .. automethod:: globalInputChanged
   .. automethod:: graphicsChanged
   .. automethod:: isGraphicsChangedListening
   .. automethod:: isMaskAvailable
   .. automethod:: parametersModified
   .. automethod:: removeInput
   .. automethod:: run
   .. automethod:: setActionFlag
   .. automethod:: setActive
   .. automethod:: setInput
   .. automethod:: setInputDataType
   .. automethod:: setInputs
   .. automethod:: setOutput
   .. automethod:: setOutputColorMap
   .. automethod:: setOutputDataType
   .. automethod:: setOutputs
   .. automethod:: setParam
   .. automethod:: stop
   .. automethod:: updateStaticOutputs
   