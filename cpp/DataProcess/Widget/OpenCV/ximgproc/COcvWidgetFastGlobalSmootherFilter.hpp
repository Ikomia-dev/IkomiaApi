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

#ifndef COCVWIDGETFASTGLOBALSMOOTHERFILTER_HPP
#define COCVWIDGETFASTGLOBALSMOOTHERFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvFastGlobalSmootherFilter.hpp"

class COcvWidgetFastGlobalSmootherFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFastGlobalSmootherFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetFastGlobalSmootherFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFastGlobalSmootherFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFastGlobalSmootherFilterParam>();

            auto pLabelLambda = new QLabel(QObject::tr("Lambda"));
            m_pSpinLambda = new QDoubleSpinBox;
            m_pSpinLambda->setValue(m_pParam->m_lambda);
            m_pSpinLambda->setRange(0.0, 100.0);
            m_pSpinLambda->setSingleStep(0.1);

            auto pLabelSigmaC = new QLabel(QObject::tr("Sigma color"));
            m_pSpinSigmaC = new QDoubleSpinBox;
            m_pSpinSigmaC->setValue(m_pParam->m_sigmaColor);
            m_pSpinSigmaC->setRange(0.0, 100.0);
            m_pSpinSigmaC->setSingleStep(0.1);

            auto pLabelLambdaAtt = new QLabel(QObject::tr("Lambda attenuation"));
            m_pSpinLambdaAtt = new QDoubleSpinBox;
            m_pSpinLambdaAtt->setValue(m_pParam->m_lambdaAttenuation);
            m_pSpinLambdaAtt->setRange(0.0, 1.0);
            m_pSpinLambdaAtt->setSingleStep(0.1);

            auto pLabelIteration = new QLabel(QObject::tr("Number of iteration"));
            m_pSpinIteration = new QSpinBox;
            m_pSpinIteration->setRange(1, 100);
            m_pSpinIteration->setValue(m_pParam->m_nbIter);
            m_pSpinIteration->setSingleStep(1);

            m_pLayout->addWidget(pLabelLambda, 0, 0);
            m_pLayout->addWidget(m_pSpinLambda, 0, 1);
            m_pLayout->addWidget(pLabelSigmaC, 1, 0);
            m_pLayout->addWidget(m_pSpinSigmaC, 1, 1);
            m_pLayout->addWidget(pLabelLambdaAtt, 2, 0);
            m_pLayout->addWidget(m_pSpinLambdaAtt, 2, 1);
            m_pLayout->addWidget(pLabelIteration, 3, 0);
            m_pLayout->addWidget(m_pSpinIteration, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_lambda = m_pSpinLambda->value();
            m_pParam->m_sigmaColor = m_pSpinSigmaC->value();
            m_pParam->m_lambdaAttenuation = m_pSpinLambdaAtt->value();
            m_pParam->m_nbIter = m_pSpinIteration->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvFastGlobalSmootherFilterParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinLambda = nullptr;
        QDoubleSpinBox* m_pSpinSigmaC = nullptr;
        QDoubleSpinBox* m_pSpinLambdaAtt = nullptr;
        QSpinBox*       m_pSpinIteration = nullptr;
};

class COcvWidgetFastGlobalSmootherFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastGlobalSmootherFilterFactory()
        {
            m_name = "ocv_fast_global_smooth_filter";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastGlobalSmootherFilter>(pParam);
        }
};

#endif // COCVWIDGETFASTGLOBALSMOOTHERFILTER_HPP
