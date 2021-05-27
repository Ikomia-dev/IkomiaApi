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

#ifndef CWIDGETCUT_HPP
#define CWIDGETCUT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CCut.hpp"

class CWidgetCut : public CProtocolTaskWidget
{
    public:

        CWidgetCut(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CWidgetCut(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CCutParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CCutParam>();

            auto pSpinSize = addSpin(0, tr("Size"), m_pParam->m_size);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_size = pSpinSize->value();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<CCutParam> m_pParam = nullptr;
};

class CWidgetCutFactory : public CWidgetFactory
{
    public:

        CWidgetCutFactory()
        {
            m_name = QObject::tr("Cut").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetCut>(pParam);
        }
};

#endif // CWIDGETCUT_HPP
