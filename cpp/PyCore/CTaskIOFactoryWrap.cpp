#include "CTaskIOFactoryWrap.h"

WorkflowTaskIOPtr CTaskIOFactoryWrap::create(IODataType dataType)
{
    CPyEnsureGIL gil;
    try
    {
        return this->get_override("create")(dataType);
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
