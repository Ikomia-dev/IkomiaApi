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

#include "CWidgetRgbHlsThreshold.h"

//----------------------------------//
//----- CWidgetRgbHlsThreshold -----//
//----------------------------------//
CWidgetRgbHlsThreshold::CWidgetRgbHlsThreshold(QWidget *parent) : CProtocolTaskWidget(parent)
{
    init();
}

CWidgetRgbHlsThreshold::CWidgetRgbHlsThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent) : CProtocolTaskWidget(parent)
{
    m_pParam = std::dynamic_pointer_cast<CRgbHlsThresholdParam>(pParam);
    init();
}

void CWidgetRgbHlsThreshold::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<CRgbHlsThresholdParam>();

    m_pClearBtn = new QPushButton(tr("Clear segmentation"));
    m_pClearBtn->setDefault(false);
    m_pApplyBtn->setFocus();

    connect(m_pApplyBtn, &QPushButton::clicked, [&]{ emit doApplyProcess(m_pParam); } );
    connect(m_pClearBtn, &QPushButton::clicked, [&]{ emit doSendProcessAction(CRgbHlsThresholdParam::ActionFlags::CLEAR_SEGMENTATION); });

    
    m_pLayout->addWidget(m_pClearBtn, 0, 0);
    
}

//-----------------------------------------//
//----- CWidgetRgbHlsThresholdFactory -----//
//-----------------------------------------//
CWidgetRgbHlsThresholdFactory::CWidgetRgbHlsThresholdFactory()
{
    m_name = QObject::tr("RGB-HLS Segmentation").toStdString();
}

ProtocolTaskWidgetPtr CWidgetRgbHlsThresholdFactory::create(std::shared_ptr<CProtocolTaskParam> pParam)
{
    return std::make_shared<CWidgetRgbHlsThreshold>(pParam);
}
