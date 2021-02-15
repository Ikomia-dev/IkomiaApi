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

#ifndef COCVWIDGETBCKGNDSUBMOG2_HPP
#define COCVWIDGETBCKGNDSUBMOG2_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvBckgndSubMog2.hpp"

class COcvWidgetBckgndSubMog2 : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubMog2(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubMog2(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubMog2Param>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubMog2Param>();

            auto pLabelHistory = new QLabel(tr("History"));
            auto pSpinHistory = new QSpinBox;
            pSpinHistory->setSingleStep(1);
            pSpinHistory->setRange(1, 10000);
            pSpinHistory->setValue(m_pParam->m_history);

            auto pLabelDistThresh = new QLabel(tr("Variance threshold"));
            auto pSpinVarThresh = new QDoubleSpinBox;
            pSpinVarThresh->setSingleStep(1.0);
            pSpinVarThresh->setRange(1, 255);
            pSpinVarThresh->setValue(m_pParam->m_varThreshold);

            auto pCheckDetectShadow = new QCheckBox(tr("Detect shadow"));
            pCheckDetectShadow->setChecked(m_pParam->m_bDetectShadow);

            
            m_pLayout->addWidget(pLabelHistory, 0, 0);
            m_pLayout->addWidget(pSpinHistory, 0, 1);
            m_pLayout->addWidget(pLabelDistThresh, 1, 0);
            m_pLayout->addWidget(pSpinVarThresh, 1, 1);
            m_pLayout->addWidget(pCheckDetectShadow, 2, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_history = pSpinHistory->value();
                m_pParam->m_varThreshold = pSpinVarThresh->value();
                m_pParam->m_bDetectShadow = pCheckDetectShadow->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubMog2Param> m_pParam = nullptr;
};

class COcvWidgetBckgndSubMog2Factory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubMog2Factory()
        {
            m_name = QObject::tr("MOG2 background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubMog2>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBMOG2_HPP
