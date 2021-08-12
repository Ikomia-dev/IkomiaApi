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

class COcvWidgetBckgndSubMog2 : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubMog2(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubMog2(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubMog2Param>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubMog2Param>();

            auto pLabelHistory = new QLabel(tr("History"));
            m_pSpinHistory = new QSpinBox;
            m_pSpinHistory->setSingleStep(1);
            m_pSpinHistory->setRange(1, 10000);
            m_pSpinHistory->setValue(m_pParam->m_history);

            auto pLabelDistThresh = new QLabel(tr("Variance threshold"));
            m_pSpinVarThresh = new QDoubleSpinBox;
            m_pSpinVarThresh->setSingleStep(1.0);
            m_pSpinVarThresh->setRange(1, 255);
            m_pSpinVarThresh->setValue(m_pParam->m_varThreshold);

            m_pCheckDetectShadow = new QCheckBox(tr("Detect shadow"));
            m_pCheckDetectShadow->setChecked(m_pParam->m_bDetectShadow);

            m_pLayout->addWidget(pLabelHistory, 0, 0);
            m_pLayout->addWidget(m_pSpinHistory, 0, 1);
            m_pLayout->addWidget(pLabelDistThresh, 1, 0);
            m_pLayout->addWidget(m_pSpinVarThresh, 1, 1);
            m_pLayout->addWidget(m_pCheckDetectShadow, 2, 0, 1, 2);
        }

        void onApply() override
        {
            m_pParam->m_history = m_pSpinHistory->value();
            m_pParam->m_varThreshold = m_pSpinVarThresh->value();
            m_pParam->m_bDetectShadow = m_pCheckDetectShadow->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBckgndSubMog2Param> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinVarThresh = nullptr;
        QSpinBox*       m_pSpinHistory = nullptr;
        QCheckBox*      m_pCheckDetectShadow = nullptr;
};

class COcvWidgetBckgndSubMog2Factory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubMog2Factory()
        {
            m_name = QObject::tr("MOG2 background substractor").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubMog2>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBMOG2_HPP
