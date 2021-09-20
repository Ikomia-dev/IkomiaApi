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

#ifndef COCVWIDGETTONEMAPDURAND_HPP
#define COCVWIDGETTONEMAPDURAND_HPP

#include "../../../Process/OpenCV/xphoto/COcvTonemapDurand.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetTonemapDurand : public CWorkflowTaskWidget
{
    public:

        COcvWidgetTonemapDurand(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetTonemapDurand(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvTonemapDurandParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvTonemapDurandParam>();

            auto pLabelGamma = new QLabel(tr("Gamma"));
            auto pLabelContrast = new QLabel(tr("Contrast"));
            auto pLabelSat = new QLabel(tr("Saturation"));
            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma color"));
            m_pSpinSigmaS = new QDoubleSpinBox;
            m_pSpinSigmaR = new QDoubleSpinBox;
            m_pSpinGamma = new QDoubleSpinBox;
            m_pSpinContrast = new QDoubleSpinBox;
            m_pSpinSat = new QDoubleSpinBox;

            m_pSpinGamma->setValue(m_pParam->m_gamma);
            m_pSpinGamma->setRange(0, 100);
            m_pSpinGamma->setSingleStep(0.1);

            m_pSpinContrast->setValue(m_pParam->m_contrast);
            m_pSpinContrast->setRange(0, 100);
            m_pSpinContrast->setSingleStep(0.1);

            m_pSpinSat->setValue(m_pParam->m_saturation);
            m_pSpinSat->setRange(0, 100);
            m_pSpinSat->setSingleStep(0.1);

            m_pSpinSigmaS->setSingleStep(1);
            m_pSpinSigmaR->setSingleStep(1);

            m_pSpinSigmaS->setRange(0, 100);
            m_pSpinSigmaR->setRange(0, 100);

            m_pSpinSigmaS->setValue(m_pParam->m_sigmaSpace);
            m_pSpinSigmaR->setValue(m_pParam->m_sigmaColor);

            m_pLayout->addWidget(pLabelGamma, 0, 0);
            m_pLayout->addWidget(m_pSpinGamma, 0, 1);

            m_pLayout->addWidget(pLabelContrast, 1, 0);
            m_pLayout->addWidget(m_pSpinContrast, 1, 1);

            m_pLayout->addWidget(pLabelSat, 2, 0);
            m_pLayout->addWidget(m_pSpinSat, 2, 1);

            m_pLayout->addWidget(pLabelSigmaS, 3, 0);
            m_pLayout->addWidget(m_pSpinSigmaS, 3, 1);

            m_pLayout->addWidget(pLabelSigmaR, 4, 0);
            m_pLayout->addWidget(m_pSpinSigmaR, 4, 1);
        }

        void onApply() override
        {
            m_pParam->m_gamma = m_pSpinGamma->value();
            m_pParam->m_contrast = m_pSpinContrast->value();
            m_pParam->m_saturation = m_pSpinSat->value();
            m_pParam->m_sigmaSpace = m_pSpinSigmaS->value();
            m_pParam->m_sigmaColor = m_pSpinSigmaR->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvTonemapDurandParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaS = nullptr;
        QDoubleSpinBox* m_pSpinSigmaR = nullptr;
        QDoubleSpinBox* m_pSpinGamma = nullptr;
        QDoubleSpinBox* m_pSpinContrast = nullptr;
        QDoubleSpinBox* m_pSpinSat = nullptr;
};

class COcvWidgetTonemapDurandFactory : public CWidgetFactory
{
    public:

        COcvWidgetTonemapDurandFactory()
        {
            m_name = "ocv_tonemap_durand";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTonemapDurand>(pParam);
        }
};

#endif // COCVWIDGETTONEMAPDURAND_HPP
