Display tools for debugging
===========================

The :py:mod:`~ikomia.dataprocess.displayIO` module aims to give display output based on Matplolib for every Ikomia input/output types.
We provide these features for debugging purpose essentially.

So it's very simple, you just have to call a single function: :py:meth:`~ikomia.dataprocess.displayIO.display`.

.. code-block:: python

    from ikomia.dataprocess import workflow, displayIO

    wf = workflow.create("ObjectDetection")
    yolov4_id, yolov4_obj = wf.add_task("infer_yolo_v4")
    wf.connect_tasks(wf.getRootID(), yolov4_id)

    wf.run_on(path="path/to/image.png")

    # Display image output
    img_output = wf.get_image_output(yolov4_id)
    displayIO.display(img_output)

    # Display graphics output
    graphics_output = wf.get_graphics_output(yolov4_id)
    displayIO.display(graphics_output)

    # Display blob measures output
    box_output = wf.get_blob_measure_output(yolov4_id)
    displayIO.display(box_output)


You can also apply the :py:meth:`~ikomia.dataprocess.displayIO.display` on:

- **task** object instance: this will show a figure with inputs and outputs of the given task.
- **workflow** object instance: this will show a GraphViz representation of the workflow.

.. important::
    Matplotlib is used to generate almost all figures, so you have to make sure you have a valid GUI backend to display 
    the figures. This will, for example, not work on Linux machine or docker image where no X server is installed.

