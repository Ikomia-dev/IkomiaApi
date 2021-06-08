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

#ifndef COCVWIDGETFLIP_HPP
#define COCVWIDGETFLIP_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvFlip.hpp"
#include <QRadioButton>

class COcvWidgetFlip : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFlip(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFlip(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFlipParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFlipParam>();

            m_pVerticalBtn = new QRadioButton(tr("Vertical"));
            m_pVerticalBtn->setChecked(true);
            m_pHorizontalBtn = new QRadioButton(tr("Horizontal"));
            m_pBothBtn = new QRadioButton(tr("Both"));

            
            m_pLayout->addWidget(m_pVerticalBtn, 0, 0, 1, 1);
            m_pLayout->addWidget(m_pHorizontalBtn, 1, 0, 1, 1);
            m_pLayout->addWidget(m_pBothBtn, 2, 0, 1, 1);
            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pVerticalBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = 0;
            });
            connect(m_pHorizontalBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = 1;
            });
            connect(m_pBothBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = -1;
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFlipParam>  m_pParam = nullptr;
        QRadioButton*                   m_pVerticalBtn = nullptr;
        QRadioButton*                   m_pHorizontalBtn = nullptr;
        QRadioButton*                   m_pBothBtn = nullptr;
};

class COcvWidgetFlipFactory : public CWidgetFactory
{
    public:

        COcvWidgetFlipFactory()
        {
            m_name = QObject::tr("Flip").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFlip>(pParam);
        }
};

#endif // COCVWIDGETFLIP_HPP
