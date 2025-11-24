CTextStreamIO
=============

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CTextStreamIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CTextStreamIO

    .. rubric:: Methods
    .. autosummary::
        ~CTextStreamIO.__init__
        ~CTextStreamIO.close
        ~CTextStreamIO.feed
        ~CTextStreamIO.is_feed_finished
        ~CTextStreamIO.is_read_finished
        ~CTextStreamIO.read_full
        ~CTextStreamIO.read_next_async

    .. rubric:: Overridden methods
    .. autosummary::
        ~CTextStreamIO.clear_data
        ~CTextStreamIO.is_data_available
        ~CTextStreamIO.load
        ~CTextStreamIO.save
        ~CTextStreamIO.to_json
        ~CTextStreamIO.from_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CTextStreamIO.copy_static_data
        ~CTextStreamIO.get_unit_element_count

    .. rubric:: Attributes
    .. autosummary::
        ~CTextStreamIO.auto_save
        ~CTextStreamIO.data_type
        ~CTextStreamIO.dim_count
        ~CTextStreamIO.description
        ~CTextStreamIO.displayable
        ~CTextStreamIO.name

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: clear_data
    .. automethod:: close
    .. automethod:: feed
    .. automethod:: from_json
    .. automethod:: is_data_available
    .. automethod:: is_feed_finished
    .. automethod:: is_read_finished
    .. automethod:: load
    .. automethod:: read_full
    .. automethod:: read_next_async
    .. automethod:: save
    .. automethod:: to_json
