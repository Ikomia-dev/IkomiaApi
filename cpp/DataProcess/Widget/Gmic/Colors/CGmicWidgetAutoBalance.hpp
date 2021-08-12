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

#ifndef CGMICWIDGETAUTOBALANCE_HPP
#define CGMICWIDGETAUTOBALANCE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicAutoBalance.hpp"

class CGmicWidgetAutoBalance : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetAutoBalance(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetAutoBalance(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicAutoBalanceParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicAutoBalanceParam>();

            m_pSpinArea = addSpin(0, tr("Area"), m_pParam->m_area);
            m_pSpinSmooth = addDoubleSpin(1, tr("Smooth"), m_pParam->m_smooth);

            m_pComboColorSpace = addCombo(2, tr("Color space"));
            m_pComboColorSpace->addItem("HSI", CGmicAutoBalanceParam::HSI);
            m_pComboColorSpace->addItem("HSV", CGmicAutoBalanceParam::HSV);
            m_pComboColorSpace->addItem("Lab", CGmicAutoBalanceParam::LAB);
            m_pComboColorSpace->addItem("Linear RGB", CGmicAutoBalanceParam::LINEAR_RGB);
            m_pComboColorSpace->addItem("RGB", CGmicAutoBalanceParam::RGB);
            m_pComboColorSpace->addItem("YCrCb", CGmicAutoBalanceParam::YCBCR);
            m_pComboColorSpace->setCurrentIndex(m_pComboColorSpace->findData(m_pParam->m_colorSpace));

            m_pCheckBalance = addCheck(3, "sRGB balance", m_pParam->m_bBalance);
            m_pCheckReduceRAM = addCheck(4, tr("Reduce RAM"), m_pParam->m_bReduceRAM);
        }

        void onApply() override
        {
            m_pParam->m_area = m_pSpinArea->value();
            m_pParam->m_smooth = m_pSpinSmooth->value();
            m_pParam->m_colorSpace = m_pComboColorSpace->currentData().toInt();
            m_pParam->m_bBalance = m_pCheckBalance->isChecked();
            m_pParam->m_bReduceRAM = m_pCheckReduceRAM->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicAutoBalanceParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinSmooth = nullptr;
        QSpinBox*       m_pSpinArea = nullptr;
        QComboBox*      m_pComboColorSpace = nullptr;
        QCheckBox*      m_pCheckBalance = nullptr;
        QCheckBox*      m_pCheckReduceRAM = nullptr;
};

class CGmicWidgetAutoBalanceFactory : public CWidgetFactory
{
    public:

        CGmicWidgetAutoBalanceFactory()
        {
            m_name = QObject::tr("Auto balance").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetAutoBalance>(pParam);
        }
};

#endif // CGMICWIDGETAUTOBALANCE_HPP
