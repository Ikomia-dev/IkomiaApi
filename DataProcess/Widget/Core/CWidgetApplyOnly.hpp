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

#ifndef CWIDGETAPPLYONLY_HPP
#define CWIDGETAPPLYONLY_HPP

#include "Protocol/CProtocolTaskWidget.h"

class CWidgetApplyOnly : public CProtocolTaskWidget
{
    public:

        CWidgetApplyOnly(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        CWidgetApplyOnly(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = pParam;
            init();
        }

    protected:

        void init() final
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CProtocolTaskParam>();

            // Remove scroll area widget from layout and delete it
            auto pLayout = dynamic_cast<QVBoxLayout*>(layout());
            if(pLayout != nullptr)
            {
                pLayout->removeWidget(m_pScrollArea);
                pLayout->insertStretch(0, 1);
                m_pScrollArea->deleteLater();
            }

            connect(m_pApplyBtn, &QPushButton::clicked, [&]{ emit doApplyProcess(m_pParam); } );
        }

    private:

        std::shared_ptr<CProtocolTaskParam> m_pParam = nullptr;
};

#endif // CWIDGETAPPLYONLY_HPP
