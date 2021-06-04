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

#ifndef CGMICWIDGETREDEYE_HPP
#define CGMICWIDGETREDEYE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicRedEye.hpp"

class CGmicWidgetRedEye : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetRedEye(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetRedEye(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicRedEyeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicRedEyeParam>();

            auto pSpinThreshold = addSpin(0, tr("Threshold"), m_pParam->m_threshold, 0, 100, 1);
            auto pSpinSmoothness = addDoubleSpin(1, tr("Smoothness"), m_pParam->m_smoothness);
            auto pSpinAttenuation = addDoubleSpin(2, tr("Attenuation"), m_pParam->m_attenuation);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_smoothness = pSpinSmoothness->value();
                m_pParam->m_attenuation = pSpinAttenuation->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicRedEyeParam> m_pParam = nullptr;
};

class CGmicWidgetRedEyeFactory : public CWidgetFactory
{
    public:

        CGmicWidgetRedEyeFactory()
        {
            m_name = QObject::tr("Red-eye").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetRedEye>(pParam);
        }
};

#endif // CGMICWIDGETREDEYE_HPP
