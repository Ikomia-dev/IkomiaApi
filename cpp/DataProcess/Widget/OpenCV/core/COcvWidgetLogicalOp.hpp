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

#ifndef COCVWIDGETAND_HPP
#define COCVWIDGETAND_HPP

#include "Process/OpenCV/core/COcvLogicalOp.hpp"
#include "Core/CWidgetFactory.hpp"
#include <QComboBox>

class COcvWidgetLogicalOp : public CWorkflowTaskWidget
{
    public:

        COcvWidgetLogicalOp(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetLogicalOp(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvLogicalOpParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvLogicalOpParam>();

            auto pLabel = new QLabel(tr("Operation"));

            m_pOpComboBox = new QComboBox();
            m_pOpComboBox->addItem("And", COcvLogicalOpParam::AND);
            m_pOpComboBox->addItem("Or", COcvLogicalOpParam::OR);
            m_pOpComboBox->addItem("Not", COcvLogicalOpParam::NOT);
            m_pOpComboBox->addItem("Xor", COcvLogicalOpParam::XOR);
            m_pOpComboBox->setCurrentIndex(m_pOpComboBox->findData(m_pParam->m_op));

            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(m_pOpComboBox, 0, 1);
        }

        void onApply() override
        {
            m_pParam->m_op = m_pOpComboBox->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        QComboBox*                          m_pOpComboBox = nullptr;
        std::shared_ptr<COcvLogicalOpParam> m_pParam = nullptr;
};

class COcvWidgetLogicalOpFactory : public CWidgetFactory
{
    public:

        COcvWidgetLogicalOpFactory()
        {
            m_name = QObject::tr("Logical operations").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLogicalOp>(pParam);
        }
};

#endif // COCVWIDGETAND_HPP
