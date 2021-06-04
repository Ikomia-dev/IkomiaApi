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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicAutoBalanceParam>();

            auto pSpinArea = addSpin(0, tr("Area"), m_pParam->m_area);
            auto pSpinSmooth = addDoubleSpin(1, tr("Smooth"), m_pParam->m_smooth);

            auto pComboColorSpace = addCombo(2, tr("Color space"));
            pComboColorSpace->addItem("HSI", CGmicAutoBalanceParam::HSI);
            pComboColorSpace->addItem("HSV", CGmicAutoBalanceParam::HSV);
            pComboColorSpace->addItem("Lab", CGmicAutoBalanceParam::LAB);
            pComboColorSpace->addItem("Linear RGB", CGmicAutoBalanceParam::LINEAR_RGB);
            pComboColorSpace->addItem("RGB", CGmicAutoBalanceParam::RGB);
            pComboColorSpace->addItem("YCrCb", CGmicAutoBalanceParam::YCBCR);
            pComboColorSpace->setCurrentIndex(pComboColorSpace->findData(m_pParam->m_colorSpace));

            auto pCheckBalance = addCheck(3, "sRGB balance", m_pParam->m_bBalance);
            auto pCheckReduceRAM = addCheck(4, tr("Reduce RAM"), m_pParam->m_bReduceRAM);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_area = pSpinArea->value();
                m_pParam->m_smooth = pSpinSmooth->value();
                m_pParam->m_colorSpace = pComboColorSpace->currentData().toInt();
                m_pParam->m_bBalance = pCheckBalance->isChecked();
                m_pParam->m_bReduceRAM = pCheckReduceRAM->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicAutoBalanceParam> m_pParam = nullptr;
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
