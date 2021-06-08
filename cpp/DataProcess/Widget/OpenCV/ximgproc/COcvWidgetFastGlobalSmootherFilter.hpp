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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFastGlobalSmootherFilterParam>();

            auto pLabelLambda = new QLabel(QObject::tr("Lambda"));
            auto pSpinLambda = new QDoubleSpinBox;
            pSpinLambda->setValue(m_pParam->m_lambda);
            pSpinLambda->setRange(0.0, 100.0);
            pSpinLambda->setSingleStep(0.1);

            auto pLabelSigmaC = new QLabel(QObject::tr("Sigma color"));
            auto pSpinSigmaC = new QDoubleSpinBox;
            pSpinSigmaC->setValue(m_pParam->m_sigmaColor);
            pSpinSigmaC->setRange(0.0, 100.0);
            pSpinSigmaC->setSingleStep(0.1);

            auto pLabelLambdaAtt = new QLabel(QObject::tr("Lambda attenuation"));
            auto pSpinLambdaAtt = new QDoubleSpinBox;
            pSpinLambdaAtt->setValue(m_pParam->m_lambdaAttenuation);
            pSpinLambdaAtt->setRange(0.0, 1.0);
            pSpinLambdaAtt->setSingleStep(0.1);

            auto pLabelIteration = new QLabel(QObject::tr("Number of iteration"));
            auto pSpinIteration = new QSpinBox;
            pSpinIteration->setRange(1, 100);
            pSpinIteration->setValue(m_pParam->m_nbIter);
            pSpinIteration->setSingleStep(1);

            
            m_pLayout->addWidget(pLabelLambda, 0, 0);
            m_pLayout->addWidget(pSpinLambda, 0, 1);
            m_pLayout->addWidget(pLabelSigmaC, 1, 0);
            m_pLayout->addWidget(pSpinSigmaC, 1, 1);
            m_pLayout->addWidget(pLabelLambdaAtt, 2, 0);
            m_pLayout->addWidget(pSpinLambdaAtt, 2, 1);
            m_pLayout->addWidget(pLabelIteration, 3, 0);
            m_pLayout->addWidget(pSpinIteration, 3, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_lambda = pSpinLambda->value();
                m_pParam->m_sigmaColor = pSpinSigmaC->value();
                m_pParam->m_lambdaAttenuation = pSpinLambdaAtt->value();
                m_pParam->m_nbIter = pSpinIteration->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFastGlobalSmootherFilterParam> m_pParam = nullptr;
};

class COcvWidgetFastGlobalSmootherFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastGlobalSmootherFilterFactory()
        {
            m_name = QObject::tr("Fast Global Smoother Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastGlobalSmootherFilter>(pParam);
        }
};

#endif // COCVWIDGETFASTGLOBALSMOOTHERFILTER_HPP
