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

#include "CGraphicsPolygon.h"
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QJsonArray>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//-----------------------------
//- Class CProxyGraphicsPolygon
//-----------------------------
CProxyGraphicsPolygon::CProxyGraphicsPolygon()
    : CProxyGraphicsItem(GraphicsItem::POLYGON)
{
}

CProxyGraphicsPolygon::CProxyGraphicsPolygon(const std::vector<CPointF> &points)
    : CProxyGraphicsItem(GraphicsItem::POLYGON)
{
    m_points = points;
    //Close polygon
    m_points.push_back(points[0]);
}

CProxyGraphicsPolygon::CProxyGraphicsPolygon(const std::vector<CPointF> &points, const CGraphicsPolygonProperty &property)
    : CProxyGraphicsItem(GraphicsItem::POLYGON)
{
    m_points = points;
    //Close polygon
    if(points.size() > 0)
        m_points.push_back(points[0]);

    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsPolygon::setPoints(const PolygonF &pts)
{
    m_points = pts;
}

void CProxyGraphicsPolygon::setProperty(const CGraphicsPolygonProperty &prop)
{
    m_property = prop;
}

PolygonF CProxyGraphicsPolygon::getPoints() const
{
    return m_points;
}

QRectF CProxyGraphicsPolygon::getBoundingRect() const
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
    return QRectF(xmin, ymin, std::floor(xmax - xmin) + 1, std::floor(ymax - ymin) + 1);
}

CGraphicsPolygonProperty CProxyGraphicsPolygon::getProperty() const
{
    return m_property;
}

void CProxyGraphicsPolygon::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CProxyGraphicsPolygon::translate(float dx, float dy)
{
    for(size_t i=0; i<m_points.size(); ++i)
    {
        m_points[i].m_x += dx;
        m_points[i].m_y += dy;
    }
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsPolygon::clone() const
{
    return std::make_shared<CProxyGraphicsPolygon>(*this);
}

void CProxyGraphicsPolygon::toJson(QJsonObject &obj) const
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
    obj["polygon"] = pointArray;
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//------------------------
//- Class CGraphicsPolygon
//------------------------
CGraphicsPolygon::CGraphicsPolygon(QGraphicsItem *pParent)
    : QGraphicsPolygonItem(pParent),
      CGraphicsItem(GraphicsItem::POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);

    QPen pen(QColor(255,0,0));
    pen.setCosmetic(true);
    setPen(pen);
    setBrush(QBrush(QColor(255,0,0,10)));
}

CGraphicsPolygon::CGraphicsPolygon(const CGraphicsPolygonProperty &property, QGraphicsItem *pParent)
    : QGraphicsPolygonItem(pParent),
      CGraphicsItem(GraphicsItem::POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsPolygon::CGraphicsPolygon(size_t id, const CGraphicsPolygonProperty &property, QGraphicsItem *pParent)
    : QGraphicsPolygonItem(pParent),
      CGraphicsItem(id, GraphicsItem::POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsPolygon::CGraphicsPolygon(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolygon> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsPolygonItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::POLYGON)
{
    CGraphicsPolygonProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getPolygonProperty();
    else
        prop = proxyItem->m_property;

    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(prop.m_category));

    QPen pen(Utils::Graphics::toQColor(prop.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(prop.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(prop.m_brushColor));

    QPolygonF poly;
    for(size_t i=0; i<proxyItem->m_points.size(); ++i)
        poly.push_back(QPointF(proxyItem->m_points[i].m_x, proxyItem->m_points[i].m_y));

    setPolygon(poly);
}

CGraphicsPolygon::CGraphicsPolygon(const CGraphicsPolygon &polygon)
    : QGraphicsPolygonItem(polygon.parentItem()),
      CGraphicsItem(GraphicsItem::POLYGON)
{
    copy(polygon);
}

CGraphicsPolygon &CGraphicsPolygon::operator=(const CGraphicsPolygon &polygon)
{
    copy(polygon);
    return *this;
}

QGraphicsItem *CGraphicsPolygon::clone() const
{
    return new CGraphicsPolygon(*this);
}

void CGraphicsPolygon::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsPolygon::setBrushColor(QColor color)
{
    setBrush(color);
}

int CGraphicsPolygon::type() const
{
    return Type;
}

QRectF CGraphicsPolygon::boundingRect() const
{
    if(isSelected())
    {
        //Add vertex size margins
        QRectF rect = QGraphicsPolygonItem::boundingRect();
        rect = rect.marginsAdded(QMarginsF() + Utils::Graphics::getSelectionMargin(pen().widthF()));
        return rect;
    }
    else
        return QGraphicsPolygonItem::boundingRect();
}

void CGraphicsPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    if(isSelected())
    {
        //Call parent polygon paint without selected flag
        QStyleOptionGraphicsItem myOption(*option);
        myOption.state &= ~QStyle::State_Selected;
        QGraphicsPolygonItem::paint(painter, &myOption, widget);

        //Highlight polygon vertex
        for(auto it : polygon())
            Utils::Graphics::paintSelectedVertex(painter, it, pen().widthF());

        //Update bound rect to avoid paint artefacts
        prepareGeometryChange();
    }
    else
        QGraphicsPolygonItem::paint(painter, option, widget);
}

void CGraphicsPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hitVertexIndex != -1)
        updatePoint(m_hitVertexIndex, event->pos());
    else
        QGraphicsPolygonItem::mouseMoveEvent(event);
}

void CGraphicsPolygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(event->pos());
}

void CGraphicsPolygon::addPoint(QPointF pt)
{
    QPolygonF poly = polygon();
    if(poly.size() == 0)
        poly.push_back(pt);

    poly.push_back(pt);
    setPolygon(poly);
}

void CGraphicsPolygon::updateLastPoint(const QPointF &pt)
{
    QPolygonF poly = polygon();
    if(!poly.isEmpty())
    {
        poly.last() = pt;
        setPolygon(poly);
    }
}

void CGraphicsPolygon::updatePoint(int index, const QPointF &pt)
{
    QPolygonF poly = polygon();
    if(index >= 0 && index < poly.size())
    {
        poly[index] = pt;
        setPolygon(poly);
    }
}

QByteArray CGraphicsPolygon::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

ProxyGraphicsItemPtr CGraphicsPolygon::createProxyGraphicsItem() const
{
    CGraphicsPolygonProperty prop;
    prop.m_category = getCategory().toStdString();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_lineSize = pen().width();
    prop.m_brushColor = Utils::Graphics::toCColor(brush().color());

    QPolygonF poly = mapToScene(polygon());
    std::vector<CPointF> points;

    for(int i=0; i<poly.size(); ++i)
        points.push_back(CPointF(poly[i].x(), poly[i].y()));

    return std::make_shared<CProxyGraphicsPolygon>(points, prop);
}

void CGraphicsPolygon::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsPolygon::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CGraphicsPolygon::updateCursor(const QPointF &pos)
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

int CGraphicsPolygon::hitVertex(const QPointF &pos)
{
    qreal dist;
    int vertexIndex = -1;
    const qreal sqrInfluenceDistance = 25.0;
    QPolygonF poly = polygon();
    qreal distMin = std::numeric_limits<qreal>::max();

    for(int i=0; i<poly.size(); ++i)
    {
        dist =  ((pos.x() - poly[i].x()) * (pos.x() - poly[i].x())) +
                ((pos.y() - poly[i].y()) * (pos.y() - poly[i].y()));

        if(dist < sqrInfluenceDistance && dist < distMin)
        {
            distMin = dist;
            vertexIndex = i;
        }
    }
    return vertexIndex;
}

void CGraphicsPolygon::copy(const CGraphicsPolygon &poly)
{
    setPolygon(poly.polygon());
    setPen(poly.pen());
    setBrush(poly.brush());
    setCategory(poly.getCategory());
    setParentItem(poly.parentItem());
}
