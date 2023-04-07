CDataStringIO
=============

String specialization of C++ template class CNumericIO.

.. currentmodule:: ikomia.dataprocess.pydataprocess

.. autoclass:: CDataStringIO

    .. rubric:: Import
    .. code-block:: python

        from ikomia.dataprocess import CDataStringIO

    .. rubric:: Methods
    .. autosummary::
        ~CNumericIO.__init__
        ~CNumericIO.add_value_list
        ~CNumericIO.get_all_header_labels
        ~CNumericIO.get_all_label_list
        ~CNumericIO.get_all_value_list
        ~CNumericIO.get_output_type
        ~CNumericIO.get_plot_type
        ~CNumericIO.get_value_list
        ~CNumericIO.set_output_type
        ~CNumericIO.set_plot_type

    .. rubric:: Overridden methods
    .. autosummary::
        ~CNumericIO.clear_data
        ~CNumericIO.from_json
        ~CNumericIO.is_data_available
        ~CNumericIO.load
        ~CNumericIO.save
        ~CNumericIO.to_json

    .. rubric:: Inherited methods
    .. autosummary::
        ~CNumericIO.copy_static_data
        ~CNumericIO.get_unit_element_count

    .. rubric:: Attributes
    .. autosummary::
        ~CNumericIO.auto_save
        ~CNumericIO.data_type
        ~CNumericIO.dim_count
        ~CNumericIO.description
        ~CNumericIO.displayable
        ~CNumericIO.name
        ~CNumericIO.source_file_path
   
   .. rubric:: Details
   .. automethod:: __init__
   .. automethod:: add_value_list
   .. automethod:: clear_data
   .. automethod:: copy_static_data
   .. automethod:: from_json
   .. automethod:: get_all_header_labels
   .. automethod:: get_all_label_list
   .. automethod:: get_all_value_list
   .. automethod:: get_output_type
   .. automethod:: get_plot_type
   .. automethod:: get_unit_element_count
   .. automethod:: get_value_list
   .. automethod:: is_data_available
   .. automethod:: load
   .. automethod:: save
   .. automethod:: set_output_type
   .. automethod:: set_plot_type
   .. automethod:: to_json
   