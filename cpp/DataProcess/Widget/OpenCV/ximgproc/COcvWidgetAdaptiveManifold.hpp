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

#ifndef COCVWIDGETADAPTIVEMANIFOLD_HPP
#define COCVWIDGETADAPTIVEMANIFOLD_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvAdaptiveManifold.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>

class COcvWidgetAdaptiveManifold : public CWorkflowTaskWidget
{
    public:

        COcvWidgetAdaptiveManifold(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAdaptiveManifold(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAdaptiveManifoldParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAdaptiveManifoldParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma Spatial"));
            auto pSpinSigmaS = new QDoubleSpinBox;
            auto pLabelSigmaR = new QLabel(tr("Sigma Color Space"));
            auto pSpinSigmaR = new QDoubleSpinBox;
            auto pLabelTreeHeight = new QLabel(tr("Tree Height"));
            auto pSpinTreeHeight = new QSpinBox;
            auto pLabelNumPCA = new QLabel(tr("Iterations"));
            auto pSpinNumPCA = new QSpinBox;
            auto pCheckOutliers = new QCheckBox(tr("Adjust Outliers"));
            auto pCheckRng = new QCheckBox(tr("Use RNG"));

            pSpinSigmaS->setSingleStep(0.1);
            pSpinSigmaR->setSingleStep(0.1);
            pSpinTreeHeight->setMinimum(-1);

            pSpinSigmaS->setValue(m_pParam->m_sigmaS);
            pSpinSigmaR->setValue(m_pParam->m_sigmaR);
            pSpinTreeHeight->setValue(m_pParam->m_treeHeight);
            pSpinNumPCA->setValue(m_pParam->m_numPcaIterations);
            pCheckOutliers->setChecked(m_pParam->m_bAdjustOutliers);
            pCheckRng->setChecked(m_pParam->m_bUseRNG);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_sigmaS = pSpinSigmaS->value();
                m_pParam->m_sigmaR = pSpinSigmaR->value();
                m_pParam->m_treeHeight = pSpinTreeHeight->value();
                m_pParam->m_numPcaIterations = pSpinNumPCA->value();
                m_pParam->m_bAdjustOutliers = pCheckOutliers->isChecked();
                m_pParam->m_bUseRNG = pCheckRng->isChecked();
                emit doApplyProcess(m_pParam);
            } );

            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(pSpinSigmaS, 0, 1);

            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(pSpinSigmaR, 1, 1);

            m_pLayout->addWidget(pLabelTreeHeight, 2, 0);
            m_pLayout->addWidget(pSpinTreeHeight, 2, 1);

            m_pLayout->addWidget(pLabelNumPCA, 3, 0);
            m_pLayout->addWidget(pSpinNumPCA, 3, 1);

            m_pLayout->addWidget(pCheckOutliers, 4, 0);
            m_pLayout->addWidget(pCheckRng, 5, 0);

            
        }

    private:

        std::shared_ptr<COcvAdaptiveManifoldParam> m_pParam = nullptr;
};

class COcvWidgetAdaptiveManifoldFactory : public CWidgetFactory
{
    public:

        COcvWidgetAdaptiveManifoldFactory()
        {
            m_name = QObject::tr("Adaptive Manifold Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAdaptiveManifold>(pParam);
        }
};

#endif // COCVWIDGETADAPTIVEMANIFOLD_HPP
