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

#ifndef COCVWIDGETBCKGNDSUBLSBP_HPP
#define COCVWIDGETBCKGNDSUBLSBP_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubLsbp.hpp"

class COcvWidgetBckgndSubLsbp : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubLsbp(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubLsbp(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubLsbpParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubLsbpParam>();

            m_pComboMotion = addCombo(0, tr("Motion compensation"));
            m_pComboMotion->addItem(tr("None"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE);
            m_pComboMotion->addItem(tr("LK"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_LK);
            m_pComboMotion->setCurrentIndex(m_pComboMotion->findData(m_pParam->m_motionComp));

            m_pSpinSamples = addSpin(1, tr("Samples count"), m_pParam->m_samples);
            m_pSpinRadius = addSpin(2, tr("LSBP radius"), m_pParam->m_lsbpRadius);
            m_pSpinTLower = addDoubleSpin(3, tr("T lower bound"), m_pParam->m_TLower);
            m_pSpinTUpper = addDoubleSpin(4, tr("T upper bound"), m_pParam->m_TUpper);
            m_pSpinTInc = addDoubleSpin(5, tr("T increase step"), m_pParam->m_TInc);
            m_pSpinTDec = addDoubleSpin(6, tr("T decrease step"), m_pParam->m_TDec, 0.0, 10.0, 0.01, 2);
            m_pSpinRscale = addDoubleSpin(7, tr("Scale factor for threshold"), m_pParam->m_RScale);
            m_pSpinRIncDec = addDoubleSpin(8, tr("Increase/decrease step for threshold"), m_pParam->m_RIncDec, 0.0, 10.0, 0.001, 3);
            m_pSpinNoiseRemoveBG = addDoubleSpin(9, tr("Strength of the noise removal for background points"), m_pParam->m_noiseRemovalThresholdFacBG, 0.0, 1.0, 0.0001, 4);
            m_pSpinNoiseRemoveFG = addDoubleSpin(10, tr("Strength of the noise removal for foreground points"), m_pParam->m_noiseRemovalThresholdFacFG, 0.0, 1.0, 0.0001, 4);
            m_pSpinThreshold = addSpin(11, tr("LSBP threshold"), m_pParam->m_lsbpThreshold);
            m_pSpinMintCount = addSpin(12, tr("Minimal number of matches"), m_pParam->m_minCount);
        }

        void onApply() override
        {
            m_pParam->m_motionComp = m_pComboMotion->currentData().toInt();
            m_pParam->m_samples = m_pSpinSamples->value();
            m_pParam->m_lsbpRadius = m_pSpinRadius->value();
            m_pParam->m_TLower = m_pSpinTLower->value();
            m_pParam->m_TUpper = m_pSpinTUpper->value();
            m_pParam->m_TInc = m_pSpinTInc->value();
            m_pParam->m_TDec = m_pSpinTDec->value();
            m_pParam->m_RScale = m_pSpinRscale->value();
            m_pParam->m_RIncDec = m_pSpinRIncDec->value();
            m_pParam->m_noiseRemovalThresholdFacBG = m_pSpinNoiseRemoveBG->value();
            m_pParam->m_noiseRemovalThresholdFacFG = m_pSpinNoiseRemoveFG->value();
            m_pParam->m_lsbpThreshold = m_pSpinThreshold->value();
            m_pParam->m_minCount = m_pSpinMintCount->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBckgndSubLsbpParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinTLower = nullptr;
        QDoubleSpinBox* m_pSpinTUpper = nullptr;
        QDoubleSpinBox* m_pSpinTInc = nullptr;
        QDoubleSpinBox* m_pSpinTDec = nullptr;
        QDoubleSpinBox* m_pSpinRscale = nullptr;
        QDoubleSpinBox* m_pSpinRIncDec = nullptr;
        QDoubleSpinBox* m_pSpinNoiseRemoveBG = nullptr;
        QDoubleSpinBox* m_pSpinNoiseRemoveFG = nullptr;
        QSpinBox*       m_pSpinSamples = nullptr;
        QSpinBox*       m_pSpinRadius = nullptr;
        QSpinBox*       m_pSpinThreshold = nullptr;
        QSpinBox*       m_pSpinMintCount = nullptr;
        QComboBox*      m_pComboMotion = nullptr;
};

class COcvWidgetBckgndSubLsbpFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubLsbpFactory()
        {
            m_name = "ocv_bck_substractor_lspb";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubLsbp>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBLSBP_HPP
