#include "PyDataIO.h"
#include "Data/CvMatNumpyArrayConverter.h"
#include "COpencvImageIO.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL IKOMIA_ARRAY_API
#include <numpy/ndarrayobject.h>

//Numpy initialization
static bool init_numpy()
{
    import_array();

    if(PyArray_API == NULL)
        return false;
    else
        return true;
}

BOOST_PYTHON_MODULE(pydataio)
{
    //Numpy initialization
    init_numpy();

    //CMat <-> Numpy NdArray converters
    to_python_converter<CMat, BoostCvMatToNumpyArrayConverter>();
    BoostNumpyArrayToCvMatConverter();

    //--------------------------//
    //----- COpencvImageIO -----//
    //--------------------------//
    //Overloaded member functions
    CMat (COpencvImageIO::*read1)() = &COpencvImageIO::read;

    class_<COpencvImageIO>("COpencvImageIO", init<const std::string&>())
        .def("read", read1)
    ;
}
