CImageIO
========

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CImageIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CImageIO


    .. rubric:: Methods
    .. autosummary::
        ~CImageIO.__init__
        ~CImageIO.draw_graphics
        ~CImageIO.get_channel_count
        ~CImageIO.get_data
        ~CImageIO.get_image
        ~CImageIO.get_image_with_graphics
        ~CImageIO.get_image_with_mask
        ~CImageIO.get_image_with_mask_and_graphics
        ~CImageIO.get_overlay_mask
        ~CImageIO.is_overlay_available
        ~CImageIO.set_channel_count
        ~CImageIO.set_image
        ~CImageIO.set_overlay_mask

    .. rubric:: Overridden methods
    .. autosummary::
        ~CImageIO.clear_data
        ~CImageIO.copy_static_data
        ~CImageIO.get_unit_element_count
        ~CImageIO.is_data_available
        ~CImageIO.load
        ~CImageIO.save
        ~CImageIO.to_json
        ~CImageIO.from_json
   
    .. rubric:: Attributes
    .. autosummary::
        ~CImageIO.auto_save
        ~CImageIO.data_type
        ~CImageIO.dim_count
        ~CImageIO.description
        ~CImageIO.displayable
        ~CImageIO.name
        ~CImageIO.source_file_path

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: clear_data
    .. automethod:: copy_static_data
    .. automethod:: draw_graphics
    .. automethod:: from_json
    .. automethod:: get_channel_count
    .. automethod:: get_data
    .. automethod:: get_image
    .. automethod:: get_image_with_graphics
    .. automethod:: get_image_with_mask
    .. automethod:: get_image_with_mask_and_graphics
    .. automethod:: get_overlay_mask
    .. automethod:: get_unit_element_count
    .. automethod:: is_data_available
    .. automethod:: is_overlay_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: set_channel_count
    .. automethod:: set_image
    .. automethod:: set_overlay_mask
    .. automethod:: to_json
   