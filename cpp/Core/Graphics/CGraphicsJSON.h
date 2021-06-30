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

#ifndef CGRAPHICSJSON_H
#define CGRAPHICSJSON_H

#include <QByteArray>
#include <QJsonObject>
#include <QPen>
#include "Main/CoreGlobal.hpp"
#include "GraphicsProperty.hpp"

/** @cond INTERNAL */

class QGraphicsItem;
class CGraphicsPoint;
class CGraphicsEllipse;
class CGraphicsRectangle;
class CGraphicsPolygon;
class CGraphicsPolyline;
class CGraphicsComplexPolygon;
class CGraphicsText;

class CORESHARED_EXPORT CGraphicsJSON
{
    public:

        CGraphicsJSON();

        static QByteArray  getBinaryData(const QGraphicsItem* pItem);
        static QByteArray  getBinaryData(const CGraphicsPoint* pItem);
        static QByteArray  getBinaryData(const CGraphicsEllipse* pItem);
        static QByteArray  getBinaryData(const CGraphicsRectangle* pItem);
        static QByteArray  getBinaryData(const CGraphicsPolygon* pItem);
        static QByteArray  getBinaryData(const CGraphicsPolyline* pItem);
        static QByteArray  getBinaryData(const CGraphicsComplexPolygon* pItem);
        static QByteArray  getBinaryData(const CGraphicsText* pItem);

        static QJsonObject toJsonObject(const QPen& pen);
        static QJsonObject toJsonObject(const QBrush& brush);
        static QJsonObject toJsonObject(const QColor& color);
        static QJsonObject toJsonObject(const QFont& font);
        static QJsonObject toJsonObject(const CColor& color);
        static QJsonObject toJsonObject(const CGraphicsPointProperty& prop);
        static QJsonObject toJsonObject(const CGraphicsRectProperty& prop);
        static QJsonObject toJsonObject(const CGraphicsEllipseProperty& prop);
        static QJsonObject toJsonObject(const CGraphicsPolygonProperty& prop);
        static QJsonObject toJsonObject(const CGraphicsPolylineProperty& prop);
        static QJsonObject toJsonObject(const CGraphicsTextProperty& prop);

        static void        buildObject(QGraphicsItem* pItem, QByteArray data);
        static void        buildObject(CGraphicsPoint* pItem, QByteArray data);
        static void        buildObject(CGraphicsEllipse* pItem, QByteArray data);
        static void        buildObject(CGraphicsRectangle* pItem, QByteArray data);
        static void        buildObject(CGraphicsPolygon* pItem, QByteArray data);
        static void        buildObject(CGraphicsPolyline* pItem, QByteArray data);
        static void        buildObject(CGraphicsComplexPolygon* pItem, QByteArray data);
        static void        buildObject(CGraphicsText* pItem, QByteArray data);

        static void        buildObject(QColor& color, const QJsonObject& json);
        static void        buildObject(QPen& pen, const QJsonObject& json);
        static void        buildObject(QBrush& brush, const QJsonObject& json);
        static void        buildObject(QFont& font, const QJsonObject& json);
};

/** @endcond */

#endif // CGRAPHICSJSON_H
