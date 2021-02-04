#include "CDnnTrainProcessParamWrap.h"

CDnnTrainProcessParamWrap::CDnnTrainProcessParamWrap() : CDnnTrainProcessParam()
{
}

CDnnTrainProcessParamWrap::CDnnTrainProcessParamWrap(const CDnnTrainProcessParam &param)
    : CDnnTrainProcessParam(param)
{
}

void CDnnTrainProcessParamWrap::setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setParamOver = this->get_override("setParamMap"))
            setParamOver(paramMap);
        else
            CDnnTrainProcessParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CDnnTrainProcessParamWrap::default_setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        this->CDnnTrainProcessParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CDnnTrainProcessParamWrap::getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getParamOver = this->get_override("getParamMap"))
            return getParamOver();
        return CDnnTrainProcessParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CDnnTrainProcessParamWrap::default_getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDnnTrainProcessParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CDnnTrainProcessParamWrap::getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getHashValueOver = this->get_override("getHashValue"))
            return getHashValueOver();
        return CDnnTrainProcessParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CDnnTrainProcessParamWrap::default_getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CDnnTrainProcessParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
