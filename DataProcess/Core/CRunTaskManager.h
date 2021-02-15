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

#ifndef CRUNTASKMANAGER_H
#define CRUNTASKMANAGER_H

#include "Protocol/CProtocolTask.h"

class CRunTaskManager
{
    public:

        CRunTaskManager();

        void    setBatchMode(bool bEnable);

        void    run(const ProtocolTaskPtr& pTask);

        void    stop(const ProtocolTaskPtr &taskPtr);

    private:

        void    runImageProcess2D(const ProtocolTaskPtr& taskPtr);
        void    runVideoProcess(const ProtocolTaskPtr& taskPtr);
        void    runWholeVideoProcess(const ProtocolTaskPtr& taskPtr);

    private:

        bool                m_bBatchMode = false;
        std::atomic_bool    m_bStop{false};
        size_t              m_outputVideoId = SIZE_MAX;
};

#endif // CRUNTASKMANAGER_H
