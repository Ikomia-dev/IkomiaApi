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

#ifndef CDATAVIDEOBUFFER_HPP
#define CDATAVIDEOBUFFER_HPP

#include <QObject>
#include <QtConcurrent>
#include <thread>
#include <mutex>
#include "DataIOGlobal.hpp"
#include "Containers/CQueue.hpp"
#include "Data/CMat.hpp"


/** @cond INTERNAL */

class DATAIOSHARED_EXPORT CDataVideoBuffer: public QObject
{
    Q_OBJECT

    public:

        enum Type { VIDEO, OPENNI_STREAM, ID_STREAM, IP_STREAM, PATH_STREAM, IMAGE_SEQUENCE, GSTREAMER_PIPE };

        CDataVideoBuffer();
        CDataVideoBuffer(const std::string& path);
        ~CDataVideoBuffer();

        void            closeCamera();
        void            openVideo();

        void            startRead();
        void            stopRead();
        void            pauseRead();
        void            clearRead();

        void            startWrite(int width, int height, int nbFrames, int fps=25, int fourcc=-1);
        void            startStreamWrite();
        void            stopWrite();

        CMat            read();
        void            write(CMat image);
        CMat            snapshot(int pos = -1);

        void            waitWriteFinished();

        bool            hasReadImage() const;
        bool            isReadStarted() const;
        bool            isReadOpened() const;
        bool            isReadMode() const;

        bool            hasWriteImage() const;
        bool            isWriteStarted() const;

        void            setVideoPath(const std::string& path);
        void            setQueueSize(size_t queueSize);
        void            setPosition(int pos);
        void            setFPS(int fps);
        void            setSize(int width, int height);
        void            setFrameCount(int nb);
        void            setMode(int mode);
        void            setFourCC(int code);
        void            setMaxFailureCount(int nb);

        std::string     getCurrentPath() const;
        std::string     getSourceName() const;
        int             getFrameCount() const;
        int             getCurrentPos() const;
        int             getFPS() const;
        int             getWidth() const;
        int             getHeight() const;
        int             getCodec() const;
        Type            getSourceType() const;

        void            openStreamFromId(int id);
        void            openStreamFromIP(const std::string& ipAdress);
        void            openStreamFromPath(const std::string& path);

    private:

        void            init();

        void            updateRead();
        void            updateWrite();
        void            updateStreamWrite();

        bool            isStreamSource() const;
        bool            isNumber(const std::string& s) const;
        void            isWritable();

    private:

        CQueue<CMat>            m_queueRead;
        CQueue<CMat>            m_queueWrite;
        cv::VideoCapture        m_reader;
        cv::VideoWriter         m_writer;
        std::string             m_path;
        size_t                  m_queueSize = 128;
        std::mutex              m_mutex;
        QFutureWatcher<void>    m_watcherRead;
        QFutureWatcher<void>    m_watcherWrite;
        std::future<void>       m_writeFuture;
        std::atomic_bool        m_bStopRead{true};
        std::atomic_bool        m_bStopWrite{true};
        Type                    m_type = VIDEO;
        int                     m_fps = 0;
        int                     m_width = 0;
        int                     m_height = 0;
        int                     m_nbFrames = 0;
        int                     m_fourcc = -1;
        int                     m_currentPos = 0;
        int                     m_mode = 0;
        int                     m_writeAPI = cv::CAP_FFMPEG;
        int                     m_maxFailureCount = 10;
};

using CDataVideoBufferPtr = std::unique_ptr<CDataVideoBuffer>;

/** @endcond */

#endif // CDATAVIDEOBUFFER_HPP
