#include "CProtocolTaskParamWrap.h"

CProtocolTaskParamWrap::CProtocolTaskParamWrap() : CProtocolTaskParam()
{
}

CProtocolTaskParamWrap::CProtocolTaskParamWrap(const CProtocolTaskParam &param)
    : CProtocolTaskParam(param)
{
}

void CProtocolTaskParamWrap::setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        if(override setParamOver = this->get_override("setParamMap"))
            setParamOver(paramMap);
        else
            CProtocolTaskParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CProtocolTaskParamWrap::default_setParamMap(const UMapString &paramMap)
{
    CPyEnsureGIL gil;
    try
    {
        this->CProtocolTaskParam::setParamMap(paramMap);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CProtocolTaskParamWrap::getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getParamOver = this->get_override("getParamMap"))
            return getParamOver();
        return CProtocolTaskParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

UMapString CProtocolTaskParamWrap::default_getParamMap() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskParam::getParamMap();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CProtocolTaskParamWrap::getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override getHashValueOver = this->get_override("getHashValue"))
            return getHashValueOver();
        return CProtocolTaskParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

uint CProtocolTaskParamWrap::default_getHashValue() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CProtocolTaskParam::getHashValue();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
