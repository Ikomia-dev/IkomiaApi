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

#ifndef COCVWIDGETMAX_HPP
#define COCVWIDGETMAX_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMax.hpp"

class COcvWidgetMax : public CProtocolTaskWidget
{
    public:

        COcvWidgetMax(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMax(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMaxParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMaxParam>();

            auto pCheckScalar = addCheck(0, tr("Scalar operation"), m_pParam->m_bScalar);
            auto pSpinScalar = addDoubleSpin(1, tr("Scalar value"), m_pParam->m_scalar[0]);
            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bScalar = pCheckScalar->isChecked();
                m_pParam->m_scalar= cv::Scalar::all(pSpinScalar->value());
                emit doApplyProcess(m_pParam);
            } );
        }

    private:

        std::shared_ptr<COcvMaxParam> m_pParam = nullptr;
};

class COcvWidgetMaxFactory : public CWidgetFactory
{
    public:

        COcvWidgetMaxFactory()
        {
            m_name = QObject::tr("Max").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMax>(pParam);
        }
};

#endif // COCVWIDGETMAXHPP_H
