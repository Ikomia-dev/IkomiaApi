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

intptr_t CWorkflowWrap::getRootID()
{
    return reinterpret_cast<std::intptr_t>(getRootId());
}

std::vector<std::intptr_t> CWorkflowWrap::getTaskIDs()
{
    std::vector<std::intptr_t> nodes;
    auto vertices = getVertices();

    for(auto it=vertices.first; it!=vertices.second; ++it)
        nodes.push_back(reinterpret_cast<std::intptr_t>(*it));

    return nodes;
}

WorkflowTaskPtr CWorkflowWrap::getTask(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    return CWorkflow::getTask(vertex);
}

double CWorkflowWrap::getElapsedTimeTo(intptr_t id)
{
    WorkflowVertex vertex = reinterpret_cast<WorkflowVertex>(id);
    return CWorkflow::getElapsedTimeTo(vertex);
}

intptr_t CWorkflowWrap::addTaskWrap(const WorkflowTaskPtr &taskPtr)
{
    auto vertex = addTask(taskPtr);
    return reinterpret_cast<std::intptr_t>(vertex);
}

void CWorkflowWrap::connectWrap(const std::intptr_t &src, const std::intptr_t &target, int srcIndex, int targetIndex)
{
    auto srcVertex = reinterpret_cast<WorkflowVertex>(src);
    auto targetVertex = reinterpret_cast<WorkflowVertex>(target);

    if(srcIndex == -1 && targetIndex == -1)
    {
        // Auto connection
        connect(srcVertex, targetVertex);
    }
    else
    {
        connect(srcVertex, srcIndex, targetVertex, targetIndex);
    }
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
