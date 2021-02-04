#include "CPluginProcessInterfaceWrap.h"

std::shared_ptr<CProcessFactory> CPluginProcessInterfaceWrap::getProcessFactory()
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("getProcessFactory")();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

std::shared_ptr<CWidgetFactory> CPluginProcessInterfaceWrap::getWidgetFactory()
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("getWidgetFactory")();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
