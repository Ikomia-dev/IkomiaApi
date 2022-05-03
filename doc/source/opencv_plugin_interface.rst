Plugin interface
==================

Code example for ocv_basics plugin interface.

.. code-block:: python

    from ikomia import dataprocess


    # --------------------
    # - Interface class to integrate the process with Ikomia application
    # - Inherits dataprocess.CPluginProcessInterface from Ikomia API
    # --------------------
    class IkomiaPlugin(dataprocess.CPluginProcessInterface):

        def __init__(self):
            dataprocess.CPluginProcessInterface.__init__(self)

        def getProcessFactory(self):
            # Instantiate process object
            from ocv_basics.ocv_basics_process import OCVBasicsFactory
            return OCVBasicsFactory()

        def getWidgetFactory(self):
            # Instantiate associated widget object
            from ocv_basics.ocv_basics_widget import OCVBasicsWidgetFactory
            return OCVBasicsWidgetFactory()