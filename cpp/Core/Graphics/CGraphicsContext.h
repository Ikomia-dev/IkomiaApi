/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CGLOBALGRAPHICSCONTEXT_H
#define CGLOBALGRAPHICSCONTEXT_H

#include <memory>
#include "Main/CoreGlobal.hpp"
#include "Graphics/GraphicsProperty.hpp"

/** @cond INTERNAL */

class CORESHARED_EXPORT CGraphicsContext
{
    public:

        CGraphicsContext();

        void                        setTool(GraphicsShape tool);
        void                        setCurrentCategory(const std::string &category);

        GraphicsShape               getTool() const;
        CGraphicsPointProperty*      getPointPropertyPtr();
        CGraphicsPointProperty       getPointProperty() const;
        CGraphicsEllipseProperty*    getEllipsePropertyPtr();
        CGraphicsEllipseProperty     getEllipseProperty() const;
        CGraphicsRectProperty*       getRectPropertyPtr();
        CGraphicsRectProperty        getRectProperty() const;
        CGraphicsPolygonProperty*    getPolygonPropertyPtr();
        CGraphicsPolygonProperty     getPolygonProperty() const;
        CGraphicsPolylineProperty*   getPolylinePropertyPtr();
        CGraphicsPolylineProperty    getPolylineProperty() const;
        CGraphicsTextProperty*       getTextPropertyPtr();
        CGraphicsTextProperty        getTextProperty() const;

    private:

        GraphicsShape               m_tool = GraphicsShape::SELECTION;
        CGraphicsPointProperty      m_pointProperty;
        CGraphicsEllipseProperty    m_ellipseProperty;
        CGraphicsRectProperty       m_rectProperty;
        CGraphicsPolygonProperty    m_polygonProperty;
        CGraphicsPolylineProperty   m_polylineProperty;
        CGraphicsTextProperty       m_textProperty;
};

using GraphicsContextPtr = std::shared_ptr<CGraphicsContext>;

/** @endcond */

#endif // CGLOBALGRAPHICSCONTEXT_H
