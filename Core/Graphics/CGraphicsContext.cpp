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

#include "CGraphicsContext.h"

CGraphicsContext::CGraphicsContext()
{
}

void CGraphicsContext::setTool(GraphicsShape tool)
{
    m_tool = tool;
}

void CGraphicsContext::setCurrentCategory(const std::string &category)
{
    m_pointProperty.m_category = category;
    m_ellipseProperty.m_category = category;
    m_rectProperty.m_category = category;
    m_polygonProperty.m_category = category;
    m_polylineProperty.m_category = category;
}

GraphicsShape CGraphicsContext::getTool() const
{
    return m_tool;
}

GraphicsPointProperty* CGraphicsContext::getPointPropertyPtr()
{
    return &m_pointProperty;
}

GraphicsPointProperty CGraphicsContext::getPointProperty() const
{
    return m_pointProperty;
}

GraphicsEllipseProperty *CGraphicsContext::getEllipsePropertyPtr()
{
    return &m_ellipseProperty;
}

GraphicsEllipseProperty CGraphicsContext::getEllipseProperty() const
{
    return m_ellipseProperty;
}

GraphicsRectProperty *CGraphicsContext::getRectPropertyPtr()
{
    return &m_rectProperty;
}

GraphicsRectProperty CGraphicsContext::getRectProperty() const
{
    return m_rectProperty;
}

GraphicsPolygonProperty *CGraphicsContext::getPolygonPropertyPtr()
{
    return &m_polygonProperty;
}

GraphicsPolygonProperty CGraphicsContext::getPolygonProperty() const
{
    return m_polygonProperty;
}

GraphicsPolylineProperty *CGraphicsContext::getPolylinePropertyPtr()
{
    return &m_polylineProperty;
}

GraphicsPolylineProperty CGraphicsContext::getPolylineProperty() const
{
    return m_polylineProperty;
}

GraphicsTextProperty *CGraphicsContext::getTextPropertyPtr()
{
    return &m_textProperty;
}

GraphicsTextProperty CGraphicsContext::getTextProperty() const
{
    return m_textProperty;
}
