CTextIO
=======

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CTextIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CTextIO

    .. rubric:: Methods
    .. autosummary::
        ~CTextIO.__init__
        ~CTextIO.add_text_field
        ~CTextIO.finalize
        ~CTextIO.get_graphics_io
        ~CTextIO.get_text_field_count
        ~CTextIO.get_text_field
        ~CTextIO.get_text_fields
        ~CTextIO.init

    .. rubric:: Overridden methods
    .. autosummary::
        ~CTextIO.clear_data
        ~CTextIO.is_data_available
        ~CTextIO.load
        ~CTextIO.save
        ~CTextIO.to_json
        ~CTextIO.from_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CTextIO.copy_static_data
        ~CTextIO.get_unit_element_count

    .. rubric:: Attributes
    .. autosummary::
        ~CTextIO.auto_save
        ~CTextIO.data_type
        ~CTextIO.dim_count
        ~CTextIO.description
        ~CTextIO.displayable
        ~CTextIO.name
        ~CTextIO.source_file_path

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_text_field
    .. automethod:: clear_data
    .. automethod:: finalize
    .. automethod:: get_graphics_io
    .. automethod:: get_text_field_count
    .. automethod:: get_text_field
    .. automethod:: get_text_fields
    .. automethod:: init
    .. automethod:: is_data_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: to_json
    .. automethod:: from_json