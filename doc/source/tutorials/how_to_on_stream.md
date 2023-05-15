# First Steps with Camera Stream Processing with Ikomia

Camera stream processing refers to the real-time analysis and manipulation of images and video streams captured from a camera. 
This technique is widely used in various applications, such as computer vision, surveillance, robotics, and entertainment, among others.

In computer vision, camera stream processing is used for object detection and recognition, face detection, motion tracking, and image segmentation. 

In **surveillance**, camera stream processing is used for detecting anomalies and events, such as intrusion detection and crowd behavior analysis. 

In **robotics**, camera stream processing is used for autonomous navigation, object detection, and obstacle avoidance. 

In **entertainment**, camera stream processing is used for augmented reality, virtual reality, and gesture recognition.

Overall, camera stream processing plays a crucial role in various fields and enables many exciting applications that were once considered impossible.

To get started with camera stream processing, we will use OpenCV and `VideoCapture`.

```{warning}
In this part, you need to be familiar with Ikomia Workflow.

Please read the [Getting Started](../getting_started) and [Going deeper with worflows](../advanced_guide/index) before this tutorial.
```

## Process Stream from your Webcam

### Test your Webcam without Ikomia Workflow

Start by testing your webcam with the following code :
```python
from ikomia.utils.displayIO import display
import cv2

stream = cv2.VideoCapture(0)

while True:
    # Read image from stream
    ret, frame = stream.read()
    
    # Test if streaming is OK
    if not ret:
        continue
    
    # Display image with OpenCV
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    display(frame, viewer="opencv", title="Ikomia Demo")

    # Press 'q' to quit the streaming process
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the stream object
stream.release()
# Destroy all windows
cv2.destroyAllWindows()
```
In this code, we used `cv2.cvtColor()` because OpenCV reads images as BGR (Blue, Red, Green) buffers whereas Ikomia needs RGB images.

### Basic Process with Ikomia Workflow

We introduce Ikomia Workflow in the process and we integrate color conversion directly in the workflow.

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik
from ikomia.utils.displayIO import display
import cv2

stream = cv2.VideoCapture(0)

# Init the workflow
wf = Workflow()

# Add color conversion to workflow
cvt = wf.add_task(ik.ocv_color_conversion(code=str(cv2.COLOR_BGR2RGB)), auto_connect=True)

while True:
    # Read image from stream
    ret, frame = stream.read()

    # Test if streaming is OK
    if not ret:
        continue
        
    # Run workflow on image
    wf.run_on(frame)
    
    # Display results from "cvt"
    display(cvt.get_output(0).get_image(), title="Demo", viewer="opencv")

    # Press 'q' to quit the streaming process
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the stream object
stream.release()
# Destroy all windows
cv2.destroyAllWindows()
```

### Face detection

We just add a face detector to the workflow and display the corresponding results.

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik
from ikomia.utils.displayIO import display
import cv2

stream = cv2.VideoCapture(0)

# Init the workflow
wf = Workflow()

# Add color conversion
cvt = wf.add_task(ik.ocv_color_conversion(code=str(cv2.COLOR_BGR2RGB)), auto_connect=True)

# Add face detection
face = wf.add_task(ik.infer_face_detection_kornia(conf_thres="0.8"), auto_connect=True)

while True:
    ret, frame = stream.read()
    
    # Test if streaming is OK
    if not ret:
        continue
        
    # Run workflow on image
    wf.run_on(frame)

    # Display results from "face"
    display(face.get_image_with_graphics(), title="Demo", viewer="opencv")

    # Press 'q' to quit the streaming process
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the stream object
stream.release()
# Destroy all windows
cv2.destroyAllWindows()
```

### Blurring Face Detection

We just add a blurring algorithm for each detected face and display the corresponding results.

```python
from ikomia.dataprocess.workflow import Workflow
from ikomia.utils import ik
from ikomia.utils.displayIO import display
import cv2

stream = cv2.VideoCapture(0)

# Init the workflow
wf = Workflow()

# Add color conversion
cvt = wf.add_task(ik.ocv_color_conversion(code=str(cv2.COLOR_BGR2RGB)), auto_connect=True)

# Add face detection
face = wf.add_task(ik.infer_face_detection_kornia(conf_thres="0.8"), auto_connect=True)

# Add face blurring
blur = wf.add_task(ik.ocv_stack_blur(kSizeHeight="61", kSizeWidth="61"), auto_connect=True)

while True:
    ret, frame = stream.read()
    
    # Test if streaming is OK
    if not ret:
        continue

    # Run workflow on image
    wf.run_on(frame)

    # Display results from "face" and "blur"
    display(face.get_image_with_graphics(), title="Demo Face Detection", viewer="opencv")
    display(blur.get_output(0).get_image(), title="Demo Blurry Face Detection", viewer="opencv")

    # Press 'q' to quit the streaming process
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the stream object
stream.release()
# Destroy all windows
cv2.destroyAllWindows()
```

## Troubleshooting Tips

Here are some common errors that users may encounter when processing camera streams, along with some troubleshooting tips:

1. **"Error opening video capture device"**: This error occurs when the video capture device cannot be opened. This could be due to the device being busy or not connected properly. Make sure that the device is connected and try restarting the application.
2. **"Unable to retrieve frame"**: This error occurs when the application is unable to retrieve frames from the video capture device. This could be due to the device being busy or not connected properly. Make sure that the device is connected and try restarting the application.
3. **"Memory allocation error"**: This error occurs when the application is unable to allocate memory for image processing. This could be due to insufficient memory on the system. Try closing other applications to free up memory.

If you encounter any other errors or issues, try searching for solutions online or consulting the documentation for the specific library or application you are using.

## Resources

- [OpenCV documentation](https://docs.opencv.org/master/)
- [OpenCV tutorials](https://docs.opencv.org/master/d9/df8/tutorial_root.html)
- [VideoCapture documentation](https://docs.opencv.org/master/d8/dfe/classcv_1_1VideoCapture.html)

