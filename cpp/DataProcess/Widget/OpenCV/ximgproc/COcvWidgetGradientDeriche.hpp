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

#ifndef COCVWIDGETGRADIENTDERICHE_HPP
#define COCVWIDGETGRADIENTDERICHE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGradientDeriche.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>

class COcvWidgetGradientDeriche : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGradientDeriche(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGradientDeriche(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGradientDericheParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGradientDericheParam>();

            QGroupBox* pBox = new QGroupBox("Orientation");
            QGridLayout* pBoxLayout = new QGridLayout;
            QRadioButton* pRadioX = new QRadioButton("X");
            QRadioButton* pRadioY = new QRadioButton("Y");
            pBoxLayout->addWidget(pRadioX);
            pBoxLayout->addWidget(pRadioY);
            pBox->setLayout(pBoxLayout);
            if(m_pParam->m_orientation == COcvGradientDericheParam::orientation::X)
                pRadioX->setChecked(true);
            else
                pRadioY->setChecked(true);

            QLabel* pLabelSpinDerive = new QLabel(QObject::tr("Derive"));
            m_pDblSpinDerive = new QDoubleSpinBox;
            m_pDblSpinDerive->setRange(0, 4);
            m_pDblSpinDerive->setSingleStep(0.1);
            m_pDblSpinDerive->setValue(m_pParam->m_alphaDerive);

            QLabel* pLabelSpinMean = new QLabel(QObject::tr("Mean"));
            m_pDblSpinMean = new QDoubleSpinBox;
            m_pDblSpinMean->setRange(0, 4);
            m_pDblSpinMean->setSingleStep(0.1);
            m_pDblSpinMean->setValue(m_pParam->m_alphaMean);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_orientation = pRadioX->isChecked() ? COcvGradientDericheParam::orientation::X : COcvGradientDericheParam::orientation::Y;
                m_pParam->m_alphaDerive = m_pDblSpinDerive->value();
                m_pParam->m_alphaMean = m_pDblSpinMean->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pBox, 0, 0, 1, 2);

            m_pLayout->addWidget(pLabelSpinDerive, 1, 0);
            m_pLayout->addWidget(m_pDblSpinDerive, 1, 1);

            m_pLayout->addWidget(pLabelSpinMean, 2, 0);
            m_pLayout->addWidget(m_pDblSpinMean, 2, 1);

            
        }

    private:

        std::shared_ptr<COcvGradientDericheParam> m_pParam = nullptr;
        QDoubleSpinBox*                     m_pDblSpinDerive = nullptr;
        QDoubleSpinBox*                     m_pDblSpinMean = nullptr;
};

class COcvWidgetGradientDericheFactory : public CWidgetFactory
{
    public:

        COcvWidgetGradientDericheFactory()
        {
            m_name = QObject::tr("Deriche gradient filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGradientDeriche>(pParam);
        }
};

#endif // COCVWIDGETGRADIENTDERICHE_HPP
