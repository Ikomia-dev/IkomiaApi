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

#ifndef COCVWIDGETTRACKERGOTURN_HPP
#define COCVWIDGETTRACKERGOTURN_HPP
#include "Process/OpenCV/tracking/COcvTrackerGOTURN.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetTrackerGOTURN : public CWidgetApplyOnly
{
    public:

        COcvWidgetTrackerGOTURN(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetTrackerGOTURN(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvTrackerGOTURNParam> m_pParam = nullptr;
};

class COcvWidgetTrackerGOTURNFactory : public CWidgetFactory
{
    public:

        COcvWidgetTrackerGOTURNFactory()
        {
            m_name = QObject::tr("TrackerGOTURN").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTrackerGOTURN>(pParam);
        }
};
#endif // COCVWIDGETTRACKERGOTURN_HPP
