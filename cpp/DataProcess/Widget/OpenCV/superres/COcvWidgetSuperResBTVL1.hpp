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

#ifndef COCVWIDGETSUPERRESBTVL1_HPP
#define COCVWIDGETSUPERRESBTVL1_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/superres/COcvSuperResBTVL1.h"

class COcvWidgetSuperResBTVL1 : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSuperResBTVL1(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperResBTVL1(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperResBTVL1Param>();

            m_pSpinScale = addSpin(0, tr("Scale factor"), m_pParam->m_scale);
            m_pSpinIterations = addSpin(1, tr("Number of iterations"), m_pParam->m_iterations);
            m_pSpinTau = addDoubleSpin(2, tr("Tau"), m_pParam->m_tau, 0.0, DBL_MAX, 0.1, 1);
            m_pSpinLambda = addDoubleSpin(3, tr("Lambda"), m_pParam->m_lambda, 0.0, DBL_MAX, 0.01, 2);
            m_pSpinAlpha = addDoubleSpin(4, tr("Alpha"), m_pParam->m_alpha, 0.0, DBL_MAX, 0.1, 1);
            m_pSpinBTVKernelSize = addSpin(5, tr("Bilateral TV kernel size"), m_pParam->m_btvKernelSize, 1, INT_MAX, 2);
            m_pSpinBlurKernelSize = addDoubleSpin(6, tr("Gaussian kernel size"), m_pParam->m_blurKernelSize, 1, INT_MAX, 2);
            m_pSpinBlurSigma = addDoubleSpin(7, tr("Gaussian sigma"), m_pParam->m_blurSigma, 0.0, DBL_MAX, 0.1, 1);
            m_pSpinTemporalRadius = addSpin(8, tr("Radius of temporal search area"), m_pParam->m_temporalAreaRadius, 1, INT_MAX, 1);

            m_pComboOptFlow = addCombo(9, tr("Optical flow"));
            m_pComboOptFlow->addItem(tr("Brox"), COcvSuperResBTVL1Param::BROX);
            m_pComboOptFlow->addItem(tr("DualTVL1"), COcvSuperResBTVL1Param::DUAL_TVL1);
            m_pComboOptFlow->addItem(tr("Farneback"), COcvSuperResBTVL1Param::FARNEBACK);
            m_pComboOptFlow->addItem(tr("PyrLK"), COcvSuperResBTVL1Param::PYR_LK);
            m_pComboOptFlow->setCurrentIndex(m_pComboOptFlow->findData(m_pParam->m_opticalFlowType));
        }

        void onApply() override
        {
            m_pParam->m_scale = m_pSpinScale->value();
            m_pParam->m_iterations = m_pSpinIterations->value();
            m_pParam->m_tau = m_pSpinTau->value();
            m_pParam->m_lambda = m_pSpinLambda->value();
            m_pParam->m_alpha = m_pSpinAlpha->value();
            m_pParam->m_btvKernelSize = m_pSpinBTVKernelSize->value();
            m_pParam->m_blurKernelSize = m_pSpinBlurKernelSize->value();
            m_pParam->m_blurSigma = m_pSpinBlurSigma->value();
            m_pParam->m_temporalAreaRadius = m_pSpinTemporalRadius->value();
            m_pParam->m_opticalFlowType = m_pComboOptFlow->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSuperResBTVL1Param> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinTau = nullptr;
        QDoubleSpinBox* m_pSpinLambda = nullptr;
        QDoubleSpinBox* m_pSpinAlpha = nullptr;
        QDoubleSpinBox* m_pSpinBlurKernelSize = nullptr;
        QDoubleSpinBox* m_pSpinBlurSigma = nullptr;
        QSpinBox*       m_pSpinScale = nullptr;
        QSpinBox*       m_pSpinIterations = nullptr;
        QSpinBox*       m_pSpinBTVKernelSize = nullptr;
        QSpinBox*       m_pSpinTemporalRadius = nullptr;
        QComboBox*      m_pComboOptFlow = nullptr;
};

class COcvWidgetSuperResBTVL1Factory : public CWidgetFactory
{
    public:

        COcvWidgetSuperResBTVL1Factory()
        {
            m_name = "ocv_super_resolution_btvl1";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperResBTVL1>(pParam);
        }
};

#endif // COCVWIDGETSUPERRESBTVL1_HPP
