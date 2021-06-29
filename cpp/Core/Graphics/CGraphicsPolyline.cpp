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

#include "CGraphicsPolyline.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//------------------------------
//- Class CProxyGraphicsPolyline
//------------------------------
CProxyGraphicsPolyline::CProxyGraphicsPolyline()
    : CProxyGraphicsItem(GraphicsItem::POLYLINE)
{
}

CProxyGraphicsPolyline::CProxyGraphicsPolyline(const std::vector<CPointF> &points)
    : CProxyGraphicsItem(GraphicsItem::POLYLINE)
{
    m_points = points;
}

CProxyGraphicsPolyline::CProxyGraphicsPolyline(const std::vector<CPointF> &points, const GraphicsPolylineProperty &property)
    : CProxyGraphicsItem(GraphicsItem::POLYLINE)
{
    m_points = points;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsPolyline::setPoints(const PolygonF &pts)
{
    m_points = pts;
}

void CProxyGraphicsPolyline::setProperty(const GraphicsPolylineProperty &prop)
{
    m_property = prop;
}

PolygonF CProxyGraphicsPolyline::getPoints() const
{
    return m_points;
}

QRectF CProxyGraphicsPolyline::getBoundingRect() const
{
    float xmin = FLT_MAX;
    float ymin = FLT_MAX;
    float xmax = FLT_MIN;
    float ymax = FLT_MIN;

    for(size_t i=0; i<m_points.size(); ++i)
    {
        xmin = std::min(xmin, m_points[i].m_x);
        ymin = std::min(ymin, m_points[i].m_y);
        xmax = std::max(xmax, m_points[i].m_x);
        ymax = std::max(ymax, m_points[i].m_y);
    }
    return QRectF(xmin, ymin, (xmax - xmin) + 1, (ymax - ymin) + 1);
}

GraphicsPolylineProperty CProxyGraphicsPolyline::getProperty()
{
    return m_property;
}

void CProxyGraphicsPolyline::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsPolyline::clone() const
{
    return std::make_shared<CProxyGraphicsPolyline>(*this);
}

void CProxyGraphicsPolyline::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);

    QJsonArray pointArray;
    for(size_t i=0; i<m_points.size(); ++i)
    {
        QJsonObject point;
        point["x"] = m_points[i].getX();
        point["y"] = m_points[i].getY();
        pointArray.append(point);
    }
    obj["polyline"] = pointArray;
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//-------------------------
//- Class CGraphicsPolyline
//-------------------------
CGraphicsPolyline::CGraphicsPolyline(QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(GraphicsItem::POLYLINE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);

    QPen pen(QColor(255,0,0));
    pen.setCosmetic(true);
    setPen(pen);
}

CGraphicsPolyline::CGraphicsPolyline(const GraphicsPolylineProperty &property, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(GraphicsItem::POLYLINE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    setPen(pen);
}

CGraphicsPolyline::CGraphicsPolyline(size_t id, const GraphicsPolylineProperty &property, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(id, GraphicsItem::POLYLINE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    setPen(pen);
}

CGraphicsPolyline::CGraphicsPolyline(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolyline> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::POLYLINE)
{
    GraphicsPolylineProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getPolylineProperty();
    else
        prop = proxyItem->m_property;

    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(prop.m_category));

    QPen pen(Utils::Graphics::toQColor(prop.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(prop.m_lineSize);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    setPen(pen);

    QPolygonF poly;
    for(size_t i=0; i<proxyItem->m_points.size(); ++i)
        poly.push_back(QPointF(proxyItem->m_points[i].m_x, proxyItem->m_points[i].m_y));

    setPolygon(poly);
}

CGraphicsPolyline::CGraphicsPolyline(const CGraphicsPolyline &polyline)
    : QGraphicsPathItem(polyline.parentItem()),
      CGraphicsItem(GraphicsItem::POLYLINE)
{
    copy(polyline);
}

CGraphicsPolyline &CGraphicsPolyline::operator=(const CGraphicsPolyline &polyline)
{
    copy(polyline);
    return *this;
}

QGraphicsItem *CGraphicsPolyline::clone() const
{
    return new CGraphicsPolyline(*this);
}

void CGraphicsPolyline::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsPolyline::setBrushColor(QColor color)
{
    Q_UNUSED(color);
}

int CGraphicsPolyline::type() const
{
    return Type;
}

QRectF CGraphicsPolyline::boundingRect() const
{
    if(isSelected())
    {
        //Add vertex size margins
        QRectF rect = QGraphicsPathItem::boundingRect();
        rect = rect.marginsAdded(QMarginsF() + Utils::Graphics::getSelectionMargin(pen().widthF()));
        return rect;
    }
    else
        return QGraphicsPathItem::boundingRect();
}

void CGraphicsPolyline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    if(isSelected())
    {
        //Call parent polygon paint without selected flag
        QStyleOptionGraphicsItem myOption(*option);
        myOption.state &= ~QStyle::State_Selected;
        QGraphicsPathItem::paint(painter, &myOption, widget);

        //Highlight polygon vertex
        for(auto it : m_polygon)
            Utils::Graphics::paintSelectedVertex(painter, it, pen().widthF());

        //Update bound rect to avoid paint artefacts
        prepareGeometryChange();
    }
    else
        QGraphicsPathItem::paint(painter, option, widget);
}

void CGraphicsPolyline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hitVertexIndex != -1)
        updatePoint(m_hitVertexIndex, event->pos());
    else
        QGraphicsPathItem::mouseMoveEvent(event);
}

void CGraphicsPolyline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(event->pos());
}

QPolygonF CGraphicsPolyline::getPolygon() const
{
    return m_polygon;
}

void CGraphicsPolyline::addPoint(QPointF pt)
{
    if(m_polygon.size() == 0)
        m_polygon.push_back(pt);

    m_polygon.push_back(pt);
    QPainterPath path;
    path.addPolygon(m_polygon);
    setPath(path);
}

void CGraphicsPolyline::updateLastPoint(QPointF pt)
{
    m_polygon.last() = pt;
    QPainterPath path;
    path.addPolygon(m_polygon);
    setPath(path);
}

void CGraphicsPolyline::updatePoint(int index, const QPointF &pt)
{
    if(index >= 0 && index < m_polygon.size())
    {
        m_polygon[index] = pt;
        QPainterPath path;
        path.addPolygon(m_polygon);
        setPath(path);
    }
}

ProxyGraphicsItemPtr CGraphicsPolyline::createProxyGraphicsItem() const
{
    GraphicsPolylineProperty prop;
    prop.m_category = getCategory().toStdString();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_lineSize = pen().width();

    QPolygonF poly = mapToScene(getPolygon());
    std::vector<CPointF> points;

    for(int i=0; i<poly.size(); ++i)
        points.push_back(CPointF(poly[i].x(), poly[i].y()));

    return std::make_shared<CProxyGraphicsPolyline>(points, prop);
}

void CGraphicsPolyline::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsPolyline::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

QByteArray CGraphicsPolyline::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

void CGraphicsPolyline::setPolygon(QPolygonF &poly)
{
    m_polygon = poly;
    QPainterPath path;
    path.addPolygon(poly);
    setPath(path);
}

void CGraphicsPolyline::updateCursor(const QPointF &pos)
{
    if(isSelected())
    {
        m_hitVertexIndex = hitVertex(pos);
        if(m_hitVertexIndex == -1)
            setCursor(Qt::ClosedHandCursor);
        else
            setCursor(Qt::SizeAllCursor);
    }
    else
        setCursor(Qt::ArrowCursor);
}

int CGraphicsPolyline::hitVertex(const QPointF &pos)
{
    qreal dist;
    int vertexIndex = -1;
    const qreal sqrInfluenceDistance = 25.0;
    qreal distMin = std::numeric_limits<qreal>::max();

    for(int i=0; i<m_polygon.size(); ++i)
    {
        dist =  ((pos.x() - m_polygon[i].x()) * (pos.x() - m_polygon[i].x())) +
                ((pos.y() - m_polygon[i].y()) * (pos.y() - m_polygon[i].y()));

        if(dist < sqrInfluenceDistance && dist < distMin)
        {
            distMin = dist;
            vertexIndex = i;
        }
    }
    return vertexIndex;
}

void CGraphicsPolyline::copy(const CGraphicsPolyline &polyline)
{
    m_polygon = polyline.m_polygon;

    QPainterPath path;
    path.addPolygon(m_polygon);
    setPath(path);

    setPen(polyline.pen());
    setCategory(polyline.getCategory());
    setParentItem(polyline.parentItem());
}
