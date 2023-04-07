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
        ~CDatasetIO.get_categories
        ~CDatasetIO.get_category_count
        ~CDatasetIO.get_graphics_annotations
        ~CDatasetIO.get_image_paths
        ~CDatasetIO.get_mask_path
        ~CDatasetIO.get_source_format

    .. rubric:: Overridden methods
    .. autosummary::
        ~CDatasetIO.clear_data
        ~CDatasetIO.from_json
        ~CDatasetIO.is_data_available
        ~CDatasetIO.save
        ~CDatasetIO.to_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CDatasetIO.copy_static_data
        ~CDatasetIO.get_unit_element_count
   
    .. rubric:: Attributes
    .. autosummary::
        ~CDatasetIO.auto_save
        ~CDatasetIO.data_type
        ~CDatasetIO.dim_count
        ~CDatasetIO.description
        ~CDatasetIO.displayable
        ~CDatasetIO.name
        ~CDatasetIO.source_file_path
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: clear_data
   .. automethod:: copy_static_data
   .. automethod:: from_json
   .. automethod:: get_categories
   .. automethod:: get_category_count
   .. automethod:: get_graphics_annotations
   .. automethod:: get_image_paths
   .. automethod:: get_mask_path
   .. automethod:: get_unit_element_count
   .. automethod:: is_data_available
   .. automethod:: save
   .. automethod:: to_json
   