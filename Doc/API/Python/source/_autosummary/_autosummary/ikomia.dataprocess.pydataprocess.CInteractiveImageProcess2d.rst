CInteractiveImageProcess2d
==========================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CInteractiveImageProcess2d

   .. rubric:: Import
   .. code-block:: python

         from ikomia import dataprocess

   .. rubric:: Methods
   .. autosummary::
      ~CInteractiveImageProcess2d.__init__
      ~CInteractiveImageProcess2d.clearInteractionLayer
      ~CInteractiveImageProcess2d.computeBlobs
      ~CInteractiveImageProcess2d.createInteractionMask
      ~CInteractiveImageProcess2d.emitGraphicsContextChanged
      ~CInteractiveImageProcess2d.emitOutputChanged
      ~CInteractiveImageProcess2d.emitStepProgress
      ~CInteractiveImageProcess2d.getBlobs

   .. rubric:: Overridden methods
   .. autosummary::
      ~CInteractiveImageProcess2d.globalInputChanged
      ~CInteractiveImageProcess2d.graphicsChanged
      ~CInteractiveImageProcess2d.isGraphicsChangedListening
      ~CInteractiveImageProcess2d.setActive

   .. rubric:: Inherited methods
   .. autosummary::
      ~CInteractiveImageProcess2d.addInput
      ~CInteractiveImageProcess2d.addOutput
      ~CInteractiveImageProcess2d.applyGraphicsMask
      ~CInteractiveImageProcess2d.applyGraphicsMaskToBinary
      ~CInteractiveImageProcess2d.beginTaskRun
      ~CInteractiveImageProcess2d.createGraphicsMask
      ~CInteractiveImageProcess2d.endTaskRun
      ~CInteractiveImageProcess2d.executeActions
      ~CInteractiveImageProcess2d.forwardInputImage
      ~CInteractiveImageProcess2d.getElapsedTime
      ~CInteractiveImageProcess2d.getGraphicsMask
      ~CInteractiveImageProcess2d.getInput
      ~CInteractiveImageProcess2d.getInputCount
      ~CInteractiveImageProcess2d.getInputDataType
      ~CInteractiveImageProcess2d.getInputs
      ~CInteractiveImageProcess2d.getInteractionMask
      ~CInteractiveImageProcess2d.getOutput
      ~CInteractiveImageProcess2d.getOutputCount
      ~CInteractiveImageProcess2d.getOutputDataType
      ~CInteractiveImageProcess2d.getOutputs
      ~CInteractiveImageProcess2d.getParam
      ~CInteractiveImageProcess2d.getProgressSteps
      ~CInteractiveImageProcess2d.isMaskAvailable
      ~CInteractiveImageProcess2d.parametersModified
      ~CInteractiveImageProcess2d.removeInput
      ~CInteractiveImageProcess2d.run
      ~CInteractiveImageProcess2d.setActionFlag
      ~CInteractiveImageProcess2d.setInput
      ~CInteractiveImageProcess2d.setInputDataType
      ~CInteractiveImageProcess2d.setInputs
      ~CInteractiveImageProcess2d.setOutput
      ~CInteractiveImageProcess2d.setOutputColorMap
      ~CInteractiveImageProcess2d.setOutputDataType
      ~CInteractiveImageProcess2d.setOutputs
      ~CInteractiveImageProcess2d.setParam
      ~CInteractiveImageProcess2d.stop
      ~CInteractiveImageProcess2d.updateStaticOutputs
   
   .. rubric:: Attributes
   .. autosummary::
      ~CInteractiveImageProcess2d.name
      ~CInteractiveImageProcess2d.type
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: addInput
   .. automethod:: addOutput
   .. automethod:: applyGraphicsMask
   .. automethod:: applyGraphicsMaskToBinary
   .. automethod:: beginTaskRun
   .. automethod:: clearInteractionLayer
   .. automethod:: computeBlobs
   .. automethod:: createGraphicsMask
   .. automethod:: createInteractionMask
   .. automethod:: emitGraphicsContextChanged
   .. automethod:: emitOutputChanged
   .. automethod:: emitStepProgress
   .. automethod:: endTaskRun
   .. automethod:: executeActions
   .. automethod:: forwardInputImage
   .. automethod:: getBlobs
   .. automethod:: getElapsedTime
   .. automethod:: getGraphicsMask
   .. automethod:: getInput
   .. automethod:: getInputCount
   .. automethod:: getInputDataType
   .. automethod:: getInputs
   .. automethod:: getInteractionMask
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
