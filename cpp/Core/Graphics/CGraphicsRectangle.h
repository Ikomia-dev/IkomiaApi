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

#ifndef CGRAPHICSRECTANGLE_H
#define CGRAPHICSRECTANGLE_H

#include <QGraphicsRectItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsEllipse
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsRect: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsRect();
        CProxyGraphicsRect(float x, float y, float w, float h);
        CProxyGraphicsRect(float x, float y, float w, float h, const CGraphicsRectProperty& property);

        //Accessors are for Python binding
        void                    setX(float x);
        void                    setY(float y);
        void                    setWidth(float w);
        void                    setHeight(float h);
        void                    setProperty(const CGraphicsRectProperty& prop);
        void                    setCategory(const std::string& category) override;

        float                   getX() const;
        float                   getY() const;
        float                   getWidth() const;
        float                   getHeight() const;
        QRectF                  getBoundingQRect() const override;
        std::vector<float>      getBoundingRect() const override;
        CGraphicsRectProperty   getProperty() const;
        std::string             getCategory() const override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        void                    translate(float dx, float dy) override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void                    toJson(QJsonObject &obj) const override;

    public:

        float                   m_x = 0.0;
        float                   m_y = 0.0;
        float                   m_width = 0.0;
        float                   m_height = 0.0;
        CGraphicsRectProperty   m_property;
};

//--------------------------
//- Class CGraphicsRectangle
//--------------------------
class CORESHARED_EXPORT CGraphicsRectangle : public QGraphicsRectItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::RECTANGLE) };

        CGraphicsRectangle(QGraphicsItem* pParent=nullptr);
        CGraphicsRectangle(const CGraphicsRectProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsRectangle(size_t id, const CGraphicsRectProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsRectangle(const cv::Rect& rect, QGraphicsItem* pParent=nullptr);
        CGraphicsRectangle(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsRect> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsRectangle(const CGraphicsRectangle& rectangle);

        CGraphicsRectangle&     operator=(const CGraphicsRectangle& rectangle);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QRectF                  boundingRect() const override;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);
        void                    updateBorder(qreal dx, qreal dy);

        int                     hitVertex(const QPointF& pos);

        void                    switchHitVertex(bool bHorizontal, bool bVertical);

        void                    copy(const CGraphicsRectangle& rectangle);

    private:

        int     m_hitVertexIndex = -1;
        QPointF m_clickPt;
};

//----------------------------------------------------------------
//- Class CGraphicsRectangleFactory
//- Factory for dynamic CGraphicsEllipse instanciation (from type)
//----------------------------------------------------------------
class CGraphicsRectangleFactory: public CGraphicsFactory
{
    public:

        CGraphicsRectangleFactory()
        {
            m_type = CGraphicsRectangle::UserType + static_cast<size_t>(GraphicsItem::RECTANGLE);
        }

        ~CGraphicsRectangleFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsRectangle(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSRECTANGLE_H
