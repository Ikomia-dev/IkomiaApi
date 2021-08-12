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

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAdaptiveManifoldParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma Spatial"));
            m_pSpinSigmaS = new QDoubleSpinBox;
            auto pLabelSigmaR = new QLabel(tr("Sigma Color Space"));
            m_pSpinSigmaR = new QDoubleSpinBox;
            auto pLabelTreeHeight = new QLabel(tr("Tree Height"));
            m_pSpinTreeHeight = new QSpinBox;
            auto pLabelNumPCA = new QLabel(tr("Iterations"));
            m_pSpinNumPCA = new QSpinBox;
            m_pCheckOutliers = new QCheckBox(tr("Adjust Outliers"));
            m_pCheckRng = new QCheckBox(tr("Use RNG"));

            m_pSpinSigmaS->setSingleStep(0.1);
            m_pSpinSigmaR->setSingleStep(0.1);
            m_pSpinTreeHeight->setMinimum(-1);

            m_pSpinSigmaS->setValue(m_pParam->m_sigmaS);
            m_pSpinSigmaR->setValue(m_pParam->m_sigmaR);
            m_pSpinTreeHeight->setValue(m_pParam->m_treeHeight);
            m_pSpinNumPCA->setValue(m_pParam->m_numPcaIterations);
            m_pCheckOutliers->setChecked(m_pParam->m_bAdjustOutliers);
            m_pCheckRng->setChecked(m_pParam->m_bUseRNG);
            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(m_pSpinSigmaS, 0, 1);

            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(m_pSpinSigmaR, 1, 1);

            m_pLayout->addWidget(pLabelTreeHeight, 2, 0);
            m_pLayout->addWidget(m_pSpinTreeHeight, 2, 1);

            m_pLayout->addWidget(pLabelNumPCA, 3, 0);
            m_pLayout->addWidget(m_pSpinNumPCA, 3, 1);

            m_pLayout->addWidget(m_pCheckOutliers, 4, 0);
            m_pLayout->addWidget(m_pCheckRng, 5, 0);
        }

        void onApply() override
        {
            m_pParam->m_sigmaS = m_pSpinSigmaS->value();
            m_pParam->m_sigmaR = m_pSpinSigmaR->value();
            m_pParam->m_treeHeight = m_pSpinTreeHeight->value();
            m_pParam->m_numPcaIterations = m_pSpinNumPCA->value();
            m_pParam->m_bAdjustOutliers = m_pCheckOutliers->isChecked();
            m_pParam->m_bUseRNG = m_pCheckRng->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvAdaptiveManifoldParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaS = nullptr;
        QDoubleSpinBox* m_pSpinSigmaR = nullptr;
        QSpinBox*       m_pSpinTreeHeight = nullptr;
        QSpinBox*       m_pSpinNumPCA = nullptr;
        QCheckBox*      m_pCheckOutliers = nullptr;
        QCheckBox*      m_pCheckRng = nullptr;
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
