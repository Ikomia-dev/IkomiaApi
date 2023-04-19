CKeypointsIO
============

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CKeypointsIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CKeypointsIO

    .. rubric:: Methods
    .. autosummary::
        ~CKeypointsIO.__init__
        ~CKeypointsIO.add_object
        ~CKeypointsIO.get_keypoint_links
        ~CKeypointsIO.get_keypoint_names
        ~CKeypointsIO.get_object_count
        ~CKeypointsIO.get_object
        ~CKeypointsIO.get_objects
        ~CKeypointsIO.init
        ~CKeypointsIO.set_keypoint_links
        ~CKeypointsIO.set_keypoint_names

    .. rubric:: Overridden methods
    .. autosummary::
        ~CKeypointsIO.clear_data
        ~CKeypointsIO.is_data_available
        ~CKeypointsIO.load
        ~CKeypointsIO.save
        ~CKeypointsIO.to_json
        ~CKeypointsIO.from_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CKeypointsIO.copy_static_data
        ~CKeypointsIO.get_unit_element_count

    .. rubric:: Attributes
    .. autosummary::
        ~CKeypointsIO.auto_save
        ~CKeypointsIO.data_type
        ~CKeypointsIO.dim_count
        ~CKeypointsIO.description
        ~CKeypointsIO.displayable
        ~CKeypointsIO.name
        ~CKeypointsIO.source_file_path

    .. rubric:: Details
    .. automethod:: __init__
    .. automethod:: add_object
    .. automethod:: clear_data
    .. automethod:: from_json
    .. automethod:: get_keypoint_links
    .. automethod:: get_keypoint_names
    .. automethod:: get_object_count
    .. automethod:: get_object
    .. automethod:: get_objects
    .. automethod:: init
    .. automethod:: is_data_available
    .. automethod:: load
    .. automethod:: save
    .. automethod:: set_keypoint_links
    .. automethod:: set_keypoint_names
    .. automethod:: to_json