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

#ifndef COCVWIDGETCAMSHIFT_HPP
#define COCVWIDGETCAMSHIFT_HPP

#include "Process/OpenCV/video/COcvCamShift.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetCamShift : public CWidgetApplyOnly
{
    public:

        COcvWidgetCamShift(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetCamShift(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCamShiftParam>(pParam);
        }

    private:

        std::shared_ptr<COcvCamShiftParam> m_pParam = nullptr;
};

class COcvWidgetCamShiftFactory : public CWidgetFactory
{
    public:

        COcvWidgetCamShiftFactory()
        {
            m_name = QObject::tr("CamShift").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCamShift>(pParam);
        }
};

#endif // COCVWIDGETCAMSHIFT_HPP
