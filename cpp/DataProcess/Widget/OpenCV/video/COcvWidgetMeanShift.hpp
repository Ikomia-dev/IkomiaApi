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

#ifndef COCVWIDGETMEANSHIFT_HPP
#define COCVWIDGETMEANSHIFT_HPP

#include "Process/OpenCV/video/COcvMeanShift.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetMeanShift : public CWidgetApplyOnly
{
    public:

        COcvWidgetMeanShift(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetMeanShift(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMeanShiftParam>(pParam);
        }

    private:

        std::shared_ptr<COcvMeanShiftParam> m_pParam = nullptr;
};

class COcvWidgetMeanShiftFactory : public CWidgetFactory
{
    public:

        COcvWidgetMeanShiftFactory()
        {
            m_name = "ocv_mean_shift";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMeanShift>(pParam);
        }
};

#endif // COCVWIDGETMEANSHIFT_HPP
