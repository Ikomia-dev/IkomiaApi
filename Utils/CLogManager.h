#ifndef CLOGMANAGER_H
#define CLOGMANAGER_H

#include <QObject>
#include <QFile>
#include <QFileSystemWatcher>
#include <QtMsgHandler>
#include <functional>
#include <QLoggingCategory>
#include <mutex>
#include "UtilsGlobal.hpp"


/// @cond INTERNAL

/**
 * @brief
 *
 */
class UTILSSHARED_EXPORT CLogManager: public QObject
{
    Q_OBJECT

    public:

        /**
         * @brief
         *
         * @param
         */
        CLogManager(const CLogManager&) = delete;
        /**
         * @brief
         *
         * @param
         */
        void operator=(const CLogManager&) = delete;

        /**
         * @brief
         *
         * @return CLogManager
         */
        static CLogManager& instance();
        /**
         * @brief
         *
         * @param type
         * @param context
         * @param msg
         */
        void                handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
        /**
         * @brief
         *
         * @param (int
         * @param outputFunc
         */
        void                addOutputManager(std::function<void (int, const QString&, const QString&)> outputFunc);
        /**
         * @brief
         *
         */
        void                clearOutputManagers();

        void                setStdRedirection(const QString& path);

    private:

        /**
         * @brief
         *
         */
        CLogManager();

    private slots:

        void                onFileChanged(const QString& filePath);

    private:

        using LogOutputFunc = std::function<void(int, const QString&, const QString&)>;
        std::vector<LogOutputFunc>  m_outputFunctions;
        std::mutex                  m_mutex;
        QFileSystemWatcher*         m_pFileWatcher = nullptr;
        QFile                       m_stdout_err;
        QTextStream                 m_stdStream;
};

/// @endcond

#endif // CLOGMANAGER_H
