CSemanticSegmentationIO
=======================

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CSemanticSegmentationIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CSemanticSegmentationIO

    .. rubric:: Methods
    .. autosummary::
        ~CSemanticSegmentationIO.__init__
        ~CSemanticSegmentationIO.get_mask
        ~CSemanticSegmentationIO.get_class_names
        ~CSemanticSegmentationIO.get_colors
        ~CSemanticSegmentationIO.set_mask
        ~CSemanticSegmentationIO.set_class_names

    .. rubric:: Overridden methods
    .. autosummary::
        ~CSemanticSegmentationIO.clear_data
        ~CSemanticSegmentationIO.is_data_available
        ~CSemanticSegmentationIO.load
        ~CSemanticSegmentationIO.save
        ~CSemanticSegmentationIO.to_json
        ~CSemanticSegmentationIO.from_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CSemanticSegmentationIO.copy_static_data
        ~CSemanticSegmentationIO.get_unit_element_count
        ~CSemanticSegmentationIO.is_auto_input

    .. rubric:: Attributes
    .. autosummary::
        ~CSemanticSegmentationIO.auto_save
        ~CSemanticSegmentationIO.data_type
        ~CSemanticSegmentationIO.dim_count
        ~CSemanticSegmentationIO.description
        ~CSemanticSegmentationIO.displayable
        ~CSemanticSegmentationIO.name
        ~CSemanticSegmentationIO.source_file_path

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: get_mask
    .. automethod:: get_class_names
    .. automethod:: get_colors
    .. automethod:: set_mask
    .. automethod:: set_class_names
    .. automethod:: is_data_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: to_json
    .. automethod:: from_json