#include "CWidgetOutputWrap.h"

CWidgetOutputWrap::CWidgetOutputWrap() : CWidgetOutput()
{
}

CWidgetOutputWrap::CWidgetOutputWrap(IODataType dataType) : CWidgetOutput(dataType)
{
}

CWidgetOutputWrap::CWidgetOutputWrap(const CWidgetOutput &out) : CWidgetOutput(out)
{
}

void CWidgetOutputWrap::setWidget(long long widgetPtr)
{
    CPyEnsureGIL gil;
    try
    {
        auto pWidget = reinterpret_cast<QWidget*>(widgetPtr);
        this->CWidgetOutput::setWidget(pWidget);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWidgetOutputWrap::isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        if(override isDataOver = this->get_override("isDataAvailable"))
            return isDataOver();
        return CWidgetOutput::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

bool CWidgetOutputWrap::default_isDataAvailable() const
{
    CPyEnsureGIL gil;
    try
    {
        return this->CWidgetOutput::isDataAvailable();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWidgetOutputWrap::clearData()
{
    CPyEnsureGIL gil;
    try
    {
        if(override clearDataOver = this->get_override("clearData"))
            clearDataOver();
        else
            CWidgetOutput::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWidgetOutputWrap::default_clearData()
{
    CPyEnsureGIL gil;
    try
    {
        this->CWidgetOutput::clearData();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
