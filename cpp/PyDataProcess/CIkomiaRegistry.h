#ifndef CIKOMIAREGISTRY_H
#define CIKOMIAREGISTRY_H

#include "CProcessRegistration.h"

class CIkomiaRegistry
{
    public:

        CIkomiaRegistry();

        ~CIkomiaRegistry();

        void                        setPluginsDirectory(const std::string& dir);

        std::vector<std::string>    getAlgorithms() const;
        std::string                 getPluginsDirectory() const;

        WorkflowTaskPtr             createInstance(const std::string& processName);
        WorkflowTaskPtr             createInstance(const std::string& processName, const WorkflowTaskParamPtr& paramPtr);

        void                        loadCppPlugins();

    private:

        void                        loadCppPlugin(const QString& fileName);

    private:

        CProcessRegistration            m_processRegistrator;
        std::string                     m_pluginsDir;
        QMap<QString, QPluginLoader*>   m_loaders;
};

#endif // CIKOMIAREGISTRY_H
