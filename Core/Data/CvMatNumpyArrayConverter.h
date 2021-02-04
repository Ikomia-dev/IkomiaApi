#include <opencv2/core/core.hpp>
#include <boost/python.hpp>
#include "Main/CoreGlobal.hpp"
#include "Data/CMat.hpp"
#include "PythonThread.hpp"

//-----------------------------------------------//
//----- Numpy NdArray <-> cv::Mat converter -----//
//-----------------------------------------------//
struct CvMatNumpyArrayConverter
{
    static cv::Mat		toMat(PyObject* pyObj);
    static PyObject*	toNDArray(const cv::Mat& cvMat);
};

//----------------------------//
//----- Boost converters -----//
//----------------------------//
struct BoostCvMatToNumpyArrayConverter
{
    static PyObject* convert(const CMat &m);
};

struct BoostNumpyArrayToCvMatConverter
{
    BoostNumpyArrayToCvMatConverter();

    /// @brief Check if PyObject is an array and can be converted to OpenCV matrix.
    static void* convertible(PyObject* object);

    /// @brief Construct a Mat from an NDArray object.
    static void  construct(PyObject* object, boost::python::converter::rvalue_from_python_stage1_data* data);
};
