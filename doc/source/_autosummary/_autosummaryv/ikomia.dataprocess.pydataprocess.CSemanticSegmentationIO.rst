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
        ~CSemanticSegmentationIO.get_polygons
        ~CSemanticSegmentationIO.set_class_colors
        ~CSemanticSegmentationIO.set_class_names
        ~CSemanticSegmentationIO.set_mask

    .. rubric:: Overridden methods
    .. autosummary::
        ~CSemanticSegmentationIO.clear_data
        ~CSemanticSegmentationIO.from_json
        ~CSemanticSegmentationIO.is_data_available
        ~CSemanticSegmentationIO.load
        ~CSemanticSegmentationIO.save
        ~CSemanticSegmentationIO.to_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CSemanticSegmentationIO.copy_static_data
        ~CSemanticSegmentationIO.get_unit_element_count

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
    .. automethod:: from_json
    .. automethod:: get_mask
    .. automethod:: get_class_names
    .. automethod:: get_colors
    .. automethod:: get_polygons
    .. automethod:: is_data_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: set_class_names
    .. automethod:: set_class_colors
    .. automethod:: set_mask
    .. automethod:: to_json