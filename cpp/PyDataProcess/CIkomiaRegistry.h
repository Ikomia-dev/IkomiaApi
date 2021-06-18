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
        CTaskInfo                   getAlgorithmInfo(const std::string& name) const;

        WorkflowTaskPtr             createInstance(const std::string& processName);
        WorkflowTaskPtr             createInstance(const std::string& processName, const WorkflowTaskParamPtr& paramPtr);

        void                        registerTask(const TaskFactoryPtr& factoryPtr);

        void                        loadCppPlugins();
        void                        loadCppPlugin(const std::string &directory);

    private:

        void                        _loadCppPlugin(const QString &fileName);

    private:

        CProcessRegistration            m_processRegistrator;
        std::string                     m_pluginsDir;
        QMap<QString, QPluginLoader*>   m_loaders;
};

#endif // CIKOMIAREGISTRY_H
