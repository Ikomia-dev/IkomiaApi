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

#ifndef COCVWIDGETDETAILENHANCE_HPP
#define COCVWIDGETDETAILENHANCE_HPP

#include "Process/OpenCV/photo/COcvDetailEnhance.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetDetailEnhance : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDetailEnhance(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDetailEnhance(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDetailEnhanceParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDetailEnhanceParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma range"));
            m_pSpinSigmaS = new QDoubleSpinBox;
            m_pSpinSigmaR = new QDoubleSpinBox;

            m_pSpinSigmaS->setSingleStep(1);
            m_pSpinSigmaR->setSingleStep(0.1);

            m_pSpinSigmaS->setRange(0, 200);
            m_pSpinSigmaR->setRange(0, 1);

            m_pSpinSigmaS->setValue(m_pParam->m_sigma_s);
            m_pSpinSigmaR->setValue(m_pParam->m_sigma_r);
            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(m_pSpinSigmaS, 0, 1);
            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(m_pSpinSigmaR, 1, 1);
        }

        void onApply() override
        {
            m_pParam->m_sigma_s = m_pSpinSigmaS->value();
            m_pParam->m_sigma_r = m_pSpinSigmaR->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDetailEnhanceParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaS = nullptr;
        QDoubleSpinBox* m_pSpinSigmaR = nullptr;
};

class COcvWidgetDetailEnhanceFactory : public CWidgetFactory
{
    public:

        COcvWidgetDetailEnhanceFactory()
        {
            m_name = QObject::tr("Detail Enhance Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDetailEnhance>(pParam);
        }
};

#endif // COCVWIDGETDETAILENHANCE_HPP
