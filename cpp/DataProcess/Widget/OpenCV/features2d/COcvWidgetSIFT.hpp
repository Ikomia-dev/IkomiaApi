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

#ifndef COCVWIDGETSIFT_HPP
#define COCVWIDGETSIFT_HPP

#include "Protocol/CProtocolTaskWidget.h"
#include "Process/OpenCV/features2d/COcvSIFT.hpp"
#include "Core/CWidgetFactory.hpp"

class COcvWidgetSIFT : public CProtocolTaskWidget
{
    public:

        COcvWidgetSIFT(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSIFT(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSIFTParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSIFTParam>();

            auto pSpinFeatures = addSpin(tr("Features count"), m_pParam->m_featuresCount);
            auto pSpinOctaveLayer = addSpin(tr("Octave layers count"), m_pParam->m_octaveLayersCount);
            auto pSpinContrastThresh = addDoubleSpin(tr("Contrast threshold"), m_pParam->m_contrastThreshold, 0.0, 1.0, 0.01, 2);
            auto pSpinEdgeThresh = addDoubleSpin(tr("Edge threshold"), m_pParam->m_edgeThreshold, 0.0, DBL_MAX, 1, 1);
            auto pSpinSigma = addDoubleSpin(tr("Gaussian sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            auto pCheckKeyPts = addCheck(tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            auto pCheckDetect = addCheck(tr("Detect"), m_pParam->m_bDetect);
            pCheckDetect->setEnabled(!m_pParam->m_bUseProvidedKeypoints);
            auto pCheckCompute = addCheck(tr("Compute"), m_pParam->m_bCompute);

            connect(pCheckKeyPts, &QCheckBox::clicked, [=](bool checked)
            {
                if(checked)
                {
                    pCheckDetect->setEnabled(true);
                }
                else
                {
                    pCheckDetect->setChecked(true);
                    pCheckDetect->setEnabled(false);
                }
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_featuresCount = pSpinFeatures->value();
                m_pParam->m_octaveLayersCount = pSpinOctaveLayer->value();
                m_pParam->m_contrastThreshold = pSpinContrastThresh->value();
                m_pParam->m_edgeThreshold = pSpinEdgeThresh->value();
                m_pParam->m_sigma = pSpinSigma->value();
                m_pParam->m_bUseProvidedKeypoints = pCheckKeyPts->isChecked();
                m_pParam->m_bDetect = pCheckDetect->isChecked();
                m_pParam->m_bCompute = pCheckCompute->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvSIFTParam> m_pParam = nullptr;
};

class COcvWidgetSIFTFactory : public CWidgetFactory
{
    public:

        COcvWidgetSIFTFactory()
        {
            m_name = QObject::tr("SIFT").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSIFT>(pParam);
        }
};

#endif // COCVWIDGETSIFT_HPP
