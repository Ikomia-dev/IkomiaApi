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

#ifndef COCVWIDGETBCKGNDSUBKNN_HPP
#define COCVWIDGETBCKGNDSUBKNN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvBckgndSubKnn.hpp"

class COcvWidgetBckgndSubKnn : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubKnn(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubKnn(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubKnnParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubKnnParam>();

            auto pLabelHistory = new QLabel(tr("History"));
            auto pSpinHistory = new QSpinBox;
            pSpinHistory->setSingleStep(1);
            pSpinHistory->setRange(1, 10000);
            pSpinHistory->setValue(m_pParam->m_history);

            auto pLabelDistThresh = new QLabel(tr("Distance threshold"));
            auto pSpinDistThresh = new QDoubleSpinBox;
            pSpinDistThresh->setSingleStep(1.0);
            pSpinDistThresh->setRange(1, 1000);
            pSpinDistThresh->setValue(m_pParam->m_distanceThreshold);

            auto pCheckDetectShadow = new QCheckBox(tr("Detect shadow"));
            pCheckDetectShadow->setChecked(m_pParam->m_bDetectShadow);

            
            m_pLayout->addWidget(pLabelHistory, 0, 0);
            m_pLayout->addWidget(pSpinHistory, 0, 1);
            m_pLayout->addWidget(pLabelDistThresh, 1, 0);
            m_pLayout->addWidget(pSpinDistThresh, 1, 1);
            m_pLayout->addWidget(pCheckDetectShadow, 2, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_history = pSpinHistory->value();
                m_pParam->m_distanceThreshold = pSpinDistThresh->value();
                m_pParam->m_bDetectShadow = pCheckDetectShadow->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubKnnParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubKnnFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubKnnFactory()
        {
            m_name = QObject::tr("KNN background substractor").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubKnn>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBKNN_HPP
