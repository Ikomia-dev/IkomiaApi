CDatasetIO
==========

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CDatasetIO

   .. rubric:: Import
   .. code-block:: python

         from ikomia.dataprocess import CDatasetIO

   .. note:: A default implementation is provided in the pure Python API. See :py:mod:`~ikomia.dnn.datasetio` for details.


   .. rubric:: Methods
   .. autosummary::
      ~CDatasetIO.__init__
      ~CDatasetIO.getCategories
      ~CDatasetIO.getCategoryCount
      ~CDatasetIO.getGraphicsAnnotations
      ~CDatasetIO.getImagePaths
      ~CDatasetIO.getMaskPath
      ~CDatasetIO.getSourceFormat

   .. rubric:: Overridden methods
   .. autosummary::
      ~CDatasetIO.clearData
      ~CDatasetIO.fromJson
      ~CDatasetIO.isAutoInput
      ~CDatasetIO.isDataAvailable
      ~CDatasetIO.save
      ~CDatasetIO.toJson

   .. rubric:: Inherited methods
   .. autosummary::
      ~CDatasetIO.copyStaticData
      ~CDatasetIO.getUnitElementCount
      ~CDatasetIO.setDisplayable
   
   .. rubric:: Attributes
   .. autosummary::
      ~CDatasetIO.dataType
      ~CDatasetIO.dimCount
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: clearData
   .. automethod:: copyStaticData
   .. automethod:: fromJson
   .. automethod:: getCategories
   .. automethod:: getCategoryCount
   .. automethod:: getGraphicsAnnotations
   .. automethod:: getImagePaths
   .. automethod:: getMaskPath
   .. automethod:: getUnitElementCount
   .. automethod:: isAutoInput
   .. automethod:: isDataAvailable
   .. automethod:: save
   .. automethod:: toJson
   