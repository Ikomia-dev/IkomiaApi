#include "CIkomiaRegistry.h"
#include "UtilsTools.hpp"
#include "CPluginProcessInterface.hpp"

CIkomiaRegistry::CIkomiaRegistry()
{
    m_pluginsDir = QDir::homePath().toStdString() + "/Ikomia/Plugins";
    loadCppPlugins();
}

CIkomiaRegistry::~CIkomiaRegistry()
{
    for(auto&& it: m_loaders)
        delete it;
}

void CIkomiaRegistry::setPluginsDirectory(const std::string &dir)
{
    m_pluginsDir = dir;
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

std::string CIkomiaRegistry::getPluginsDirectory() const
{
    return m_pluginsDir;
}

CTaskInfo CIkomiaRegistry::getAlgorithmInfo(const std::string &name) const
{
    return m_processRegistrator.getProcessInfo(name);
}

WorkflowTaskPtr CIkomiaRegistry::createInstance(const std::string &processName)
{
    return m_processRegistrator.createProcessObject(processName, nullptr);
}

WorkflowTaskPtr CIkomiaRegistry::createInstance(const std::string &processName, const WorkflowTaskParamPtr &paramPtr)
{
    return m_processRegistrator.createProcessObject(processName, paramPtr);
}

void CIkomiaRegistry::registerTask(const TaskFactoryPtr &factoryPtr)
{
    m_processRegistrator.registerProcess(factoryPtr, nullptr);
}

void CIkomiaRegistry::loadCppPlugins()
{
    QDir pluginsDir(QString::fromStdString(m_pluginsDir + "/C++"));

#ifdef Q_OS_WIN64
    //Add plugin root folder to the search path of the DLL loader
    SetDllDirectoryA(QDir::toNativeSeparators(m_cppPath).toStdString().c_str());
#endif

    //Load plugins placed directly in the root folder
    foreach (QString fileName, pluginsDir.entryList(QDir::Files|QDir::NoSymLinks))
        _loadCppPlugin(pluginsDir.absoluteFilePath(fileName));

    //Scan sub-directories
    foreach (QString directory, pluginsDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
    {
        auto dirPath = pluginsDir.absoluteFilePath(directory);
        QDir pluginDir(dirPath);

#ifdef Q_OS_WIN64
        //Add current plugin folder to the search path of the DLL loader
        SetDllDirectoryA(QDir::toNativeSeparators(dirPath).toStdString().c_str());
#endif

        foreach (QString fileName, pluginDir.entryList(QDir::Files|QDir::NoSymLinks))
            _loadCppPlugin(pluginDir.absoluteFilePath(fileName));
    }

#ifdef Q_OS_WIN64
    //Restore standard DLL search path
    SetDllDirectoryA(NULL);
#endif
}

void CIkomiaRegistry::loadCppPlugin(const std::string& directory)
{
    QDir pluginDir(QString::fromStdString(directory));
    foreach (QString fileName, pluginDir.entryList(QDir::Files|QDir::NoSymLinks))
        _loadCppPlugin(pluginDir.absoluteFilePath(fileName));
}

void CIkomiaRegistry::_loadCppPlugin(const QString &fileName)
{
    try
    {
        if(QLibrary::isLibrary(fileName))
        {
            QPluginLoader* pLoader;
            if(m_loaders.contains(fileName))
                pLoader = m_loaders[fileName];
            else
                pLoader = m_loaders.insert(fileName, new QPluginLoader(fileName)).value();

            // Get root component object of the plugin
            QObject* pObject = pLoader->instance();

            // Check if plugin is loaded or root component instantiated
            if(pObject == nullptr)
            {
                std::cerr << QString("Plugin %1 could not be loaded: %2.").arg(fileName).arg(pLoader->errorString()).toStdString();
                return;
            }

            // Check if plugin is a CPluginProcessInterface
            CPluginProcessInterface* pPlugin = qobject_cast<CPluginProcessInterface*>(pObject);
            if(pPlugin == nullptr)
            {
                std::cerr << QString("Plugin %1 interface is not valid.").arg(fileName).toStdString();
                return;
            }

            auto taskFactoryPtr = pPlugin->getProcessFactory();
            if(taskFactoryPtr == nullptr)
            {
                std::cerr << QString("Plugin %1 has no process factory.").arg(fileName).toStdString();
                return;
            }
            taskFactoryPtr->getInfo().setInternal(false);
            taskFactoryPtr->getInfo().setLanguage(ApiLanguage::CPP);
            taskFactoryPtr->getInfo().setOS(Utils::OS::getCurrent());

            auto version = QString::fromStdString(taskFactoryPtr->getInfo().getIkomiaVersion());
            auto state = Utils::Plugin::getCppState(version);

            if(state == PluginState::DEPRECATED)
            {
                QString str = QString("Plugin %1 is deprecated: based on Ikomia %2 while the current version is %3.")
                        .arg(QString::fromStdString(taskFactoryPtr->getInfo().getName()))
                        .arg(version)
                        .arg(Utils::IkomiaApp::getCurrentVersionNumber());
                std::cerr << str.toStdString();
                return;
            }
            else if(state == PluginState::UPDATED)
            {
                QString str = QString("Plugin %1 is not compatible: you must update Ikomia Studio to version %2.")
                        .arg(QString::fromStdString(taskFactoryPtr->getInfo().getName()))
                        .arg(version);
                std::cerr << str.toStdString();
                return;
            }

            auto widgetFactoryPtr = pPlugin->getWidgetFactory();
            if(widgetFactoryPtr == nullptr)
            {
                QString str = QString("Plugin %1 has no widget factory.").arg(QString::fromStdString(taskFactoryPtr->getInfo().getName()));
                std::cerr << str.toStdString();
                return;
            }

            m_processRegistrator.registerProcess(taskFactoryPtr, widgetFactoryPtr);
            std::cout << QString("Plugin %1 is loaded.").arg(fileName).toStdString();
        }
    }
    catch(std::exception& e)
    {
        std::cerr << QString("Plugin %1 failed to load:").arg(fileName).toStdString();
        std::cerr << e.what();
    }
}
