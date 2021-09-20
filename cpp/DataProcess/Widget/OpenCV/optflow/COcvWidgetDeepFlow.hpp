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

#ifndef COCVWIDGETDEEPFLOW_HPP
#define COCVWIDGETDEEPFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvDeepFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDeepFlow : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDeepFlow(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDeepFlow(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDeepFlowParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDeepFlowParam>();

            m_pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDeepFlowParam> m_pParam = nullptr;
        QCheckBox* m_pCheck = nullptr;
};

class COcvWidgetDeepFlowFactory : public CWidgetFactory
{
    public:

        COcvWidgetDeepFlowFactory()
        {
            m_name = "ocv_deepflow";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDeepFlow>(pParam);
        }
};

#endif // COCVWIDGETDEEPFLOW_HPP
