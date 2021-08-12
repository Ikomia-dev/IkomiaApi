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

#include "Workflow/CWorkflowTaskWidget.h"
#include "Process/OpenCV/features2d/COcvSIFT.hpp"
#include "Core/CWidgetFactory.hpp"

class COcvWidgetSIFT : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSIFT(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSIFT(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSIFTParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSIFTParam>();

            m_pSpinFeatures = addSpin(tr("Features count"), m_pParam->m_featuresCount);
            m_pSpinOctaveLayer = addSpin(tr("Octave layers count"), m_pParam->m_octaveLayersCount);
            m_pSpinContrastThresh = addDoubleSpin(tr("Contrast threshold"), m_pParam->m_contrastThreshold, 0.0, 1.0, 0.01, 2);
            m_pSpinEdgeThresh = addDoubleSpin(tr("Edge threshold"), m_pParam->m_edgeThreshold, 0.0, DBL_MAX, 1, 1);
            m_pSpinSigma = addDoubleSpin(tr("Gaussian sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            m_pCheckKeyPts = addCheck(tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            m_pCheckDetect = addCheck(tr("Detect"), m_pParam->m_bDetect);
            m_pCheckDetect->setEnabled(!m_pParam->m_bUseProvidedKeypoints);
            m_pCheckCompute = addCheck(tr("Compute"), m_pParam->m_bCompute);

            connect(m_pCheckKeyPts, &QCheckBox::clicked, [&](bool checked)
            {
                if(checked)
                {
                    m_pCheckDetect->setEnabled(true);
                }
                else
                {
                    m_pCheckDetect->setChecked(true);
                    m_pCheckDetect->setEnabled(false);
                }
            });
        }

        void onApply() override
        {
            m_pParam->m_featuresCount = m_pSpinFeatures->value();
            m_pParam->m_octaveLayersCount = m_pSpinOctaveLayer->value();
            m_pParam->m_contrastThreshold = m_pSpinContrastThresh->value();
            m_pParam->m_edgeThreshold = m_pSpinEdgeThresh->value();
            m_pParam->m_sigma = m_pSpinSigma->value();
            m_pParam->m_bUseProvidedKeypoints = m_pCheckKeyPts->isChecked();
            m_pParam->m_bDetect = m_pCheckDetect->isChecked();
            m_pParam->m_bCompute = m_pCheckCompute->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSIFTParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinContrastThresh = nullptr;
        QDoubleSpinBox* m_pSpinEdgeThresh = nullptr;
        QDoubleSpinBox* m_pSpinSigma = nullptr;
        QSpinBox*       m_pSpinFeatures = nullptr;
        QSpinBox*       m_pSpinOctaveLayer = nullptr;
        QCheckBox*      m_pCheckKeyPts = nullptr;
        QCheckBox*      m_pCheckDetect = nullptr;
        QCheckBox*      m_pCheckCompute = nullptr;
};

class COcvWidgetSIFTFactory : public CWidgetFactory
{
    public:

        COcvWidgetSIFTFactory()
        {
            m_name = QObject::tr("SIFT").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSIFT>(pParam);
        }
};

#endif // COCVWIDGETSIFT_HPP
