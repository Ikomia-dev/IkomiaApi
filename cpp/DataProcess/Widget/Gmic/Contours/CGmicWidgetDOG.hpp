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

#ifndef CGMICWIDGETDOG_HPP
#define CGMICWIDGETDOG_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicDOG.hpp"

class CGmicWidgetDoG : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetDoG(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDoG(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDoGParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDoGParam>();

            m_pSpinVariance1 = addDoubleSpin(0, tr("1st variance"), m_pParam->m_variance1, 0, 5, 0.1);
            m_pSpinVariance2 = addDoubleSpin(1, tr("2nd variance"), m_pParam->m_variance2, 0, 5, 0.1);
            m_pSpinThreshold = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold, 0, 49, 0.5);
            m_pCheckNegative = addCheck(3, tr("Negative colors"), m_pParam->m_bNegativeColors);
            m_pCheckMonochrome = addCheck(4, tr("Monochrome"), m_pParam->m_bMonochrome);
        }

        void onApply() override
        {
            m_pParam->m_variance1 = m_pSpinVariance1->value();
            m_pParam->m_variance2 = m_pSpinVariance2->value();
            m_pParam->m_threshold = m_pSpinThreshold->value();
            m_pParam->m_bNegativeColors = m_pCheckNegative->isChecked();
            m_pParam->m_bMonochrome = m_pCheckMonochrome->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicDoGParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinVariance1 = nullptr;
        QDoubleSpinBox* m_pSpinVariance2 = nullptr;
        QDoubleSpinBox* m_pSpinThreshold = nullptr;
        QCheckBox*      m_pCheckNegative = nullptr;
        QCheckBox*      m_pCheckMonochrome = nullptr;
};

class CGmicWidgetDoGFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDoGFactory()
        {
            m_name = QObject::tr("Difference of Gaussians").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDoG>(pParam);
        }
};

#endif // CGMICWIDGETDOG_HPP
