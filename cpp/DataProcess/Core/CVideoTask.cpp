// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "CVideoTask.h"

CVideoTask::CVideoTask() : C2dImageTask()
{
    m_type = CWorkflowTask::Type::VIDEO;
}

CVideoTask::CVideoTask(const std::string& name) : C2dImageTask(name)
{
    m_type = CWorkflowTask::Type::VIDEO;
}

void CVideoTask::beginTaskRun()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_bRunning = true;

    //Notify current task name
    emit m_signalHandler->doSetMessage(QString::fromStdString(m_name));

    //Check inputs validity
    if(getInputCount() == 0)
        throw CException(CoreExCode::INVALID_PARAMETER, "No input", __func__, __FILE__, __LINE__);

    //Start timing
    m_timer.start();

    // Clear graphics as in C2dImageTask::beginTaskRun
    m_graphicsMasks.clear();
}

void CVideoTask::notifyVideoStart(int frameCount)
{
    Q_UNUSED(frameCount);
}

void CVideoTask::notifyVideoEnd()
{
}
