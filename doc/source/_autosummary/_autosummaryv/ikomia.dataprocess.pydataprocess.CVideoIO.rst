CVideoIO
========

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CVideoIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CVideoIO


    .. rubric:: Methods
    .. autosummary::
        ~CVideoIO.__init__
        ~CVideoIO.add_video_image
        ~CVideoIO.get_current_pos
        ~CVideoIO.get_snapshot
        ~CVideoIO.get_video_frame_count
        ~CVideoIO.get_video_images
        ~CVideoIO.get_video_path
        ~CVideoIO.has_video
        ~CVideoIO.set_video_path
        ~CVideoIO.set_video_pos
        ~CVideoIO.start_video
        ~CVideoIO.start_video_write
        ~CVideoIO.stop_video
        ~CVideoIO.stop_video_write
        ~CVideoIO.write_image

    .. rubric:: Overridden methods
    .. autosummary::
        ~CVideoIO.clear_data
        ~CVideoIO.copy_static_data
        ~CVideoIO.get_image
        ~CVideoIO.get_unit_element_count
        ~CVideoIO.is_data_available

    .. rubric:: Inherited methods
    .. autosummary::
        ~CVideoIO.draw_graphics
        ~CVideoIO.get_channel_count
        ~CVideoIO.get_data
        ~CVideoIO.get_overlay_mask
        ~CVideoIO.is_overlay_available
        ~CVideoIO.set_channel_count
        ~CVideoIO.set_current_image_index
        ~CVideoIO.set_image
        ~CVideoIO.set_overlay_mask

    .. rubric:: Attributes
    .. autosummary::
        ~CVideoIO.auto_save
        ~CVideoIO.data_type
        ~CVideoIO.dim_count
        ~CVideoIO.description
        ~CVideoIO.displayable
        ~CVideoIO.name
        ~CVideoIO.source_file_path

   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: add_video_image
   .. automethod:: clear_data
   .. automethod:: copy_static_data
   .. automethod:: get_channel_count
   .. automethod:: get_current_pos
   .. automethod:: get_data
   .. automethod:: get_image
   .. automethod:: get_overlay_mask
   .. automethod:: get_snapshot
   .. automethod:: get_unit_element_count
   .. automethod:: get_video_frame_count
   .. automethod:: get_video_images
   .. automethod:: get_video_path
   .. automethod:: has_video
   .. automethod:: is_data_available
   .. automethod:: is_overlay_available
   .. automethod:: set_channel_count
   .. automethod:: set_current_image_index
   .. automethod:: set_image
   .. automethod:: set_overlay_mask
   .. automethod:: set_video_path
   .. automethod:: set_video_pos
   .. automethod:: start_video
   .. automethod:: start_video_write
   .. automethod:: stop_video
   .. automethod:: stop_video_write
   .. automethod:: write_image
   