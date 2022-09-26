Welcome to Ikomia API documentation
===================================

:Current version: |version|

The Ikomia platform is made up of three components:

    - **Ikomia HUB**: HUB is the central sharing place for Plug'n'Play algorithms in Computer Vision and Deep Learning. We regularly push the best state of the art algorithms in classification, object detection, segmentation or pose estimation (among others). With Ikomia HUB, all algorithms are Open Source and ready to use in few clicks and you can easily edit the code if you want.
    - **Ikomia Studio**: Studio is an Open Source no-code application (Linux and Windows) that allows you to test, chain and train any Computer Vision algorithms with a drag and drop system. You can load all images and video formats, and select the best state of the art algorithms from Ikomia HUB.
    - **Ikomia API**: Python API to leverage state of the art algorithms for your own computer vision application and/or create plugins for Ikomia Studio and Ikomia HUB.


.. note:: Ikomia API is built around Ikomia Core, a set of C++ libraries with Python bindings.


.. important:: Ikomia Studio and Ikomia API are Open-Source projects, distributed under AGPLv3 and Apache 2.0 respectively.


Choose your API usage
---------------------
As an Ikomia user, you will be led to use this API for two reasons:

    - **Create workflows and use Ikomia algorithms in your custom application**: leverage built-in algorithms and those from Ikomia HUB directly in your application. Run single algorithm or workflows with few lines of code.
    - **Create plugins for Ikomia platform**: with a little wrap around your Python code, you are able to give access to your algorithm from a user friendly software (Ikomia Studio) and a flexible workflow API. Moreover, from Ikomia Studio you can publish your work to Ikomia HUB in one click and share it with the community.


**Ready to start?**

.. toctree::
    :maxdepth: 1

    index_api
    index_plugin


Useful links
------------

`Ikomia GitHub repository <https://github.com/Ikomia-dev/>`_

`Ikomia HUB algorithms GitHub repository <https://github.com/Ikomia-hub/>`_

`Ikomia blog <https://blog.ikomia.com/>`_

`Ikomia web site <https://ikomia.com/en/>`_


API Reference
-------------
This section provides the auto-generated documentation from the Python code. You will find all the implementation details and
the comprehensive list of available functions.

.. toctree::
   :maxdepth: 2

   modules


Indices and tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
