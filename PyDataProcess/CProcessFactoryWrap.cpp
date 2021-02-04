#include "CProcessFactoryWrap.h"

ProtocolTaskPtr CProcessFactoryWrap::create()
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("create")();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

ProtocolTaskPtr CProcessFactoryWrap::create(const ProtocolTaskParamPtr &pParam)
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("create")(pParam);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
