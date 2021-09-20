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

#ifndef COCVWIDGETADD_HPP
#define COCVWIDGETADD_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvAdd.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetAdd : public CWorkflowTaskWidget
{
    public:

        COcvWidgetAdd(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAdd(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAddParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAddParam>();

            m_pComboDepth = addCombo(1, tr("Depth"));
            m_pComboDepth->addItem("Default", -1);
            m_pComboDepth->addItem("CV_8U", CV_8U);
            m_pComboDepth->addItem("CV_16U", CV_16U);
            m_pComboDepth->addItem("CV_32F", CV_32F);
            m_pComboDepth->addItem("CV_64F", CV_64F);
            m_pComboDepth->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_dtype));
        }

        void onApply() override
        {
            m_pParam->m_dtype = m_pComboDepth->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvAddParam> m_pParam = nullptr;
        QComboBox* m_pComboDepth = nullptr;
};

class COcvWidgetAddFactory : public CWidgetFactory
{
    public:

        COcvWidgetAddFactory()
        {
            m_name = "ocv_add";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAdd>(pParam);
        }
};

#endif // COCVWIDGETADD_HPP
