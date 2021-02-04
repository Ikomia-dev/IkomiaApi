Plugin interface
==================

Code example for OCVBasics plugin interface.

.. code-block:: python

    from ikomia import dataprocess
    import OCVBasics_process as processMod
    import OCVBasics_widget as widgetMod


    # --------------------
    # - Interface class to integrate the process with Ikomia application
    # - Inherits dataprocess.CPluginProcessInterface from Ikomia API
    # --------------------
    class OCVBasics(dataprocess.CPluginProcessInterface):

        def __init__(self):
            dataprocess.CPluginProcessInterface.__init__(self)

        def getProcessFactory(self):
            # Instantiate process object
            return processMod.OCVBasicsProcessFactory()

        def getWidgetFactory(self):
            # Instantiate associated widget object
            return widgetMod.OCVBasicsWidgetFactory()