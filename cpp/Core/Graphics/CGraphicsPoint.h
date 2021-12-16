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

#ifndef CGRAPHICSPOINT_H
#define CGRAPHICSPOINT_H

#include "Main/CoreDefine.hpp"
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPoint
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPoint: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPoint();
        CProxyGraphicsPoint(const CPointF& point);
        CProxyGraphicsPoint(const CPointF& point, const CGraphicsPointProperty& property);

        void                    setPoint(const CPointF& pt);
        void                    setProperty(const CGraphicsPointProperty& prop);
        void                    setCategory(const std::string& categ) override;

        CPointF                 getPoint() const;
        QRectF                  getBoundingQRect() const override;
        std::vector<float>      getBoundingRect() const override;
        CGraphicsPointProperty  getProperty() const;
        std::string             getCategory() const override;

        void                    translate(float dx, float dy);

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void                    toJson(QJsonObject &obj) const override;

    public:

        CPointF                 m_point;
        CGraphicsPointProperty  m_property;
};

//----------------------
//- Class CGraphicsPoint
//----------------------
class CORESHARED_EXPORT CGraphicsPoint: public QGraphicsEllipseItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POINT) };

        CGraphicsPoint(QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(const QPointF& position, const CGraphicsPointProperty& property , QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(size_t id, const QPointF& position, const CGraphicsPointProperty& property , QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPoint> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPoint(const CGraphicsPoint& point);

        CGraphicsPoint&         operator=(const CGraphicsPoint& point);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;
        QRectF                  boundingRect() const override;
        QPainterPath            shape() const override;
        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);

        void                    copy(const CGraphicsPoint& point);
};

//--------------------------------------------------------------
//- Class CGraphicsPointFactory
//- Factory for dynamic CGraphicsPoint instanciation (from type)
//--------------------------------------------------------------
class CGraphicsPointFactory: public CGraphicsFactory
{
    public:

        CGraphicsPointFactory()
        {
            m_type = CGraphicsPoint::UserType + static_cast<size_t>(GraphicsItem::POINT);
        }

        ~CGraphicsPointFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPoint(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSPOINT_H
