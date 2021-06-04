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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvTonemapDurandParam>();

            auto pLabelGamma = new QLabel(tr("Gamma"));
            auto pLabelContrast = new QLabel(tr("Contrast"));
            auto pLabelSat = new QLabel(tr("Saturation"));
            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma color"));
            auto pSpinSigmaS = new QDoubleSpinBox;
            auto pSpinSigmaR = new QDoubleSpinBox;
            auto pSpinGamma = new QDoubleSpinBox;
            auto pSpinContrast = new QDoubleSpinBox;
            auto pSpinSat = new QDoubleSpinBox;

            pSpinGamma->setValue(m_pParam->m_gamma);
            pSpinGamma->setRange(0, 100);
            pSpinGamma->setSingleStep(0.1);

            pSpinContrast->setValue(m_pParam->m_contrast);
            pSpinContrast->setRange(0, 100);
            pSpinContrast->setSingleStep(0.1);

            pSpinSat->setValue(m_pParam->m_saturation);
            pSpinSat->setRange(0, 100);
            pSpinSat->setSingleStep(0.1);

            pSpinSigmaS->setSingleStep(1);
            pSpinSigmaR->setSingleStep(1);

            pSpinSigmaS->setRange(0, 100);
            pSpinSigmaR->setRange(0, 100);

            pSpinSigmaS->setValue(m_pParam->m_sigmaSpace);
            pSpinSigmaR->setValue(m_pParam->m_sigmaColor);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_gamma = pSpinGamma->value();
                m_pParam->m_contrast = pSpinContrast->value();
                m_pParam->m_saturation = pSpinSat->value();
                m_pParam->m_sigmaSpace = pSpinSigmaS->value();
                m_pParam->m_sigmaColor = pSpinSigmaR->value();
                emit doApplyProcess(m_pParam); } );

            m_pLayout->addWidget(pLabelGamma, 0, 0);
            m_pLayout->addWidget(pSpinGamma, 0, 1);

            m_pLayout->addWidget(pLabelContrast, 1, 0);
            m_pLayout->addWidget(pSpinContrast, 1, 1);

            m_pLayout->addWidget(pLabelSat, 2, 0);
            m_pLayout->addWidget(pSpinSat, 2, 1);

            m_pLayout->addWidget(pLabelSigmaS, 3, 0);
            m_pLayout->addWidget(pSpinSigmaS, 3, 1);

            m_pLayout->addWidget(pLabelSigmaR, 4, 0);
            m_pLayout->addWidget(pSpinSigmaR, 4, 1);
        }

    private:

        std::shared_ptr<COcvTonemapDurandParam> m_pParam = nullptr;
};

class COcvWidgetTonemapDurandFactory : public CWidgetFactory
{
    public:

        COcvWidgetTonemapDurandFactory()
        {
            m_name = QObject::tr("Tonemap Durand").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTonemapDurand>(pParam);
        }
};

#endif // COCVWIDGETTONEMAPDURAND_HPP
