#ifndef CIKOMIAREGISTRY_H
#define CIKOMIAREGISTRY_H

#include "CProcessRegistration.h"

class CIkomiaRegistry
{
    public:

        CIkomiaRegistry();

        std::vector<std::string>    getAlgorithms() const;

        WorkflowTaskPtr             createInstance(const std::string& processName);
        WorkflowTaskPtr             createInstance(const std::string& processName, const WorkflowTaskParamPtr& paramPtr);

    private:

        CProcessRegistration        m_processRegistrator;
};

#endif // CIKOMIAREGISTRY_H
