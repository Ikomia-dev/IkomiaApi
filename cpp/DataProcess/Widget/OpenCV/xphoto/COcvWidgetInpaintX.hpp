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

#ifndef COCVWIDGETINPAINTX_HPP
#define COCVWIDGETINPAINTX_HPP

#include "Process/OpenCV/xphoto/COcvInpaintX.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetInpaintX : public CWidgetApplyOnly
{
    public:

        COcvWidgetInpaintX(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetInpaintX(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetInpaintXFactory : public CWidgetFactory
{
    public:

        COcvWidgetInpaintXFactory()
        {
            m_name = "ocv_xphoto_inpaint";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInpaintX>(pParam);
        }
};

#endif // COCVWIDGETINPAINTX_HPP
