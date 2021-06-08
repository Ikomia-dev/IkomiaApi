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

#ifndef CGMICWIDGETHOTPIXELS_HPP
#define CGMICWIDGETHOTPIXELS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicHotPixels.hpp"

class CGmicWidgetHotPixels : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetHotPixels(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetHotPixels(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicHotPixelsParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicHotPixelsParam>();

            auto pSpinMaskSize = addSpin(0, tr("Mask size"), m_pParam->m_maskSize);
            auto pSpinThreshold = addSpin(1, tr("Threshold(%)"), m_pParam->m_threshold);

            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_maskSize = pSpinMaskSize->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicHotPixelsParam> m_pParam = nullptr;
};

class CGmicWidgetHotPixelsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetHotPixelsFactory()
        {
            m_name = QObject::tr("Hot pixels").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetHotPixels>(pParam);
        }
};

#endif // CGMICWIDGETHOTPIXELS_HPP
