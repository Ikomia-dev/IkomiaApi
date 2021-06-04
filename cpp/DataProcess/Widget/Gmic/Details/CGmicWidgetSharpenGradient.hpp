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

#ifndef CGMICWIDGETSHARPENGRADIENT_HPP
#define CGMICWIDGETSHARPENGRADIENT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenGradient.hpp"

class CGmicWidgetSharpenGradient : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSharpenGradient(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenGradient(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenGradientParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenGradientParam>();

            auto pSpinAmount = addDoubleSpin(0, tr("Amount"), m_pParam->m_amount, 0, 2, 0.1);
            auto pSpinScale = addDoubleSpin(1, tr("Scale"), m_pParam->m_scale, 0.1, 2, 0.1);

            auto pComboValues = addCombo(2, tr("Values"));
            pComboValues->addItem(tr("Cut"), CGmicSharpenGradientParam::CUT);
            pComboValues->addItem(tr("Normalize luma"), CGmicSharpenGradientParam::NORMALYZE);
            pComboValues->setCurrentIndex(pComboValues->findData(m_pParam->m_values));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_amount = pSpinAmount->value();
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_values = pComboValues->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSharpenGradientParam> m_pParam = nullptr;
};

class CGmicWidgetSharpenGradientFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenGradientFactory()
        {
            m_name = QObject::tr("Sharpen [Gradient]").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenGradient>(pParam);
        }
};

#endif // CGMICWIDGETSHARPENGRADIENT_HPP
