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

#ifndef COCVWIDGETSPLIT_HPP
#define COCVWIDGETSPLIT_HPP

#include "Process/OpenCV/core/COcvSplit.hpp"

class COcvWidgetSplit : public CProtocolTaskWidget
{
    public:

        COcvWidgetSplit(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetSplit(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSplitParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSplitParam>();

            auto pLabel = new QLabel(tr("Number of output images"));
            auto pSpin = new QSpinBox;

            pSpin->setMinimum(1);
            pSpin->setValue(m_pParam->m_outputCount);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_outputCount = pSpin->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pSpin, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvSplitParam> m_pParam = nullptr;
};

class COcvWidgetSplitFactory : public CWidgetFactory
{
    public:

        COcvWidgetSplitFactory()
        {
            m_name = QObject::tr("Split Operator").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSplit>(pParam);
        }
};

#endif // COCVWIDGETSPLIT_HPP
