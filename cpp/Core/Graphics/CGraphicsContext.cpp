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

CGraphicsPointProperty* CGraphicsContext::getPointPropertyPtr()
{
    return &m_pointProperty;
}

CGraphicsPointProperty CGraphicsContext::getPointProperty() const
{
    return m_pointProperty;
}

CGraphicsEllipseProperty *CGraphicsContext::getEllipsePropertyPtr()
{
    return &m_ellipseProperty;
}

CGraphicsEllipseProperty CGraphicsContext::getEllipseProperty() const
{
    return m_ellipseProperty;
}

CGraphicsRectProperty *CGraphicsContext::getRectPropertyPtr()
{
    return &m_rectProperty;
}

CGraphicsRectProperty CGraphicsContext::getRectProperty() const
{
    return m_rectProperty;
}

CGraphicsPolygonProperty *CGraphicsContext::getPolygonPropertyPtr()
{
    return &m_polygonProperty;
}

CGraphicsPolygonProperty CGraphicsContext::getPolygonProperty() const
{
    return m_polygonProperty;
}

CGraphicsPolylineProperty *CGraphicsContext::getPolylinePropertyPtr()
{
    return &m_polylineProperty;
}

CGraphicsPolylineProperty CGraphicsContext::getPolylineProperty() const
{
    return m_polylineProperty;
}

CGraphicsTextProperty *CGraphicsContext::getTextPropertyPtr()
{
    return &m_textProperty;
}

CGraphicsTextProperty CGraphicsContext::getTextProperty() const
{
    return m_textProperty;
}
