#include "CLogManager.h"
#include <iostream>

#ifdef Q_OS_WIN64
#include <io.h>
#else
#include <unistd.h>
#endif

CLogManager::CLogManager()
{
    m_pFileWatcher = new QFileSystemWatcher(this);
    connect(m_pFileWatcher, &QFileSystemWatcher::fileChanged, this, &CLogManager::onFileChanged);
}

CLogManager& CLogManager::instance()
{
    static CLogManager instance;
    return instance;
}

void CLogManager::handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString fullMsg;

    //Do not display special character
    if(msg == "\n" || msg == "\t")
        return;

    std::lock_guard<std::mutex> lock(m_mutex);
    switch(type)
    {
        case QtDebugMsg:
            fullMsg = "Debug:";
            break;

        case QtInfoMsg:
            fullMsg = "Info:";
            break;

        case QtWarningMsg:
            fullMsg = "Warning:";
            break;

        case QtCriticalMsg:            
            fullMsg = "Critical:";
            break;

        case QtFatalMsg:            
            fullMsg = "Fatal:";
            break;

        default:            
            fullMsg = "Default:";
            break;
    }
    fullMsg += QString(context.category) + ": " + msg;
    if(m_outputFunctions.size() > 0)
    {
        for(size_t i=0; i<m_outputFunctions.size(); ++i)
            if(m_outputFunctions[i] != nullptr)
                m_outputFunctions[i](type, fullMsg, context.category);
    }
    else
        std::cout << fullMsg.toStdString() << std::endl;
}

void CLogManager::clearOutputManagers()
{
    m_outputFunctions.clear();
}

void CLogManager::setStdRedirection(const QString &path)
{
    freopen(path.toStdString().c_str(), "w", stdout);

#ifdef Q_OS_WIN64
    _dup2(fileno(stdout), fileno(stderr));
#else
    dup2(fileno(stdout), fileno(stderr));
#endif

    m_stdout_err.setFileName(path);
    m_stdout_err.open(QFile::ReadOnly | QFile::Text);
    m_stdStream.setDevice(&m_stdout_err);
    m_pFileWatcher->addPath(path);
}

void CLogManager::onFileChanged(const QString &filePath)
{
    if(m_outputFunctions.size() > 0)
    {
        if(filePath == m_stdout_err.fileName())
        {
            QString str;
            fflush(stdout);

            while(!m_stdStream.atEnd())
                str += m_stdStream.readLine() + "\n";

            if(!str.isEmpty())
            {
                QMessageLogContext context;
                context.category = tr("Default").toStdString().c_str();
                handleMessage(QtInfoMsg, context, str);
            }
        }
    }
}

void CLogManager::addOutputManager(std::function<void(int, const QString&, const QString&)> outputFunc)
{
    m_outputFunctions.push_back(outputFunc);
}
