#include "CIkomiaRegistry.h"

CIkomiaRegistry::CIkomiaRegistry()
{   
}

std::vector<std::string> CIkomiaRegistry::getAlgorithms() const
{
    std::vector<std::string> names;
    auto factory = m_processRegistrator.getProcessFactory();
    auto taskFactories = factory.getList();

    for(auto&& factoryPtr : taskFactories)
        names.push_back(factoryPtr->getInfo().getName());

    return names;
}

WorkflowTaskPtr CIkomiaRegistry::createInstance(const std::string &processName)
{
    return m_processRegistrator.createProcessObject(processName, nullptr);
}

WorkflowTaskPtr CIkomiaRegistry::createInstance(const std::string &processName, const WorkflowTaskParamPtr &paramPtr)
{
    return m_processRegistrator.createProcessObject(processName, paramPtr);
}
