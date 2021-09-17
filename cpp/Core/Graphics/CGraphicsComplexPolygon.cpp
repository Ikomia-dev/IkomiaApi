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

#include "CGraphicsComplexPolygon.h"
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"
#include <QJsonArray>
#include "CGraphicsJSON.h"

//---------------------------------
//- Class CProxyGraphicsComplexPoly
//---------------------------------
CProxyGraphicsComplexPoly::CProxyGraphicsComplexPoly()
    : CProxyGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
}

CProxyGraphicsComplexPoly::CProxyGraphicsComplexPoly(const PolygonF &outer, const std::vector<PolygonF> &inners)
    : CProxyGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
    m_outer = outer;
    m_inners = inners;
}

CProxyGraphicsComplexPoly::CProxyGraphicsComplexPoly(const PolygonF &outer, const std::vector<PolygonF> &inners, const CGraphicsPolygonProperty &property)
    : CProxyGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
    m_outer = outer;
    m_inners = inners;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsComplexPoly::setOuter(const PolygonF &poly)
{
    m_outer = poly;
}

void CProxyGraphicsComplexPoly::setInners(const std::vector<PolygonF> &polygons)
{
    m_inners = polygons;
}

void CProxyGraphicsComplexPoly::setProperty(const CGraphicsPolygonProperty &prop)
{
    m_property = prop;
}

void CProxyGraphicsComplexPoly::setCategory(const std::string &categ)
{
    m_property.m_category = categ;
}

PolygonF CProxyGraphicsComplexPoly::getOuter() const
{
    return m_outer;
}

std::vector<PolygonF> CProxyGraphicsComplexPoly::getInners() const
{
    return m_inners;
}

QRectF CProxyGraphicsComplexPoly::getBoundingRect() const
{
    float xmin = FLT_MAX;
    float ymin = FLT_MAX;
    float xmax = FLT_MIN;
    float ymax = FLT_MIN;

    for(size_t i=0; i<m_outer.size(); ++i)
    {
        xmin = std::min(xmin, m_outer[i].m_x);
        ymin = std::min(ymin, m_outer[i].m_y);
        xmax = std::max(xmax, m_outer[i].m_x);
        ymax = std::max(ymax, m_outer[i].m_y);
    }
    return QRectF(xmin, ymin, std::floor(xmax - xmin) + 1, std::floor(ymax - ymin) + 1);
}

CGraphicsPolygonProperty CProxyGraphicsComplexPoly::getProperty() const
{
    return m_property;
}

std::string CProxyGraphicsComplexPoly::getCategory() const
{
    return m_property.m_category;
}

void CProxyGraphicsComplexPoly::translate(float dx, float dy)
{
    for(size_t i=0; i<m_outer.size(); ++i)
    {
        m_outer[i].m_x += dx;
        m_outer[i].m_y += dy;
    }

    for(size_t i=0; i<m_inners.size(); ++i)
    {
        for(size_t j=0; j<m_inners[i].size(); ++j)
        {
            m_inners[i][j].m_x += dx;
            m_inners[i][j].m_y += dy;
        }
    }
}

void CProxyGraphicsComplexPoly::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsComplexPoly::clone() const
{
    return std::make_shared<CProxyGraphicsComplexPoly>(*this);
}

void CProxyGraphicsComplexPoly::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);
    QJsonArray outerPtArray;
    auto outer = getOuter();

    for(size_t i=0; i<outer.size(); ++i)
    {
         QJsonObject point;
         point["x"] = outer[i].getX();
         point["y"] = outer[i].getY();
         outerPtArray.append(point);
    }
    obj["outer"] = outerPtArray;

    QJsonArray innerArray;
    auto inners = getInners();

    for(size_t i=0; i<inners.size(); ++i)
    {
        QJsonArray innerPtArray;
        for(size_t j=0; j<inners[i].size(); ++j)
        {
             QJsonObject point;
             point["x"] = inners[i][j].getX();
             point["y"] = inners[i][j].getY();
             innerPtArray.append(point);
        }
        innerArray.append(innerPtArray);
    }
    obj["inners"] = innerArray;
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//-------------------------------
//- Class CGraphicsComplexPolygon
//-------------------------------
CGraphicsComplexPolygon::CGraphicsComplexPolygon(QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setPen(QPen(QColor(255,0,0)));
    setBrush(QBrush(QColor(255,0,0,10)));
}

CGraphicsComplexPolygon::CGraphicsComplexPolygon(const CGraphicsPolygonProperty &property, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen;
    pen.setColor(Utils::Graphics::toQColor(property.m_penColor));
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsComplexPolygon::CGraphicsComplexPolygon(size_t id, const CGraphicsPolygonProperty &property, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(id, GraphicsItem::COMPLEX_POLYGON)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(QString::fromStdString(property.m_category));

    QPen pen;
    pen.setColor(Utils::Graphics::toQColor(property.m_penColor));
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsComplexPolygon::CGraphicsComplexPolygon(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsComplexPoly> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::COMPLEX_POLYGON)
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

    QPen pen;
    pen.setColor(Utils::Graphics::toQColor(prop.m_penColor));
    pen.setWidth(prop.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(prop.m_brushColor));

    QPolygonF outer;
    for(size_t i=0; i<proxyItem->m_outer.size(); ++i)
        outer.push_back(QPointF(proxyItem->m_outer[i].m_x, proxyItem->m_outer[i].m_y));

    setOuterPolygon(outer, false);

    QList<QPolygonF> inners;
    for(size_t i=0; i<proxyItem->m_inners.size(); ++i)
    {
        QPolygonF inner;
        for(size_t j=0; j<proxyItem->m_inners[i].size(); ++j)
            inner.push_back(QPointF(proxyItem->m_inners[i][j].m_x, proxyItem->m_inners[i][j].m_y));

        inners.push_back(inner);
    }
    setInnerPolygons(inners, false);
    buildPainterPath();
}

CGraphicsComplexPolygon::CGraphicsComplexPolygon(const CGraphicsComplexPolygon &polygon)
    : QGraphicsPathItem(polygon.parentItem()),
      CGraphicsItem(GraphicsItem::COMPLEX_POLYGON)
{
    copy(polygon);
}

CGraphicsComplexPolygon &CGraphicsComplexPolygon::operator=(const CGraphicsComplexPolygon &polygon)
{
    copy(polygon);
    return *this;
}

QGraphicsItem *CGraphicsComplexPolygon::clone() const
{
    return new CGraphicsComplexPolygon(*this);
}

void CGraphicsComplexPolygon::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsComplexPolygon::setBrushColor(QColor color)
{
    setBrush(color);
}

int CGraphicsComplexPolygon::type() const
{
    return Type;
}

QRectF CGraphicsComplexPolygon::boundingRect() const
{
    return path().boundingRect();
}

QPolygonF CGraphicsComplexPolygon::getOuterPolygon() const
{
    return m_outer;
}

QList<QPolygonF> CGraphicsComplexPolygon::getInnerPolygons() const
{
    return m_innerList;
}

void CGraphicsComplexPolygon::setOuterPolygon(QPolygonF polygon, bool bRedraw)
{
    m_outer = polygon;
    if(bRedraw)
        buildPainterPath();
}

void CGraphicsComplexPolygon::setInnerPolygons(const QList<QPolygonF> &innerList, bool bRedraw)
{
    m_innerList = innerList;
    if(bRedraw)
        buildPainterPath();
}

void CGraphicsComplexPolygon::addInnerPolygon(QPolygonF polygon, bool bRedraw)
{
    m_innerList.push_back(polygon);
    if(bRedraw)
        buildPainterPath();
}

void CGraphicsComplexPolygon::redraw()
{
    buildPainterPath();
}

ProxyGraphicsItemPtr CGraphicsComplexPolygon::createProxyGraphicsItem() const
{
    CGraphicsPolygonProperty prop;
    prop.m_category = getCategory().toStdString();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_lineSize = pen().width();
    prop.m_brushColor = Utils::Graphics::toCColor(brush().color());

    PolygonF outer;
    auto qOuter = mapToScene(m_outer);

    for(int i=0; i<qOuter.size(); ++i)
        outer.push_back(CPointF(qOuter[i].x(), qOuter[i].y()));

    std::vector<PolygonF> inners;
    for(int i=0; i<m_innerList.size(); ++i)
    {
        PolygonF inner;
        auto qInner = mapToScene(m_innerList[i]);

        for(int j=0; j<qInner.size(); ++j)
            inner.push_back(CPointF(qInner[j].x(), qInner[j].y()));

        inners.push_back(inner);
    }

    return std::make_shared<CProxyGraphicsComplexPoly>(outer, inners, prop);
}

void CGraphicsComplexPolygon::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsComplexPolygon::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CGraphicsComplexPolygon::clear()
{
    m_outer.clear();
    m_innerList.clear();
}

void CGraphicsComplexPolygon::copy(const CGraphicsComplexPolygon &polygon)
{
    //Outer polygon
    clear();
    setOuterPolygon(polygon.getOuterPolygon(), false);

    //Inner polygons
    setInnerPolygons(polygon.getInnerPolygons(), false);

    setPen(polygon.pen());
    setBrush(polygon.brush());
    setCategory(polygon.getCategory());
    setParentItem(polygon.parentItem());
    buildPainterPath();
}

QByteArray CGraphicsComplexPolygon::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

void CGraphicsComplexPolygon::buildPainterPath()
{
    QPainterPath path, outerPath, innerPath;
    path.setFillRule(Qt::WindingFill);
    outerPath.addPolygon(m_outer);

    for(int i=0; i<m_innerList.size(); ++i)
        innerPath.addPolygon(m_innerList[i]);

    path = outerPath.subtracted(innerPath);
    setPath(path);
}
