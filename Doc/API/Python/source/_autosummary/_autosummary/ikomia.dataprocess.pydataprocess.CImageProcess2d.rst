CImageProcess2d
===============

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CImageProcess2d

   .. rubric:: Import
   .. code-block:: python

         from ikomia import dataprocess


   .. rubric:: Methods
   .. autosummary::
      ~CImageProcess2d.__init__
      ~CImageProcess2d.applyGraphicsMask
      ~CImageProcess2d.applyGraphicsMaskToBinary
      ~CImageProcess2d.createGraphicsMask
      ~CImageProcess2d.emitGraphicsContextChanged
      ~CImageProcess2d.emitOutputChanged
      ~CImageProcess2d.emitStepProgress
      ~CImageProcess2d.forwardInputImage
      ~CImageProcess2d.getGraphicsMask
      ~CImageProcess2d.isMaskAvailable
      ~CImageProcess2d.setOutputColorMap

   .. rubric:: Overridden methods
   .. autosummary::
      ~CImageProcess2d.beginTaskRun
      ~CImageProcess2d.endTaskRun
      ~CImageProcess2d.graphicsChanged
      ~CImageProcess2d.setActive
      ~CImageProcess2d.updateStaticOutputs

   .. rubric:: Inherited methods
   .. autosummary::
      ~CImageProcess2d.addInput
      ~CImageProcess2d.addOutput
      ~CImageProcess2d.executeActions
      ~CImageProcess2d.getElapsedTime
      ~CImageProcess2d.getInput
      ~CImageProcess2d.getInputCount
      ~CImageProcess2d.getInputDataType
      ~CImageProcess2d.getInputs
      ~CImageProcess2d.getOutput
      ~CImageProcess2d.getOutputCount
      ~CImageProcess2d.getOutputDataType
      ~CImageProcess2d.getOutputs
      ~CImageProcess2d.getParam
      ~CImageProcess2d.getProgressSteps
      ~CImageProcess2d.globalInputChanged      
      ~CImageProcess2d.isGraphicsChangedListening      
      ~CImageProcess2d.parametersModified
      ~CImageProcess2d.removeInput
      ~CImageProcess2d.run
      ~CImageProcess2d.setActionFlag      
      ~CImageProcess2d.setInput
      ~CImageProcess2d.setInputDataType
      ~CImageProcess2d.setInputs
      ~CImageProcess2d.setOutput      
      ~CImageProcess2d.setOutputDataType
      ~CImageProcess2d.setOutputs
      ~CImageProcess2d.setParam
      ~CImageProcess2d.stop      
   
   .. rubric:: Attributes
   .. autosummary::
      ~CImageProcess2d.name
      ~CImageProcess2d.type
   
   .. rubric:: Details
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
   