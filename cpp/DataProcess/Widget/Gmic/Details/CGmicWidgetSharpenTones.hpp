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

#ifndef CGMICWIDGETSHARPENTONES_HPP
#define CGMICWIDGETSHARPENTONES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenTones.hpp"

class CGmicWidgetSharpenTones : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSharpenTones(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenTones(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenTonesParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenTonesParam>();

            m_pSpinAmount = addDoubleSpin(0, tr("Amount"), m_pParam->m_amount, 0, 4, 0.1);
            m_pSpinCentre = addSpin(1, tr("Centre"), m_pParam->m_centre, 0, 255);

            m_pComboValues = addCombo(2, tr("Values"));
            m_pComboValues->addItem(tr("Cut"), CGmicSharpenTonesParam::CUT);
            m_pComboValues->addItem(tr("Normalize luma"), CGmicSharpenTonesParam::NORMALYZE);
            m_pComboValues->setCurrentIndex(m_pComboValues->findData(m_pParam->m_values));
        }

        void onApply() override
        {
            m_pParam->m_amount = m_pSpinAmount->value();
            m_pParam->m_centre = m_pSpinCentre->value();
            m_pParam->m_values = m_pComboValues->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicSharpenTonesParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinAmount = nullptr;
        QSpinBox*       m_pSpinCentre = nullptr;
        QComboBox*      m_pComboValues = nullptr;
};

class CGmicWidgetSharpenTonesFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenTonesFactory()
        {
            m_name = QObject::tr("Sharpen [Tones]").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenTones>(pParam);
        }
};

#endif // CGMICWIDGETSHARPENTONES_HPP
