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
        GraphicsPointProperty*      getPointPropertyPtr();
        GraphicsPointProperty       getPointProperty() const;
        GraphicsEllipseProperty*    getEllipsePropertyPtr();
        GraphicsEllipseProperty     getEllipseProperty() const;
        GraphicsRectProperty*       getRectPropertyPtr();
        GraphicsRectProperty        getRectProperty() const;
        GraphicsPolygonProperty*    getPolygonPropertyPtr();
        GraphicsPolygonProperty     getPolygonProperty() const;
        GraphicsPolylineProperty*   getPolylinePropertyPtr();
        GraphicsPolylineProperty    getPolylineProperty() const;
        GraphicsTextProperty*       getTextPropertyPtr();
        GraphicsTextProperty        getTextProperty() const;

    private:

        GraphicsShape               m_tool = GraphicsShape::SELECTION;
        GraphicsPointProperty       m_pointProperty;
        GraphicsEllipseProperty     m_ellipseProperty;
        GraphicsRectProperty        m_rectProperty;
        GraphicsPolygonProperty     m_polygonProperty;
        GraphicsPolylineProperty    m_polylineProperty;
        GraphicsTextProperty        m_textProperty;
};

using GraphicsContextPtr = std::shared_ptr<CGraphicsContext>;

/** @endcond */

#endif // CGLOBALGRAPHICSCONTEXT_H
