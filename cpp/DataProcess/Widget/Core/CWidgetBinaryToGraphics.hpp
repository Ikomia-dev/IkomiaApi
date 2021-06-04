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

#ifndef CWIDGETBINARYTOGRAPHICS_HPP
#define CWIDGETBINARYTOGRAPHICS_HPP

#include "Widget/Core/CWidgetApplyOnly.hpp"
#include "Process/Core/CBinaryToGraphics.h"

class CWidgetBinaryToGraphics : public CWidgetApplyOnly
{
    public:

        CWidgetBinaryToGraphics(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        CWidgetBinaryToGraphics(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<CBinaryToGraphicsParam> m_pParam = nullptr;
};

class CWidgetBinaryToGraphicsFactory : public CWidgetFactory
{
    public:

        CWidgetBinaryToGraphicsFactory()
        {
            m_name = QObject::tr("Binary to graphics").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CWidgetBinaryToGraphics>(pParam);
        }
};

#endif // CWIDGETBINARYTOGRAPHICS_HPP
