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

#ifndef CGRAPHICSLINE_H
#define CGRAPHICSLINE_H

#include "Main/CoreDefine.hpp"
#include <QGraphicsPathItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPolygon
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPolyline: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPolyline();
        CProxyGraphicsPolyline(const std::vector<CPointF>& points);
        CProxyGraphicsPolyline(const std::vector<CPointF>& points, const CGraphicsPolylineProperty& property);

        void                        setPoints(const PolygonF& pts);
        void                        setProperty(const CGraphicsPolylineProperty& prop);
        void                        setCategory(const std::string& categ) override;

        PolygonF                    getPoints() const;
        QRectF                      getBoundingQRect() const override;
        std::vector<float>          getBoundingRect() const override;
        CGraphicsPolylineProperty   getProperty();
        std::string                 getCategory() const override;

        void                        translate(float dx, float dy) override;

        void                        insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary, bool bgr=false) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void                        toJson(QJsonObject &obj) const override;

    public:

        std::vector<CPointF>        m_points;
        CGraphicsPolylineProperty   m_property;
};

//-------------------------
//- Class CGraphicsPolyline
//-------------------------
class CORESHARED_EXPORT CGraphicsPolyline : public QGraphicsPathItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POLYLINE) };

        CGraphicsPolyline(QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(const CGraphicsPolylineProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(size_t id, const CGraphicsPolylineProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolyline> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPolyline(const CGraphicsPolyline& polyline);

        CGraphicsPolyline&      operator=(const CGraphicsPolyline& polyline);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QRectF                  boundingRect() const override;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        QPolygonF               getPolygon() const;
        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        void                    setPolygon(QPolygonF& poly);

        void                    addPoint(QPointF pt);

        void                    updateLastPoint(QPointF pt);
        void                    updatePoint(int index, const QPointF &pt);

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);
        int                     hitVertex(const QPointF& pos);
        void                    copy(const CGraphicsPolyline& polyline);

    private:

        QPolygonF   m_polygon;
        int         m_hitVertexIndex = -1;
};

//-----------------------------------------------------------------
//- Class CGraphicsPolylineFactory
//- Factory for dynamic CGraphicsPolyline instanciation (from type)
//-----------------------------------------------------------------
class CGraphicsPolylineFactory: public CGraphicsFactory
{
    public:

        CGraphicsPolylineFactory()
        {
            m_type = CGraphicsPolyline::UserType + static_cast<size_t>(GraphicsItem::POLYLINE);
        }

        ~CGraphicsPolylineFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPolyline(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSLINE_H
