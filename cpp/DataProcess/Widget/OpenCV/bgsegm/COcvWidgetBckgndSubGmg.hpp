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

#ifndef COCVWIDGETBCKGNDSUBGMG_HPP
#define COCVWIDGETBCKGNDSUBGMG_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubGmg.hpp"

class COcvWidgetBckgndSubGmg : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubGmg(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubGmg(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubGmgParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubGmgParam>();

            auto pLabelInitFrames = new QLabel(tr("Number of initialization frames"));
            auto pSpinInitFrames = new QSpinBox;
            pSpinInitFrames->setSingleStep(1);
            pSpinInitFrames->setRange(1, 10000);
            pSpinInitFrames->setValue(m_pParam->m_initializationFrames);

            auto pLabelThreshold = new QLabel(tr("Decision threshold"));
            auto pSpinThreshold = new QDoubleSpinBox;
            pSpinThreshold->setSingleStep(0.1);
            pSpinThreshold->setRange(0.0, 1.0);
            pSpinThreshold->setValue(m_pParam->m_threshold);

            
            m_pLayout->addWidget(pLabelInitFrames, 0, 0);
            m_pLayout->addWidget(pSpinInitFrames, 0, 1);
            m_pLayout->addWidget(pLabelThreshold, 1, 0);
            m_pLayout->addWidget(pSpinThreshold, 1, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_initializationFrames = pSpinInitFrames->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubGmgParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubGmgFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubGmgFactory()
        {
            m_name = QObject::tr("GMG background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubGmg>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBGMG_HPP
