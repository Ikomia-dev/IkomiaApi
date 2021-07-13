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

#include "CDnnTrainTask.h"
#include <QDesktopServices>
#include "UtilsTools.hpp"

//----------------------------//
//----- CDnnTrainTask -----//
//----------------------------//
CDnnTrainTask::CDnnTrainTask() : CWorkflowTask()
{
    m_type = CWorkflowTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CWorkflowTaskParam>();
}

CDnnTrainTask::CDnnTrainTask(const std::string &name): CWorkflowTask(name)
{
    m_type = CWorkflowTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CWorkflowTaskParam>();
}

CDnnTrainTask::CDnnTrainTask(const std::string &name, const std::shared_ptr<CWorkflowTaskParam> &pParam) : CWorkflowTask(name)
{
    m_type = CWorkflowTask::Type::DNN_TRAIN;

    if(pParam)
        m_pParam = std::make_shared<CWorkflowTaskParam>(*pParam);
    else
        m_pParam = std::make_shared<CWorkflowTaskParam>();
}

std::string CDnnTrainTask::getTensorboardLogDir() const
{
    return Utils::Tensorboard::getLogDirUri();
}

void CDnnTrainTask::enableMlflow(bool bEnable)
{
    m_bOpenMlflow = bEnable;
}

void CDnnTrainTask::enableTensorboard(bool bEnable)
{
    m_bOpenTensorboad = bEnable;
}

void CDnnTrainTask::beginTaskRun()
{
    CWorkflowTask::beginTaskRun();

    //Show MLflow server UI once
    if(m_bOpenMlflow)
    {
        Utils::OS::openUrl(Utils::MLflow::getTrackingURI());
        m_bOpenMlflow = false;
    }

    //Show Tensorbord server UI
    if(m_bOpenTensorboad)
    {
        Utils::OS::openUrl(Utils::Tensorboard::getTrackingURI());
        m_bOpenTensorboad = false;
    }
}

void CDnnTrainTask::endTaskRun()
{
    CWorkflowTask::endTaskRun();
}
