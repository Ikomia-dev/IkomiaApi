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

#ifndef COCVWIDGETCROP_HPP
#define COCVWIDGETCROP_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvCrop.hpp"
#include <QSpinBox>

class COcvWidgetCrop : public CWorkflowTaskWidget
{
    public:

        COcvWidgetCrop(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCrop(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCropParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCropParam>();

            auto pLabelLeft = new QLabel(tr("Left"));
            m_pSpinLeft = new QSpinBox;
            m_pSpinLeft->setRange(0, 10000);
            m_pSpinLeft->setSingleStep(1);
            m_pSpinLeft->setValue(m_pParam->m_cropRect.x);

            auto pLabelTop = new QLabel(tr("Top"));
            m_pSpinTop = new QSpinBox;
            m_pSpinTop->setRange(0, 10000);
            m_pSpinTop->setSingleStep(1);
            m_pSpinTop->setValue(m_pParam->m_cropRect.y);

            auto pLabelWidth = new QLabel(tr("Width"));
            m_pSpinWidth = new QSpinBox;
            m_pSpinWidth->setRange(0, 10000);
            m_pSpinWidth->setSingleStep(1);
            m_pSpinWidth->setValue(m_pParam->m_cropRect.width);

            auto pLabelHeight = new QLabel(tr("Height"));
            m_pSpinHeight = new QSpinBox;
            m_pSpinHeight->setRange(0, 10000);
            m_pSpinHeight->setSingleStep(1);
            m_pSpinHeight->setValue(m_pParam->m_cropRect.height);
            
            m_pLayout->addWidget(pLabelLeft, 0, 0);
            m_pLayout->addWidget(m_pSpinLeft, 0, 1);
            m_pLayout->addWidget(pLabelTop, 1, 0);
            m_pLayout->addWidget(m_pSpinTop, 1, 1);
            m_pLayout->addWidget(pLabelWidth, 2, 0);
            m_pLayout->addWidget(m_pSpinWidth, 2, 1);
            m_pLayout->addWidget(pLabelHeight, 3, 0);
            m_pLayout->addWidget(m_pSpinHeight, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_cropRect.x = m_pSpinLeft->value();
            m_pParam->m_cropRect.y = m_pSpinTop->value();
            m_pParam->m_cropRect.width = m_pSpinWidth->value();
            m_pParam->m_cropRect.height = m_pSpinHeight->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvCropParam>  m_pParam = nullptr;
        QSpinBox*                       m_pSpinLeft = nullptr;
        QSpinBox*                       m_pSpinTop = nullptr;
        QSpinBox*                       m_pSpinWidth = nullptr;
        QSpinBox*                       m_pSpinHeight = nullptr;
};

class COcvWidgetCropFactory : public CWidgetFactory
{
    public:

        COcvWidgetCropFactory()
        {
            m_name = "ocv_crop";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCrop>(pParam);
        }
};

#endif // COCVWIDGETCROP_HPP
