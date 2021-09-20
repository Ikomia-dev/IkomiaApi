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

#ifndef CWIDGETPLOTPROCESS_H
#define CWIDGETPLOTPROCESS_H

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CPlotMerge.hpp"
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

class CWidgetPlotMerge : public CWorkflowTaskWidget
{
    public:

        CWidgetPlotMerge(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        CWidgetPlotMerge(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CPlotMergeParam>(pParam);
            init();
        }

    protected:

        void    init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CPlotMergeParam>();

            auto pLabel = new QLabel(tr("Number of inputs"));
            m_pSpinInputs = new QSpinBox;
            m_pSpinInputs->setValue(m_pParam->m_inputCount);
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(m_pSpinInputs, 0, 1);
        }

        void    onApply() override
        {
            m_pParam->m_inputCount = m_pSpinInputs->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CPlotMergeParam>    m_pParam = nullptr;
        QSpinBox*                           m_pSpinInputs = nullptr;
};

class CWidgetPlotMergeFactory : public CWidgetFactory
{
    public:

        CWidgetPlotMergeFactory()
        {
            m_name = "ik_plot_merge";
        }
        ~CWidgetPlotMergeFactory() {}

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<CWidgetPlotMerge>(pParam);
        }
};

#endif // CWIDGETPLOTPROCESS_H
