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

#ifndef CGRAPHICSLAYER_H
#define CGRAPHICSLAYER_H

#include <QGraphicsItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//----------------------
//- Class CGraphicsLayer
//----------------------
class CORESHARED_EXPORT CGraphicsLayer : public QGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::LAYER) };

        CGraphicsLayer(const QString& name, CGraphicsLayer* pParent = nullptr);
        CGraphicsLayer(const QString& name, bool bUserLayer, CGraphicsLayer* pParent = nullptr);
        CGraphicsLayer(const CGraphicsLayer& layer);
        ~CGraphicsLayer();

        CGraphicsLayer&         operator=(const CGraphicsLayer& layer);

        void                    setName(const QString& name);
        void                    setRoot(bool bRoot);

        int                     type() const override;
        QRectF                  boundingRect() const override;
        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        QString                 getName() const;
        size_t                  getChildLayerCount() const;
        QList<CGraphicsLayer*>  getChildLayers() const;
        size_t                  getChildItemCount() const;
        QList<QGraphicsItem*>   getChildItems() const;

        bool                    isRoot() const;
        bool                    isUserLayer() const;

        void                    deleteChildItems();

    private:

        void                    copy(const CGraphicsLayer& layer);

    private:

        QString         m_name;
        bool            m_bRoot = false;
        bool            m_bUserLayer = false;
};

/** @endcond */

#endif // CGRAPHICSLAYER_H
