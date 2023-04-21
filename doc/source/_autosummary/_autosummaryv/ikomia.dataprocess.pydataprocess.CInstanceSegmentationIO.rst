CInstanceSegmentationIO
=======================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CInstanceSegmentationIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CInstanceSegmentationIO

    .. rubric:: Methods
    .. autosummary::
        ~CInstanceSegmentationIO.__init__
        ~CInstanceSegmentationIO.add_object
        ~CInstanceSegmentationIO.get_object_count
        ~CInstanceSegmentationIO.get_object
        ~CInstanceSegmentationIO.get_objects
        ~CInstanceSegmentationIO.get_merge_mask
        ~CInstanceSegmentationIO.init

    .. rubric:: Overridden methods
    .. autosummary::
        ~CInstanceSegmentationIO.clear_data
        ~CInstanceSegmentationIO.is_data_available
        ~CInstanceSegmentationIO.load
        ~CInstanceSegmentationIO.save
        ~CInstanceSegmentationIO.to_json
        ~CInstanceSegmentationIO.from_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CInstanceSegmentationIO.copy_static_data
        ~CInstanceSegmentationIO.get_unit_element_count

    .. rubric:: Attributes
    .. autosummary::
        ~CInstanceSegmentationIO.auto_save
        ~CInstanceSegmentationIO.data_type
        ~CInstanceSegmentationIO.dim_count
        ~CInstanceSegmentationIO.description
        ~CInstanceSegmentationIO.displayable
        ~CInstanceSegmentationIO.name
        ~CInstanceSegmentationIO.source_file_path

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_object
    .. automethod:: clear_data
    .. automethod:: get_object_count
    .. automethod:: get_object
    .. automethod:: get_objects
    .. automethod:: get_merge_mask
    .. automethod:: init
    .. automethod:: is_data_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: to_json
    .. automethod:: from_json