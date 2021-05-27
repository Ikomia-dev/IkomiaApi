/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
