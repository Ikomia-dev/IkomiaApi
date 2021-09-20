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

#ifndef COCVWIDGETBILATERAL_HPP
#define COCVWIDGETBILATERAL_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvBilateral.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetBilateral : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBilateral(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetBilateral(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBilateralParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBilateralParam>();

            QLabel* pLabelSpin = new QLabel(QObject::tr("Filter size d"));
            m_pSpin = new QSpinBox;
            m_pSpin->setValue(m_pParam->m_diameter);

            QLabel* pLabelSpinSpace = new QLabel(QObject::tr("Sigma spatial"));
            m_pDblSpinSpace = new QDoubleSpinBox;
            m_pDblSpinSpace->setValue(m_pParam->m_sigmaSpace);

            QLabel* pLabelSpinColor = new QLabel(QObject::tr("Sigma range"));
            m_pDblSpinColor = new QDoubleSpinBox;
            m_pDblSpinColor->setValue(m_pParam->m_sigmaColor);

            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(pLabelSpinSpace, 1, 0);
            m_pLayout->addWidget(pLabelSpinColor, 2, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);
            m_pLayout->addWidget(m_pDblSpinSpace, 1, 1);
            m_pLayout->addWidget(m_pDblSpinColor, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_diameter = m_pSpin->value();
            m_pParam->m_sigmaSpace = m_pDblSpinSpace->value();
            m_pParam->m_sigmaColor = m_pDblSpinColor->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBilateralParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
        QDoubleSpinBox*                     m_pDblSpinSpace = nullptr;
        QDoubleSpinBox*                     m_pDblSpinColor = nullptr;
};

class COcvWidgetBilateralFactory : public CWidgetFactory
{
    public:

        COcvWidgetBilateralFactory()
        {
            m_name = "ocv_bilateral_filter";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBilateral>(pParam);
        }
};

#endif // COCVWIDGETBILATERAL_HPP
