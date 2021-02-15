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

class CGmicWidgetSuperPixels : public CProtocolTaskWidget
{
    public:

        CGmicWidgetSuperPixels(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSuperPixels(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSuperPixelsParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSuperPixelsParam>();

            auto pSpinSize = addSpin(0, tr("Size"), m_pParam->m_size, 4, 64);
            auto pSpinRegularity = addSpin(1, tr("Regularity"), m_pParam->m_regularity, 0, 128);
            auto pSpinIteration = addSpin(2, tr("Iterations"), m_pParam->m_iterations, 1, 16);

            auto pComboColors = addCombo(3, tr("Colors"));
            pComboColors->addItem(tr("Average"), CGmicSuperPixelsParam::AVERAGE);
            pComboColors->addItem(tr("Random"), CGmicSuperPixelsParam::RANDOM);
            pComboColors->setCurrentIndex(pComboColors->findData(m_pParam->m_colors));

            auto pSpinOpacity = addDoubleSpin(4, tr("Border opacity"), m_pParam->m_borderOpacity, 0, 1, 0.1);
            auto pColorBtn = addColorButton(5, tr("Border color"), m_pParam->m_borderColor);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_size = pSpinSize->value();
                m_pParam->m_regularity = pSpinRegularity->value();
                m_pParam->m_iterations = pSpinIteration->value();
                m_pParam->m_colors = pComboColors->currentData().toInt();
                m_pParam->m_borderOpacity = pSpinOpacity->value();
                m_pParam->m_borderColor = pColorBtn->getColor();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSuperPixelsParam> m_pParam = nullptr;
};

class CGmicWidgetSuperPixelsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSuperPixelsFactory()
        {
            m_name = QObject::tr("Super-pixels").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSuperPixels>(pParam);
        }
};

#endif // CGMICWIDGETSUPERPIXELS_HPP
