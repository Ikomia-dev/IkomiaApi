Plugin interface
==================

Code example for Hello World plugin interface.

.. code-block:: python

    from ikomia import dataprocess
    import HelloWorld_process as processMod
    import HelloWorld_widget as widgetMod


    # --------------------
    # - Interface class to integrate the process with Ikomia application
    # - Inherits CPluginProcessInterface from Ikomia API
    # --------------------
    class HelloWorld(dataprocess.CPluginProcessInterface):

        def __init__(self):
            dataprocess.CPluginProcessInterface.__init__(self)

        def getProcessFactory(self):
            # Instantiate process object
            return processMod.HelloWorldProcessFactory()

        def getWidgetFactory(self):
            # Instantiate associated widget object
            return widgetMod.HelloWorldWidgetFactory()