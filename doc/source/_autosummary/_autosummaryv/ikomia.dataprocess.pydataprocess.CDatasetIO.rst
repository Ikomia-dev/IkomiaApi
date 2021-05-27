CDatasetIO
==========

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CDatasetIO

   .. rubric:: Import
   .. code-block:: python

         from ikomia import dataprocess

   **Note**: a default implementation is provided in the pure Python API.
   See :py:mod:`datasetio` for details.

   .. rubric:: Methods
   .. autosummary::
      ~CDatasetIO.__init__
      ~CDatasetIO.getCategories
      ~CDatasetIO.getCategoryCount
      ~CDatasetIO.getGraphicsAnnotations
      ~CDatasetIO.getImagePaths
      ~CDatasetIO.getMaskPath

   .. rubric:: Overridden methods
   .. autosummary::
      ~CDatasetIO.clearData
      ~CDatasetIO.isAutoInput
      ~CDatasetIO.isDataAvailable
      ~CDatasetIO.save

   .. rubric:: Inherited methods
   .. autosummary::
      ~CDatasetIO.copyStaticData
      ~CDatasetIO.getUnitElementCount
   
   .. rubric:: Attributes
   .. autosummary::
      ~CDatasetIO.dataType
      ~CDatasetIO.dimCount
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: clearData
   .. automethod:: copyStaticData
   .. automethod:: getCategories
   .. automethod:: getCategoryCount
   .. automethod:: getGraphicsAnnotations
   .. automethod:: getImagePaths
   .. automethod:: getMaskPath
   .. automethod:: getUnitElementCount
   .. automethod:: isAutoInput
   .. automethod:: isDataAvailable
   .. automethod:: save
   