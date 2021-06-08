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

#ifndef COCVWIDGETBCKGNDSUBGSOC_HPP
#define COCVWIDGETBCKGNDSUBGSOC_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubGsoc.hpp"

class COcvWidgetBckgndSubGsoc : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBckgndSubGsoc(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubGsoc(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubGsocParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubGsocParam>();

            auto pComboMotion = addCombo(0, tr("Motion compensation"));
            pComboMotion->addItem(tr("None"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE);
            pComboMotion->addItem(tr("LK"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_LK);
            pComboMotion->setCurrentIndex(pComboMotion->findData(m_pParam->m_motionComp));

            auto pSpinSamples = addSpin(1, tr("Samples count"), m_pParam->m_samples);
            auto pSpinReplaceRate = addDoubleSpin(2, tr("Replace rate"), m_pParam->m_replaceRate, 0.0, 1.0, 0.001, 3);
            auto pSpinPropagationRate = addDoubleSpin(3, tr("Propagation rate"), m_pParam->m_propagationRate, 0.0, 1.0, 0.01, 2);
            auto pSpinHitsThreshold = addSpin(4, tr("Hits threshold"), m_pParam->m_hitsThreshold);
            auto pSpinAlpha = addDoubleSpin(5, tr("Alpha"), m_pParam->m_alpha, 0.0, 10.0, 0.01, 2);
            auto pSpinBeta = addDoubleSpin(6, tr("Beta"), m_pParam->m_beta, 0.0, 10.0, 0.001, 4);
            auto pSpinBlinkingSupDecay = addDoubleSpin(7, tr("Blinking suppression decay"), m_pParam->m_blinkingSupressionDecay, 0.0, 10.0, 0.1, 1);
            auto pSpinBlinkingSupMult = addDoubleSpin(8, tr("Blinking suppression multiplier"), m_pParam->m_blinkingSupressionMultiplier, 0.0, 10.0, 0.1, 1);
            auto pSpinNoiseRemoveBG = addDoubleSpin(9, tr("Strength of the noise removal for background points"), m_pParam->m_noiseRemovalThresholdFacBG, 0.0, 1.0, 0.0001, 4);
            auto pSpinNoiseRemoveFG = addDoubleSpin(10, tr("Strength of the noise removal for foreground points"), m_pParam->m_noiseRemovalThresholdFacFG, 0.0, 1.0, 0.0001, 4);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_motionComp = pComboMotion->currentData().toInt();
                m_pParam->m_samples = pSpinSamples->value();
                m_pParam->m_replaceRate = pSpinReplaceRate->value();
                m_pParam->m_propagationRate = pSpinPropagationRate->value();
                m_pParam->m_hitsThreshold = pSpinHitsThreshold->value();
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_beta = pSpinBeta->value();
                m_pParam->m_blinkingSupressionDecay = pSpinBlinkingSupDecay->value();
                m_pParam->m_blinkingSupressionMultiplier = pSpinBlinkingSupMult->value();
                m_pParam->m_noiseRemovalThresholdFacBG = pSpinNoiseRemoveBG->value();
                m_pParam->m_noiseRemovalThresholdFacFG = pSpinNoiseRemoveFG->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubGsocParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubGsocFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubGsocFactory()
        {
            m_name = QObject::tr("GSOC background substractor").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubGsoc>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBGSOC_HPP
