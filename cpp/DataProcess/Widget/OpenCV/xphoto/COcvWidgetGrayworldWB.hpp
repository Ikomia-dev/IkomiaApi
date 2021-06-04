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

#ifndef COCVWIDGETGRAYWORLDWB_HPP
#define COCVWIDGETGRAYWORLDWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvGrayworldWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetGrayworldWB : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGrayworldWB(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGrayworldWB(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGrayworldWBParam>();

            QLabel* pLabelSpinSat = new QLabel(QObject::tr("Saturation threshold"));
            m_pDblSpinSat = new QDoubleSpinBox;
            m_pDblSpinSat->setValue(m_pParam->m_satThreshold);
            m_pDblSpinSat->setSingleStep(0.1f);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_satThreshold = m_pDblSpinSat->value();
                emit doApplyProcess(m_pParam);
            });


            m_pLayout->addWidget(pLabelSpinSat, 0, 0);
            m_pLayout->addWidget(m_pDblSpinSat, 0, 1);

        }

    private:

        std::shared_ptr<COcvGrayworldWBParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pDblSpinSat = nullptr;
};

class COcvWidgetGrayworldWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetGrayworldWBFactory()
        {
            m_name = QObject::tr("GrayworldWB").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGrayworldWB>(pParam);
        }
};
#endif // COCVWIDGETGRAYWORLDWB_HPP
