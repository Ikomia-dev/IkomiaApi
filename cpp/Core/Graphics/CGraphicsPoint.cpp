// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "CGraphicsPoint.h"
#include <QGraphicsSceneMouseEvent>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//-----------------------------
//- Class CProxyGraphicsPoint
//-----------------------------
CProxyGraphicsPoint::CProxyGraphicsPoint()
    : CProxyGraphicsItem(GraphicsItem::POINT)
{
}

CProxyGraphicsPoint::CProxyGraphicsPoint(const CPointF &point)
    : CProxyGraphicsItem(GraphicsItem::POINT)
{
    m_point = point;
}

CProxyGraphicsPoint::CProxyGraphicsPoint(const CPointF &point, const CGraphicsPointProperty &property)
    : CProxyGraphicsItem(GraphicsItem::POINT)
{
    m_point = point;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsPoint::setPoint(const CPointF &pt)
{
    m_point = pt;
}

void CProxyGraphicsPoint::setProperty(const CGraphicsPointProperty &prop)
{
    m_property = prop;
}

CPointF CProxyGraphicsPoint::getPoint() const
{
    return m_point;
}

QRectF CProxyGraphicsPoint::getBoundingRect() const
{
    CGraphicsPointProperty prop;
    if(!m_bUseGlobalContext)
        prop = m_property;

    float radius = (float)prop.m_size / 2.0f;
    return QRectF(m_point.m_x - radius, m_point.m_y - radius, prop.m_size, prop.m_size);
}

CGraphicsPointProperty CProxyGraphicsPoint::getProperty() const
{
    return m_property;
}

void CProxyGraphicsPoint::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsPoint::clone() const
{
    return std::make_shared<CProxyGraphicsPoint>(*this);
}

void CProxyGraphicsPoint::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);
    obj["x"] = m_point.getX();
    obj["y"] = m_point.getY();
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//----------------------
//- Class CGraphicsPoint
//----------------------
CGraphicsPoint::CGraphicsPoint(QGraphicsItem* pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(GraphicsItem::POINT)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setPen(QPen(QColor(255,0,0)));
    setBrush(QBrush(QColor(255,0,0,10)));
    setRect(-1, 1, 3, 3);
}

CGraphicsPoint::CGraphicsPoint(const QPointF &position, const CGraphicsPointProperty &property, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(GraphicsItem::POINT)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setPen(QPen(Utils::Graphics::toQColor(property.m_penColor)));
    setBrush(QBrush(Utils::Graphics::toQColor(property.m_brushColor)));
    float radius = (float)property.m_size / 2.0f;
    setRect(position.x() - radius, position.y() - radius, property.m_size, property.m_size);
    setCategory(QString::fromStdString(property.m_category));
}

CGraphicsPoint::CGraphicsPoint(size_t id, const QPointF &position, const CGraphicsPointProperty &property, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(id, GraphicsItem::POINT)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setPen(QPen(Utils::Graphics::toQColor(property.m_penColor)));
    setBrush(QBrush(Utils::Graphics::toQColor(property.m_brushColor)));
    float radius = (float)property.m_size / 2.0f;
    setRect(position.x() - radius, position.y() - radius, property.m_size, property.m_size);
    setCategory(QString::fromStdString(property.m_category));
}

CGraphicsPoint::CGraphicsPoint(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPoint> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::POINT)
{
    CGraphicsPointProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getPointProperty();
    else
        prop = proxyItem->m_property;

    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setPen(QPen(Utils::Graphics::toQColor(prop.m_penColor)));
    setBrush(QBrush(Utils::Graphics::toQColor(prop.m_brushColor)));
    float radius = (float)prop.m_size / 2.0f;
    setRect(proxyItem->m_point.m_x - radius, proxyItem->m_point.m_y - radius, prop.m_size, prop.m_size);
    setCategory(QString::fromStdString(prop.m_category));
}

CGraphicsPoint::CGraphicsPoint(const CGraphicsPoint &point)
    : QGraphicsEllipseItem(point.parentItem()),
      CGraphicsItem(GraphicsItem::POINT)
{
    copy(point);
}

CGraphicsPoint &CGraphicsPoint::operator=(const CGraphicsPoint &point)
{
    copy(point);
    return *this;
}

QGraphicsItem *CGraphicsPoint::clone() const
{
    return new CGraphicsPoint(*this);
}

void CGraphicsPoint::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsPoint::setBrushColor(QColor color)
{
    setBrush(color);
}

int CGraphicsPoint::type() const
{
    return Type;
}

QRectF CGraphicsPoint::boundingRect() const
{
    return QGraphicsEllipseItem::boundingRect();
}

QPainterPath CGraphicsPoint::shape() const
{
    return QGraphicsEllipseItem::shape();
}

void CGraphicsPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void CGraphicsPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
}

void CGraphicsPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void CGraphicsPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(event->pos());
}

QByteArray CGraphicsPoint::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

ProxyGraphicsItemPtr CGraphicsPoint::createProxyGraphicsItem() const
{
    QRectF rc = mapRectToScene(rect());
    CGraphicsPointProperty prop;
    prop.m_size = rc.width();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_category = getCategory().toStdString();
    prop.m_brushColor = Utils::Graphics::toCColor(brush().color());

    float radius = rc.width() / 2.0f;
    CPointF pt(rc.x() + radius, rc.y() + radius);
    return std::make_shared<CProxyGraphicsPoint>(pt, prop);
}

void CGraphicsPoint::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsPoint::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CGraphicsPoint::updateCursor(const QPointF &pos)
{
    Q_UNUSED(pos);

    if(isSelected())
        setCursor(Qt::ClosedHandCursor);
    else
        setCursor(Qt::ArrowCursor);
}

void CGraphicsPoint::copy(const CGraphicsPoint &point)
{
    setRect(point.rect());
    setPen(point.pen());
    setBrush(point.brush());
    setCategory(point.getCategory());
    setParentItem(point.parentItem());
}
