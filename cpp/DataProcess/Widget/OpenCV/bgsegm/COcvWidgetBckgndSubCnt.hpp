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

#ifndef COCVWIDGETBCKGNDSUBCNT_HPP
#define COCVWIDGETBCKGNDSUBCNT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubCnt.hpp"

class COcvWidgetBckgndSubCnt : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubCnt(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubCnt(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubCntParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubCntParam>();

            auto pLabelMinPxStability = new QLabel(tr("Minimum pixel stability"));
            m_pSpinMinPxStability = new QSpinBox;
            m_pSpinMinPxStability->setSingleStep(1);
            m_pSpinMinPxStability->setRange(1, 1000);
            m_pSpinMinPxStability->setValue(m_pParam->m_minPixelStability);

            auto pLabelMaxPxStability = new QLabel(tr("Minimum pixel stability"));
            m_pSpinMaxPxStability = new QSpinBox;
            m_pSpinMaxPxStability->setSingleStep(1);
            m_pSpinMaxPxStability->setRange(1, 10000);
            m_pSpinMaxPxStability->setValue(m_pParam->m_maxPixelStability);

            m_pCheckHistory = new QCheckBox(tr("Use history"));
            m_pCheckHistory->setChecked(m_pParam->m_bUseHistory);

            m_pCheckParallel = new QCheckBox(tr("Parallel computing"));
            m_pCheckParallel->setChecked(m_pParam->m_bIsParallel);

            m_pLayout->addWidget(pLabelMinPxStability, 0, 0);
            m_pLayout->addWidget(m_pSpinMinPxStability, 0, 1);
            m_pLayout->addWidget(pLabelMaxPxStability, 1, 0);
            m_pLayout->addWidget(m_pSpinMaxPxStability, 1, 1);
            m_pLayout->addWidget(m_pCheckHistory, 2, 0, 1, 2);
            m_pLayout->addWidget(m_pCheckParallel, 3, 0, 1, 2);
        }

        void onApply() override
        {
            m_pParam->m_minPixelStability = m_pSpinMinPxStability->value();
            m_pParam->m_maxPixelStability = m_pSpinMaxPxStability->value();
            m_pParam->m_bUseHistory = m_pCheckHistory->isChecked();
            m_pParam->m_bIsParallel = m_pCheckParallel->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBckgndSubCntParam> m_pParam = nullptr;
        QCheckBox*  m_pCheckHistory = nullptr;
        QCheckBox*  m_pCheckParallel = nullptr;
        QSpinBox*   m_pSpinMinPxStability = nullptr;
        QSpinBox*   m_pSpinMaxPxStability = nullptr;
};

class COcvWidgetBckgndSubCntFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubCntFactory()
        {
            m_name = "ocv_bck_substractor_cnt";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubCnt>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBCNT_HPP
