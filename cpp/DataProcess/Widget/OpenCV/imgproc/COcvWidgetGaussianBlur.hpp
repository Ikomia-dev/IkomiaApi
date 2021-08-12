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

#ifndef COCVWIDGETGAUSSIANBLUR_HPP
#define COCVWIDGETGAUSSIANBLUR_HPP

#include <QComboBox>
#include <QDoubleSpinBox>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvGaussianBlur.hpp"

class COcvWidgetGaussianBlur : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGaussianBlur(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGaussianBlur(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGaussianBlurParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGaussianBlurParam>();

            auto pLabelSize = new QLabel(QObject::tr("Kernel size"));
            m_pSpinSize = new QSpinBox;
            m_pSpinSize->setValue(m_pParam->m_size.width);
            m_pSpinSize->setSingleStep(2);
            m_pSpinSize->setRange(1, INT_MAX - 1);

            auto pLabelSigmaX = new QLabel(QObject::tr("Sigma X"));
            m_pSpinSigmaX = new QDoubleSpinBox;
            m_pSpinSigmaX->setValue(m_pParam->m_sigmaX);
            m_pSpinSigmaX->setSingleStep(0.1);
            m_pSpinSigmaX->setRange(0, 255);

            auto pLabelSigmaY = new QLabel(QObject::tr("Sigma Y"));
            m_pSpinSigmaY = new QDoubleSpinBox;
            m_pSpinSigmaY->setValue(m_pParam->m_sigmaY);
            m_pSpinSigmaY->setSingleStep(0.1);
            m_pSpinSigmaY->setRange(0, 255);

            m_pLayout->addWidget(pLabelSize, 0, 0);
            m_pLayout->addWidget(m_pSpinSize, 0, 1);
            m_pLayout->addWidget(pLabelSigmaX, 1, 0);
            m_pLayout->addWidget(m_pSpinSigmaX, 1, 1);
            m_pLayout->addWidget(pLabelSigmaY, 2, 0);
            m_pLayout->addWidget(m_pSpinSigmaY, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_size.width = m_pParam->m_size.height = m_pSpinSize->value();
            m_pParam->m_sigmaX = m_pSpinSigmaX->value();
            m_pParam->m_sigmaY = m_pSpinSigmaY->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvGaussianBlurParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSigmaX = nullptr;
        QDoubleSpinBox* m_pSpinSigmaY = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
};

class COcvWidgetGaussianBlurFactory : public CWidgetFactory
{
    public:

        COcvWidgetGaussianBlurFactory()
        {
            m_name = QObject::tr("Gaussian Blur").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGaussianBlur>(pParam);
        }
};

#endif // COCVWIDGETGAUSSIANBLUR_HPP
