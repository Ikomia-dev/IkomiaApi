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

#include "CDnnTrainProcess.h"
#include <QDesktopServices>
#include "UtilsTools.hpp"

//---------------------------------//
//----- CDnnTrainProcessParam -----//
//---------------------------------//
CDnnTrainProcessParam::CDnnTrainProcessParam() : CProtocolTaskParam()
{
}

void CDnnTrainProcessParam::setParamMap(const UMapString &paramMap)
{
    for(auto it : paramMap)
        m_cfg[it.first] = it.second;
}

UMapString CDnnTrainProcessParam::getParamMap() const
{
    UMapString map;
    for(auto it : m_cfg)
        map[it.first] = it.second;

    return map;
}

uint CDnnTrainProcessParam::getHashValue() const
{
    return CProtocolTaskParam::getHashValue();
}

//----------------------------//
//----- CDnnTrainProcess -----//
//----------------------------//
CDnnTrainProcess::CDnnTrainProcess() : CProtocolTask()
{
    m_type = CProtocolTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

CDnnTrainProcess::CDnnTrainProcess(const std::string &name): CProtocolTask(name)
{
    m_type = CProtocolTask::Type::DNN_TRAIN;
    m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

CDnnTrainProcess::CDnnTrainProcess(const std::string &name, const std::shared_ptr<CDnnTrainProcessParam> &pParam) : CProtocolTask(name)
{
    m_type = CProtocolTask::Type::DNN_TRAIN;

    if(pParam)
        m_pParam = std::make_shared<CDnnTrainProcessParam>(*pParam);
    else
        m_pParam = std::make_shared<CDnnTrainProcessParam>();
}

std::string CDnnTrainProcess::getTensorboardLogDir() const
{
    return Utils::Tensorboard::getLogDirUri();
}

void CDnnTrainProcess::enableMlflow(bool bEnable)
{
    m_bOpenMlflow = bEnable;
}

void CDnnTrainProcess::enableTensorboard(bool bEnable)
{
    m_bOpenTensorboad = bEnable;
}

void CDnnTrainProcess::beginTaskRun()
{
    CProtocolTask::beginTaskRun();

    //Show MLflow server UI once
    if(m_bOpenMlflow)
    {
        QDesktopServices::openUrl(QUrl(QString::fromStdString(Utils::MLflow::getTrackingURI())));
        m_bOpenMlflow = false;
    }

    //Show Tensorbord server UI
    if(m_bOpenTensorboad)
    {
        QDesktopServices::openUrl(QUrl(QString::fromStdString(Utils::Tensorboard::getTrackingURI())));
        m_bOpenTensorboad = false;
    }
}

void CDnnTrainProcess::endTaskRun()
{
    CProtocolTask::endTaskRun();
}
