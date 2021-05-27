CVideoProcessTracking
=====================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CVideoProcessTracking

   .. rubric:: Import
   .. code-block:: python

         from ikomia import dataprocess

   .. rubric:: Methods
   .. autosummary::
      ~CVideoProcessTracking.__init__
      ~CVideoProcessTracking.manageOutputs
      ~CVideoProcessTracking.setRoiToTrack

   .. rubric:: Overridden methods
   .. autosummary::
      ~CVideoProcessTracking.notifyVideoEnd
      ~CVideoProcessTracking.notifyVideoStart

   .. rubric:: Inherited methods
   .. autosummary::      
      ~CVideoProcessTracking.addInput
      ~CVideoProcessTracking.addOutput
      ~CVideoProcessTracking.applyGraphicsMask
      ~CVideoProcessTracking.applyGraphicsMaskToBinary
      ~CVideoProcessTracking.beginTaskRun
      ~CVideoProcessTracking.createGraphicsMask
      ~CVideoProcessTracking.emitGraphicsContextChanged
      ~CVideoProcessTracking.emitOutputChanged
      ~CVideoProcessTracking.emitStepProgress
      ~CVideoProcessTracking.endTaskRun
      ~CVideoProcessTracking.executeActions
      ~CVideoProcessTracking.forwardInputImage
      ~CVideoProcessTracking.getElapsedTime
      ~CVideoProcessTracking.getGraphicsMask
      ~CVideoProcessTracking.getInput
      ~CVideoProcessTracking.getInputCount
      ~CVideoProcessTracking.getInputDataType
      ~CVideoProcessTracking.getInputs
      ~CVideoProcessTracking.getOutput
      ~CVideoProcessTracking.getOutputCount
      ~CVideoProcessTracking.getOutputDataType
      ~CVideoProcessTracking.getOutputs
      ~CVideoProcessTracking.getParam
      ~CVideoProcessTracking.getProgressSteps
      ~CVideoProcessTracking.globalInputChanged
      ~CVideoProcessTracking.graphicsChanged
      ~CVideoProcessTracking.isGraphicsChangedListening
      ~CVideoProcessTracking.isMaskAvailable
      ~CVideoProcessTracking.parametersModified
      ~CVideoProcessTracking.removeInput
      ~CVideoProcessTracking.run
      ~CVideoProcessTracking.setActionFlag
      ~CVideoProcessTracking.setActive
      ~CVideoProcessTracking.setInput
      ~CVideoProcessTracking.setInputDataType
      ~CVideoProcessTracking.setInputs
      ~CVideoProcessTracking.setOutput
      ~CVideoProcessTracking.setOutputColorMap
      ~CVideoProcessTracking.setOutputDataType
      ~CVideoProcessTracking.setOutputs
      ~CVideoProcessTracking.setParam
      ~CVideoProcessTracking.stop
      ~CVideoProcessTracking.updateStaticOutputs

   .. rubric:: Attributes
   .. autosummary::
      ~CVideoProcessTracking.name
      ~CVideoProcessTracking.type
   
   .. automethod:: __init__
   .. automethod:: addInput
   .. automethod:: addOutput
   .. automethod:: applyGraphicsMask
   .. automethod:: applyGraphicsMaskToBinary
   .. automethod:: beginTaskRun
   .. automethod:: createGraphicsMask
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
