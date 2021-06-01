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

#ifndef CVIDEOPROCESS_H
#define CVIDEOPROCESS_H

#include "CImageProcess2d.h"
#include "IO/CVideoProcessIO.h"
#include "DataProcessGlobal.hpp"

/**
 * @ingroup groupDataProcess
 * @brief The CVideoProcess class is derived from CImageProcess2d and adds video specific features.
 * It should be the base class for all process task dedicated to video processing.
 */
class DATAPROCESSSHARED_EXPORT CVideoProcess : public CImageProcess2d
{
    public:

        /**
         * @brief Default constructor.
         * Same inputs and outputs as CImageProcess2d.
         */
        CVideoProcess();
        /**
         * @brief Constructs CVideoProcess with the given name.
         * Same inputs and outputs as CImageProcess2d.
         * @param name: name of the task.
         */
        CVideoProcess(const std::string& name);

        virtual ~CVideoProcess() = default;

        /**
         * @brief Performs checks before the process is started
         */
        virtual void    beginTaskRun() override;
        /**
         * @brief Notifies that the video has started.
         * @param frameCount: total number of frames.
         */
        virtual void    notifyVideoStart(int frameCount) override;
        /**
         * @brief Notifies that the end of the video is reached.
         */
        virtual void    notifyVideoEnd() override;
};

#endif // CVIDEOPROCESS_H
