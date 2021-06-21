#include "CWorkflowWrap.h"

CWorkflowWrap::CWorkflowWrap() : CWorkflow()
{
}

CWorkflowWrap::CWorkflowWrap(const std::string &name) : CWorkflow(name)
{
}

CWorkflowWrap::CWorkflowWrap(const std::string& name, const std::shared_ptr<CIkomiaRegistry> &registryPtr)
    : CWorkflow(name, registryPtr->getTaskRegistrator(), registryPtr->getIORegistrator(), nullptr)
{
}

CWorkflowWrap::CWorkflowWrap(const CWorkflow &workflow): CWorkflow(workflow)
{
}

void CWorkflowWrap::run()
{
    CPyEnsureGIL gil;
    try
    {
        if(override runOver = this->get_override("run"))
            runOver();
        else
            CWorkflow::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}

void CWorkflowWrap::default_run()
{
    CPyEnsureGIL gil;
    try
    {
        this->CWorkflow::run();
    }
    catch(boost::python::error_already_set&)
    {
        throw CException(CoreExCode::PYTHON_EXCEPTION, Utils::Python::handlePythonException(), __func__, __FILE__, __LINE__);
    }
}
