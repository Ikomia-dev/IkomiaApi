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

#ifndef CGRAPHICSPOLYGON_H
#define CGRAPHICSPOLYGON_H

#include "Main/CoreDefine.hpp"
#include <QGraphicsPolygonItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPolygon
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPolygon: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPolygon();
        CProxyGraphicsPolygon(const std::vector<CPointF>& points);
        CProxyGraphicsPolygon(const std::vector<CPointF>& points, const CGraphicsPolygonProperty& property);

        void                        setPoints(const PolygonF& pts);
        void                        setProperty(const CGraphicsPolygonProperty& prop);
        void                        setCategory(const std::string& categ) override;

        PolygonF                    getPoints() const;
        QRectF                      getBoundingQRect() const override;
        std::vector<float>          getBoundingRect() const override;
        CGraphicsPolygonProperty    getProperty() const;
        std::string                 getCategory() const override;

        void                        insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary, bool bgr=false) const override;

        void                        translate(float dx, float dy) override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void                        toJson(QJsonObject& obj) const override;

    public:

        std::vector<CPointF>        m_points;
        CGraphicsPolygonProperty    m_property;
};

//------------------------
//- Class CGraphicsPolygon
//------------------------
class CORESHARED_EXPORT CGraphicsPolygon : public QGraphicsPolygonItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POLYGON) };

        CGraphicsPolygon(QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(const CGraphicsPolygonProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(size_t id, const CGraphicsPolygonProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolygon> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPolygon(const CGraphicsPolygon& polygon);

        CGraphicsPolygon&       operator=(const CGraphicsPolygon& polygon);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QRectF                  boundingRect() const override;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        void                    addPoint(QPointF pt);

        void                    updateLastPoint(const QPointF &pt);
        void                    updatePoint(int index, const QPointF &pt);

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);

        int                     hitVertex(const QPointF& pos);

        void                    copy(const CGraphicsPolygon& poly);

    private:

        int     m_hitVertexIndex = -1;
};

//----------------------------------------------------------------
//- Class CGraphicsPolygonFactory
//- Factory for dynamic CGraphicsPolygon instanciation (from type)
//----------------------------------------------------------------
class CGraphicsPolygonFactory: public CGraphicsFactory
{
    public:

        CGraphicsPolygonFactory()
        {
            m_type = CGraphicsPolygon::UserType + static_cast<size_t>(GraphicsItem::POLYGON);
        }

        ~CGraphicsPolygonFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPolygon(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSPOLYGON_H
