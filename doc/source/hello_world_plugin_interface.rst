Plugin interface
==================

Code example for hello_world plugin interface.

.. code-block:: python

    from ikomia import dataprocess


    # --------------------
    # - Interface class to integrate the process with Ikomia application
    # - Inherits CPluginProcessInterface from Ikomia API
    # --------------------
    class IkomiaPlugin(dataprocess.CPluginProcessInterface):

        def __init__(self):
            dataprocess.CPluginProcessInterface.__init__(self)

        def getProcessFactory(self):
            # Instantiate process object
            from hello_world.hello_world_process import HelloWorldFactory
            return HelloWorldFactory()

        def getWidgetFactory(self):
            # Instantiate associated widget object
            from hello_world.hello_world_widget import HelloWorldWidgetFactory
            return HelloWorldWidgetFactory()