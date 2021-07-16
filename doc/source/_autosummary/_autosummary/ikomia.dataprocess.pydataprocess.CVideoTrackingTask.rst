CVideoTrackingTask
==================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CVideoTrackingTask

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import CVideoTrackingTask

   .. rubric:: Methods
   .. autosummary::
      ~CVideoTrackingTask.__init__
      ~CVideoTrackingTask.manageOutputs
      ~CVideoTrackingTask.setRoiToTrack

   .. rubric:: Overridden methods
   .. autosummary::
      ~CVideoTrackingTask.notifyVideoEnd
      ~CVideoTrackingTask.notifyVideoStart

   .. rubric:: Inherited methods
   .. autosummary::      
      ~CVideoTrackingTask.addInput
      ~CVideoTrackingTask.addOutput
      ~CVideoTrackingTask.applyGraphicsMask
      ~CVideoTrackingTask.applyGraphicsMaskToBinary
      ~CVideoTrackingTask.beginTaskRun
      ~CVideoTrackingTask.createGraphicsMask
      ~CVideoTrackingTask.emitAddSubProgressSteps
      ~CVideoTrackingTask.emitGraphicsContextChanged
      ~CVideoTrackingTask.emitOutputChanged
      ~CVideoTrackingTask.emitStepProgress
      ~CVideoTrackingTask.endTaskRun
      ~CVideoTrackingTask.executeActions
      ~CVideoTrackingTask.forwardInputImage
      ~CVideoTrackingTask.getElapsedTime
      ~CVideoTrackingTask.getGraphicsMask
      ~CVideoTrackingTask.getInput
      ~CVideoTrackingTask.getInputCount
      ~CVideoTrackingTask.getInputDataType
      ~CVideoTrackingTask.getInputs
      ~CVideoTrackingTask.getOutput
      ~CVideoTrackingTask.getOutputCount
      ~CVideoTrackingTask.getOutputDataType
      ~CVideoTrackingTask.getOutputs
      ~CVideoTrackingTask.getParam
      ~CVideoTrackingTask.getProgressSteps
      ~CVideoTrackingTask.globalInputChanged
      ~CVideoTrackingTask.graphicsChanged
      ~CVideoTrackingTask.isGraphicsChangedListening
      ~CVideoTrackingTask.isMaskAvailable
      ~CVideoTrackingTask.parametersModified
      ~CVideoTrackingTask.removeInput
      ~CVideoTrackingTask.run
      ~CVideoTrackingTask.setActionFlag
      ~CVideoTrackingTask.setActive
      ~CVideoTrackingTask.setInput
      ~CVideoTrackingTask.setInputDataType
      ~CVideoTrackingTask.setInputs
      ~CVideoTrackingTask.setOutput
      ~CVideoTrackingTask.setOutputColorMap
      ~CVideoTrackingTask.setOutputDataType
      ~CVideoTrackingTask.setOutputs
      ~CVideoTrackingTask.setParam
      ~CVideoTrackingTask.stop
      ~CVideoTrackingTask.updateStaticOutputs

   .. rubric:: Attributes
   .. autosummary::
      ~CVideoTrackingTask.name
      ~CVideoTrackingTask.type
   
   .. automethod:: __init__
   .. automethod:: addInput
   .. automethod:: addOutput
   .. automethod:: applyGraphicsMask
   .. automethod:: applyGraphicsMaskToBinary
   .. automethod:: beginTaskRun
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
   .. automethod:: manageOutputs
   .. automethod:: notifyVideoEnd
   .. automethod:: notifyVideoStart
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
   .. automethod:: setRoiToTrack
   .. automethod:: stop
   .. automethod:: updateStaticOutputs
