Input/Output management
=======================


Every algorithm in Ikomia platform comes with a list of inputs and outputs. Depending of the algorithm, 
you will have to deal with several types. This API provides a comprehensive list of I/O types to 
address common needs in Computer Vision.

Ideally, inputs and outputs should be defined in the constructor of the task.

.. note:: 
    In some case, you may need to dynamically set inputs and/or outputs based on parameters values. You can handle such case by implementing 
    :py:meth:`~ikomia.core.pycore.CWorkflowTask.parametersModified` and calling either :py:meth:`~ikomia.core.pycore.CWorkflowTask.addInput` or 
    :py:meth:`~ikomia.core.pycore.CWorkflowTask.removeInput` from your task class.


Image
-----

:py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO`: input or output containing image data stored as Numpy array with dimensions [HWC]. 
When the number of channels is 3, the color format should be RGB.

Basic usage:

.. code-block:: python

    from ikomia import core, dataprocess

    class MyPlugin(core.CWorkflowTask):
        def init(self, name, param):
            # Add image input
            self.addInput(dataprocess.CImageIO())
            # Add image output
            self.addOutput(dataprocess.CImageIO())

        def run(self):
            # Get input
            image_input = self.getInput(0)
            # Get image as Numpy array
            image = image_input.getImage()

Please consult :py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO` for details.


Graphics
--------

In Ikomia platform, graphics represent all vectorial items (line, polygon, text...) that 
bring additionnal information to images. They can be stored as input (:py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsInput`) 
or output (:py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput`). Different types of graphics 
are provided, each one being implemented in a dedicated class:

- Point: :py:mod:`~ikomia.core.pycore.CGraphicsPoint`
- Polyline: :py:mod:`~ikomia.core.pycore.CGraphicsPolyline`
- Rectangle/Square: :py:mod:`~ikomia.core.pycore.CGraphicsRectangle`
- Ellipse/Circle: :py:mod:`~ikomia.core.pycore.CGraphicsEllipse`
- Polygon: :py:mod:`~ikomia.core.pycore.CGraphicsPolygon`
- Polygon with hole(s): :py:mod:`~ikomia.core.pycore.CGraphicsComplexPolygon`
- Text: :py:mod:`~ikomia.core.pycore.CGraphicsText`

Basic usage:

.. code-block:: python

    from ikomia import core, dataprocess

    class MyPlugin(core.CWorkflowTask):
        def init(self, name, param):
            # Add graphics input
            self.addInput(dataprocess.CGraphicsInput())
            # Add graphics output
            self.addOutput(dataprocess.CGraphicsOutput())

        def run(self):
            # Get graphics input: from another algorithm or user
            graphics_input = self.getInput(0)
            items = graphics_input.getItems()

            for item in items:
                if item.getType() == core.GraphicsItem.RECTANGLE:
                    top_left = (item.x, item.y)
                    width = item.width
                    height = item.height

            # Fill graphics output
            graphics_output = self.getOutput(0)
            graphics_output.addEllipse(0, 0, 100, 200)
            graphics_output.addItem(core.CGraphicRectangle(0, 0, 100, 200))
            
.. note:: 
    - In Ikomia Studio, you can display items of graphics output as an overlay layer on top of an image I/O. You just have to call :py:meth:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput.setImageIndex` and specify the index of the desired image I/O. 
    - Some useful functions are implemented in :py:mod:`~ikomia.dataprocess.pydataprocess.C2dImageTask` to manage graphics items.
    - From :py:mod:`~ikomia.dataprocess.pydataprocess.CImageIO` you can burn items of a :py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput` object directly into the image array. See :py:meth:`~ikomia.dataprocess.pydataprocess.CImageIO.getImageWithGraphics` and :py:meth:`~ikomia.dataprocess.pydataprocess.CImageIO.drawGraphics`.


Numeric values
--------------

:py:mod:`~ikomia.dataprocess.pydataprocess.CNumericIO`: input or output dedicated to handle numeric values as float. 
Data structure is organized to be visualize in a column/row table. You can also add labels describing 
those values: *header labels* for columns and *labels* for rows.

Basic usage:

.. code-block:: python

    from ikomia import core, dataprocess

    class MyPlugin(core.CWorkflowTask):
        def init(self, name, param):
            # Add numeric output
            self.addOutput(dataprocess.CNumericIO())

        def run(self):
            # Fill numeric output
            header = "Confidence"
            labels = ["Car", "Truck", "Moto", "Bike", "Plane", "Train"]
            confidences = [0.8, 0.75, 0.2, 0.05, 0.04, 0.01]
            numeric_output = self.getOutput(0)
            numeric_output.setOutputType(dataprocess.NumericOutputType.TABLE)
            numeric_output.addValueList(confidences, header, labels)

.. note:: 
    In Ikomia Studio, :py:mod:`~ikomia.dataprocess.pydataprocess.CNumericIO` can be display differently depending on the value set with :py:meth:`~ikomia.dataprocess.pydataprocess.CNumericIO.setOutputType`. 
    Possible values are listed in :py:mod:`~ikomia.dataprocess.pydataprocess.NumericOutputType`. If **PLOT** is choosen, then you have to select the plot type with :py:meth:`~ikomia.dataprocess.pydataprocess.CNumericIO.setPlotType`.

Basic usage:

.. code-block:: python

    from ikomia import core, dataprocess

    class MyPlugin(core.CWorkflowTask):
        def init(self, name, param):
            # Add numeric output
            self.addOutput(dataprocess.CNumericIO())

        def run(self):
            # Fill numeric output
            header = "Confidence"
            labels = ["Car", "Truck", "Moto", "Bike", "Plane", "Train"]
            confidences = [0.8, 0.75, 0.2, 0.05, 0.04, 0.01]
            numeric_output = self.getOutput(0)
            numeric_output.setOutputType(dataprocess.NumericOutputType.TABLE)
            numeric_output.addValueList(confidences, header, labels)


String values
--------------

:py:mod:`~ikomia.dataprocess.pydataprocess.CDataStringIO`: input or output dedicated to output string values.
Data structure is organized to be visualize in a column/row table. You can also add labels describing
those values: header labels for columns and labels for rows.

Basic usage:

.. code-block:: python

    from ikomia import core, dataprocess

    class MyPlugin(core.CWorkflowTask):
        def init(self, name, param):
            # Add numeric output
            self.addOutput(dataprocess.CDataStringIO())

        def run(self):
            # Fill numeric output
            header = "TextRecognition"
            labels = ["Distributor", "Date", "Article", "Price", "Currency"]
            ocr_res = ["eShop", "04/12/2021", "smartphone", "199.90", "euro"]
            numeric_output = self.getOutput(0)
            numeric_output.setOutputType(dataprocess.NumericOutputType.TABLE)
            numeric_output.addValueList(ocr_res, header, labels)


Blob measures
-------------

:py:mod:`~ikomia.dataprocess.pydataprocess.CBlobMeasureIO`: input or output dedicated to handle measures computed on BLOBs (Binary Large Objects).
BLOBs are regions or connected components extracted from image based on specific properties (color, brightness, edges...). A CBlobMeasureIO instance 
stores a list of :py:mod:`~ikomia.dataprocess.pydataprocess.CObjectMeasure` (one for each BLOB). Here is the list of available measures:

- Surface (core.MeasureId.SURFACE)
- Perimeter (core.MeasureId.PERIMETER)
- Centroïd (core.MeasureId.CENTROID)
- Bounding box (core.MeasureId.BBOX): top-left point coordinates, width, height
- Oriented bounding box (core.MeasureId.ORIENTED_BBOX): center point coordinates, width, height, angle of rotation
- Equivalent diameter (core.MeasureId.EQUIVALENT_DIAMETER): diameter of the minimum enclosing circle computed from the surface
- Elongation (core.MeasureId.ELONGATION): elongation factor computed from moments (R. Mukundan and K.R. Ramakrishnan. Moment Functions in Image Analysis –Theory and Applications. World Scientific, 1998)
- Circularity (core.MeasureId.CIRCULARITY): circularity factor in [0, 1] computed from surface and perimeter
- Solidity (core.MeasureId.SOLIDITY): compactness factor defined as a ratio between blob surface and convex hull surface

BLOB measure can be associated with a graphics element from a :py:mod:`~ikomia.dataprocess.pydataprocess.CGraphicsOutput` and store the corresponding 
graphics id in :py:mod:`~ikomia.dataprocess.pydataprocess.CObjectMeasure`.

Basic usage:

.. code-block:: python

    from ikomia.core import CWorkflowTask, CMeasure, MeasureId
    from ikomia.dataprocess import CBlobMeasureIO, CObjectMeasure

    class MyPlugin(CWorkflowTask):
        def init(self, name, param):
            # Add BLOB measure output
            self.addOutput(CBlobMeasureIO())

        def run(self):
            # Fill blob measure output
            blob_output = self.getOutput(0)
            boxes, confidences = my_object_detection_func()
            measures = []
            graphics_id = -1

            for box, conf in zip(boxes, confidences):
                measures.append(CObjectMeasure(CMeasure(MeasureId.BBOX), [box.x, box.y, box.width, box.height], graphics_id, "label"))
                measures.append(CObjectMeasure(CMeasure(MeasureId.CUSTOM, "Confidence"), conf, graphics_id, "label"))
                blob_output.addObjectMeasures(measures)


.. note:: In Ikomia Studio, :py:mod:`~ikomia.dataprocess.pydataprocess.CBlobMeasureIO` outputs are visualized in tables.


Multi-dimensional array
-----------------------

:py:mod:`~ikomia.dataprocess.pydataprocess.CArrayIO`: input or output dedicated to handle multi-dimensional array. 
:py:mod:`~ikomia.dataprocess.pydataprocess.CArrayIO` instance just stores a Numpy array that will be not considered as image. 
Thus, such I/O are not visualized in Ikomia Studio.


Deep learning dataset
---------------------

:py:mod:`~ikomia.dnn.datasetio.IkDatasetIO`: input or output dedicated to handle deep learning image dataset. The Ikomia platform aims 
to simplify the use of state of the art algorithms, especially training algorithms. The idea behind :py:mod:`~ikomia.dnn.datasetio.IkDatasetIO` 
is to define a common structure so that every datasets converted to Ikomia format can then be used by any training algorithms of Ikomia 
Marketplace. Ikomia dataset structure is inspired by PyTorch and Detectron2. It is composed of a global dict with 2 main entries 
‘images’ and ‘metadata’.

This API provides built-in function to manage standard dataset format:

- COCO (2017): :py:func:`~ikomia.dnn.dataset.load_coco_dataset`.
- PASCAL-VOC (2012): :py:func:`~ikomia.dnn.dataset.load_pascalvoc_dataset`.
- VIA (VGG Image Annotator): :py:func:`~ikomia.dnn.dataset.load_via_dataset`.
- YOLO: :py:func:`~ikomia.dnn.dataset.load_yolo_dataset`.

More information in :py:mod:`~ikomia.dnn.dataset`.

Basic usage:

.. code-block:: python

    from ikomia.core import CWorkflowTask
    from ikomia.dnn.datasetio import IkDatasetIO

    class MyPlugin(CWorkflowTask):
        def init(self, name, param):
            # Add dataset input
            self.addInput(IkDatasetIO())
            # Add dataset output
            self.addOutput(IkDatasetIO())

        def run(self):
            # Load dataset
            dataset = self.getOutput(0)
            dataset.data = my_dataset_loader_func()

.. note:: 
    You will find other dataset loaders in the Ikomia Marketplace. You can also consult our GitHub repository to find implementation details (dataset_wgisd for example).


Filesystem path
---------------

:py:mod:`~ikomia.dataprocess.pydataprocess.CPathIO`: input or output dedicated to handle folder or file path.

Basic usage:

.. code-block:: python

    import os
    from ikomia.core import CWorkflowTask, IODataType
    from ikomia.dataprocess import CPathIO

    class MyPlugin(CWorkflowTask):
        def init(self, name, param):
            # Add path input
            self.addInput(CPathIO(IODataType.FILE_PATH))
            # Add path output
            default_path = "/usr/local"
            self.addOutput(CPathIO(IODataType.FOLDER_PATH, default_path))

        def run(self):
            # Get path
            path_input = self.getInput(0)
            path_in = path_input.getPath()

            # Set path
            path_output = self.getOutput(0)
            path_output.setPath(os.path.dirname(path_in))