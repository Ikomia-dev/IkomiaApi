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

#ifndef CGRAPHICSELLIPSE_H
#define CGRAPHICSELLIPSE_H

#include <QGraphicsEllipseItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsEllipse
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsEllipse: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsEllipse();
        CProxyGraphicsEllipse(float x, float y, float w, float h);
        CProxyGraphicsEllipse(float x, float y, float w, float h, const CGraphicsEllipseProperty& property);

        //Accessors are for Python binding
        void                    setX(float x);
        void                    setY(float y);
        void                    setWidth(float w);
        void                    setHeight(float h);
        void                    setProperty(const CGraphicsEllipseProperty& prop);

        float                   getX() const;
        float                   getY() const;
        float                   getWidth() const;
        float                   getHeight() const;
        QRectF                  getBoundingRect() const override;
        CGraphicsEllipseProperty getProperty() const;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void                    toJson(QJsonObject& obj) const override;

    public:

        float                   m_x = 0.0;
        float                   m_y = 0.0;
        float                   m_width = 0.0;
        float                   m_height = 0.0;
        CGraphicsEllipseProperty m_property;
};

//------------------------
//- Class CGraphicsEllipse
//------------------------
class CORESHARED_EXPORT CGraphicsEllipse : public QGraphicsEllipseItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::ELLIPSE) };

        CGraphicsEllipse(QGraphicsItem* pParent=nullptr);
        CGraphicsEllipse(const CGraphicsEllipseProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsEllipse(size_t id, const CGraphicsEllipseProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsEllipse(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsEllipse> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsEllipse(const CGraphicsEllipse& ellipse);

        CGraphicsEllipse&       operator=(const CGraphicsEllipse& ellipse);

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
        void                    updateBorder(qreal dx, qreal dy);

        int                     hitVertex(const QPointF &pos);

        void                    switchHitVertex(bool bHorizontal, bool bVertical);

        void                    copy(const CGraphicsEllipse& ellipse);

    private:

        int             m_hitVertexIndex = -1;
        QPointF         m_clickPt;
};

//----------------------------------------------------------------
//- Class CGraphicsEllipseFactory
//- Factory for dynamic CGraphicsEllipse instanciation (from type)
//----------------------------------------------------------------
class CGraphicsEllipseFactory: public CGraphicsFactory
{
    public:

        CGraphicsEllipseFactory()
        {
            m_type = CGraphicsEllipse::UserType + static_cast<size_t>(GraphicsItem::ELLIPSE);
        }

        ~CGraphicsEllipseFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsEllipse(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSELLIPSE_H
