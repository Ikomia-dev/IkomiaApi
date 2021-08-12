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

#ifndef COCVWIDGETEDGEPRESERVINGFILTER_HPP
#define COCVWIDGETEDGEPRESERVINGFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/photo/COcvEdgePreservingFilter.hpp"

class COcvWidgetEdgePreservingFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetEdgePreservingFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetEdgePreservingFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvEdgePreservingFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvEdgePreservingFilterParam>();

            auto pLabelFilterType = new QLabel(tr("Filter type"));
            m_pComboFilterType = new QComboBox;
            m_pComboFilterType->addItem("Recursive", cv::RECURS_FILTER);
            m_pComboFilterType->addItem("Normalized convolution", cv::NORMCONV_FILTER);
            m_pComboFilterType->setCurrentIndex(m_pComboFilterType->findData(m_pParam->m_flag));

            auto pLabelSigmaS = new QLabel(tr("Sigma S"));
            m_pSpinSigmaS = new QDoubleSpinBox;
            m_pSpinSigmaS->setSingleStep(1.0);
            m_pSpinSigmaS->setRange(0.0, 200.0);
            m_pSpinSigmaS->setValue(m_pParam->m_sigmaS);

            auto pLabelSigmaR = new QLabel(tr("Sigma R"));
            m_pSpinSigmaR = new QDoubleSpinBox;
            m_pSpinSigmaR->setSingleStep(0.1);
            m_pSpinSigmaR->setRange(0.0, 1.0);
            m_pSpinSigmaR->setValue(m_pParam->m_sigmaR);
            
            m_pLayout->addWidget(pLabelFilterType, 0, 0);
            m_pLayout->addWidget(m_pComboFilterType, 0, 1);
            m_pLayout->addWidget(pLabelSigmaS, 1, 0);
            m_pLayout->addWidget(m_pSpinSigmaS, 1, 1);
            m_pLayout->addWidget(pLabelSigmaR, 2, 0);
            m_pLayout->addWidget(m_pSpinSigmaR, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_flag = m_pComboFilterType->currentData().toInt();
            m_pParam->m_sigmaS = m_pSpinSigmaS->value();
            m_pParam->m_sigmaR = m_pSpinSigmaR->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvEdgePreservingFilterParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaS = nullptr;
        QDoubleSpinBox* m_pSpinSigmaR = nullptr;
        QComboBox*      m_pComboFilterType = nullptr;
};

class COcvWidgetEdgePreservingFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetEdgePreservingFilterFactory()
        {
            m_name = QObject::tr("Edge Preserving Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetEdgePreservingFilter>(pParam);
        }
};

#endif // COCVWIDGETEDGEPRESERVINGFILTER_HPP
