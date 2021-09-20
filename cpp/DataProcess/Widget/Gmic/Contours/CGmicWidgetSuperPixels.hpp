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

#ifndef CGMICWIDGETSUPERPIXELS_HPP
#define CGMICWIDGETSUPERPIXELS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicSuperPixels.hpp"

class CGmicWidgetSuperPixels : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSuperPixels(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSuperPixels(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSuperPixelsParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSuperPixelsParam>();

            m_pSpinSize = addSpin(0, tr("Size"), m_pParam->m_size, 4, 64);
            m_pSpinRegularity = addSpin(1, tr("Regularity"), m_pParam->m_regularity, 0, 128);
            m_pSpinIteration = addSpin(2, tr("Iterations"), m_pParam->m_iterations, 1, 16);

            m_pComboColors = addCombo(3, tr("Colors"));
            m_pComboColors->addItem(tr("Average"), CGmicSuperPixelsParam::AVERAGE);
            m_pComboColors->addItem(tr("Random"), CGmicSuperPixelsParam::RANDOM);
            m_pComboColors->setCurrentIndex(m_pComboColors->findData(m_pParam->m_colors));

            m_pSpinOpacity = addDoubleSpin(4, tr("Border opacity"), m_pParam->m_borderOpacity, 0, 1, 0.1);
            m_pColorBtn = addColorButton(5, tr("Border color"), m_pParam->m_borderColor);
        }

        void onApply() override
        {
            m_pParam->m_size = m_pSpinSize->value();
            m_pParam->m_regularity = m_pSpinRegularity->value();
            m_pParam->m_iterations = m_pSpinIteration->value();
            m_pParam->m_colors = m_pComboColors->currentData().toInt();
            m_pParam->m_borderOpacity = m_pSpinOpacity->value();
            m_pParam->m_borderColor = m_pColorBtn->getColor();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicSuperPixelsParam> m_pParam = nullptr;
        QDoubleSpinBox*     m_pSpinOpacity = nullptr;
        QSpinBox*           m_pSpinSize = nullptr;
        QSpinBox*           m_pSpinRegularity = nullptr;
        QSpinBox*           m_pSpinIteration = nullptr;
        QComboBox*          m_pComboColors = nullptr;
        CColorPushButton*   m_pColorBtn = nullptr;
};

class CGmicWidgetSuperPixelsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSuperPixelsFactory()
        {
            m_name = "gmic_super_pixels";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSuperPixels>(pParam);
        }
};

#endif // CGMICWIDGETSUPERPIXELS_HPP
