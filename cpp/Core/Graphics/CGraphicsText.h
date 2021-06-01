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

#ifndef CGRAPHICSTEXT_H
#define CGRAPHICSTEXT_H

#include <QGraphicsTextItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsText
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsText: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsText();
        CProxyGraphicsText(const std::string& text);
        CProxyGraphicsText(const std::string& text, float x, float y);
        CProxyGraphicsText(const std::string& text, float x, float y, const GraphicsTextProperty& property);

        void            setX(float x);
        void            setY(float x);
        void            setText(const std::string& text);

        float           getX() const;
        float           getY() const;
        std::string     getText() const;
        QRectF          getBoundingRect() const override;

        bool            isTextItem() const override;

        void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void            toJson(QJsonObject &obj) const override;

    public:

        float                   m_x = 0;
        float                   m_y = 0;
        std::string             m_text;
        GraphicsTextProperty    m_property;
};

//---------------------
//- Class CGraphicsText
//---------------------
class CORESHARED_EXPORT CGraphicsText : public QGraphicsTextItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::TEXT) };

        CGraphicsText(QGraphicsItem* pParent=nullptr);
        CGraphicsText(const QString& text, QGraphicsItem* pParent=nullptr);
        CGraphicsText(const QString& text, const GraphicsTextProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsText(const QString& text, const GraphicsTextProperty& property, QPointF position, QGraphicsItem* pParent=nullptr);
        CGraphicsText(size_t id, const QString& text, const GraphicsTextProperty& property, QPointF position, QGraphicsItem* pParent=nullptr);
        CGraphicsText(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsText> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsText(const CGraphicsText& text);

        CGraphicsText&          operator=(const CGraphicsText& text);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        virtual void            paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        static int              pixelToPointSize(int size);

    private:

        void                    copy(const CGraphicsText& text);
};

//-------------------------------------------------------------
//- Class CGraphicsTextFactory
//- Factory for dynamic CGraphicsText instanciation (from type)
//-------------------------------------------------------------
class CGraphicsTextFactory: public CGraphicsFactory
{
    public:

        CGraphicsTextFactory()
        {
            m_type = CGraphicsText::UserType + static_cast<size_t>(GraphicsItem::TEXT);
        }

        ~CGraphicsTextFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsText(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSTEXT_H
