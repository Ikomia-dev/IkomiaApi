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

#ifndef COCVWIDGETMERGE_HPP
#define COCVWIDGETMERGE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMerge.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetMerge : public CProtocolTaskWidget
{
    public:

        COcvWidgetMerge(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetMerge(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMergeParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMergeParam>();

            auto pLabel = new QLabel(tr("Number of input images"));
            auto pSpin = new QSpinBox;

            pSpin->setMinimum(1);
            pSpin->setValue(m_pParam->m_inputCount);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_inputCount = pSpin->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pSpin, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvMergeParam> m_pParam = nullptr;
};

class COcvWidgetMergeFactory : public CWidgetFactory
{
    public:

        COcvWidgetMergeFactory()
        {
            m_name = QObject::tr("Merge Operator").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetMerge>(pParam);
        }
};

#endif // COCVWIDGETMERGE_HPP
