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

#include "CGraphicsEllipse.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//-----------------------------
//- Class CProxyGraphicsEllipse
//-----------------------------
CProxyGraphicsEllipse::CProxyGraphicsEllipse()
    : CProxyGraphicsItem(GraphicsItem::ELLIPSE)
{
}

CProxyGraphicsEllipse::CProxyGraphicsEllipse(float x, float y, float w, float h)
    : CProxyGraphicsItem(GraphicsItem::ELLIPSE)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
}

CProxyGraphicsEllipse::CProxyGraphicsEllipse(float x, float y, float w, float h, const GraphicsEllipseProperty &property)
    : CProxyGraphicsItem(GraphicsItem::ELLIPSE)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsEllipse::setX(float x)
{
    m_x = x;
}

void CProxyGraphicsEllipse::setY(float y)
{
    m_y = y;
}

void CProxyGraphicsEllipse::setWidth(float w)
{
    m_width = w;
}

void CProxyGraphicsEllipse::setHeight(float h)
{
    m_height = h;
}

float CProxyGraphicsEllipse::getX() const
{
    return m_x;
}

float CProxyGraphicsEllipse::getY() const
{
    return m_y;
}

float CProxyGraphicsEllipse::getWidth() const
{
    return m_width;
}

float CProxyGraphicsEllipse::getHeight() const
{
    return m_height;
}

QRectF CProxyGraphicsEllipse::getBoundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void CProxyGraphicsEllipse::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsEllipse::clone() const
{
    return std::make_shared<CProxyGraphicsEllipse>(*this);
}

void CProxyGraphicsEllipse::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);
    obj["x"] = m_x;
    obj["y"] = m_y;
    obj["width"] = m_width;
    obj["height"] = m_height;
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//------------------------
//- Class CGraphicsEllipse
//------------------------
CGraphicsEllipse::CGraphicsEllipse(QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(GraphicsItem::ELLIPSE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);

    QPen pen(QColor(255,0,0));
    pen.setCosmetic(true);
    setPen(pen);
    setBrush(QBrush(QColor(255,0,0,10)));
}

CGraphicsEllipse::CGraphicsEllipse(const GraphicsEllipseProperty& property, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(GraphicsItem::ELLIPSE)
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

CGraphicsEllipse::CGraphicsEllipse(size_t id, const GraphicsEllipseProperty &property, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(id, GraphicsItem::ELLIPSE)
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

CGraphicsEllipse::CGraphicsEllipse(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsEllipse> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsEllipseItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::ELLIPSE)
{
    GraphicsEllipseProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getEllipseProperty();
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

    setRect(proxyItem->m_x, proxyItem->m_y, proxyItem->m_width, proxyItem->m_height);
}

CGraphicsEllipse::CGraphicsEllipse(const CGraphicsEllipse &ellipse)
    : QGraphicsEllipseItem(ellipse.parentItem()),
      CGraphicsItem(GraphicsItem::ELLIPSE)
{
    copy(ellipse);
}

CGraphicsEllipse &CGraphicsEllipse::operator=(const CGraphicsEllipse &ellipse)
{
    copy(ellipse);
    return *this;
}

QGraphicsItem *CGraphicsEllipse::clone() const
{
    return new CGraphicsEllipse(*this);
}

void CGraphicsEllipse::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsEllipse::setBrushColor(QColor color)
{
    setBrush(color);
}

int CGraphicsEllipse::type() const
{
    return Type;
}

QRectF CGraphicsEllipse::boundingRect() const
{
    if(isSelected())
    {
        //Add vertex size margins
        QRectF rect = QGraphicsEllipseItem::boundingRect();
        rect = rect.marginsAdded(QMarginsF() + Utils::Graphics::getSelectionMargin(pen().widthF()));
        return rect;
    }
    else
        return QGraphicsEllipseItem::boundingRect();
}

QPainterPath CGraphicsEllipse::shape() const
{
    if(isSelected())
    {
        QPainterPath path;
        path.addRect(rect());
        return path;
    }
    else
        return QGraphicsEllipseItem::shape();
}

void CGraphicsEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    if(isSelected())
    {
        //Call parent rectangle paint
        QStyleOptionGraphicsItem myOption(*option);
        myOption.state &= ~QStyle::State_Selected;
        QGraphicsEllipseItem::paint(painter, &myOption, widget);

        //Highlight rectangle vertex
        QRectF rc = rect();
        qreal penW = pen().widthF();
        const qreal midX = (rc.left() + rc.right()) / 2.0;
        const qreal midY = (rc.top() + rc.bottom()) / 2.0;
        Utils::Graphics::paintSelectedVertex(painter, rc.topLeft(), penW);
        Utils::Graphics::paintSelectedVertex(painter, rc.topRight(), penW);
        Utils::Graphics::paintSelectedVertex(painter, rc.bottomLeft(), penW);
        Utils::Graphics::paintSelectedVertex(painter, rc.bottomRight(), penW);
        Utils::Graphics::paintSelectedVertex(painter, QPointF(rc.left(), midY), penW);
        Utils::Graphics::paintSelectedVertex(painter, QPointF(rc.right(), midY), penW);
        Utils::Graphics::paintSelectedVertex(painter, QPointF(midX, rc.top()), penW);
        Utils::Graphics::paintSelectedVertex(painter, QPointF(midX, rc.bottom()), penW);

        //Update bound rect to avoid paint artefacts
        prepareGeometryChange();
    }
    else
        QGraphicsEllipseItem::paint(painter, option, widget);
}

void CGraphicsEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
        m_clickPt = event->pos();
    else
        QGraphicsEllipseItem::mousePressEvent(event);
}

void CGraphicsEllipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hitVertexIndex != -1)
    {
        updateBorder(event->pos().x() - m_clickPt.x(), event->pos().y() - m_clickPt.y());
        m_clickPt = event->pos();
    }
    else
        QGraphicsEllipseItem::mouseMoveEvent(event);
}

void CGraphicsEllipse::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(event->pos());
}

QByteArray CGraphicsEllipse::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

ProxyGraphicsItemPtr CGraphicsEllipse::createProxyGraphicsItem() const
{
    GraphicsEllipseProperty prop;
    prop.m_category = getCategory().toStdString();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_lineSize = pen().width();
    prop.m_brushColor = Utils::Graphics::toCColor(brush().color());

    auto rc = mapRectToScene(rect());
    return std::make_shared<CProxyGraphicsEllipse>(rc.x(), rc.y(), rc.width(), rc.height(), prop);
}

void CGraphicsEllipse::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsEllipse::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CGraphicsEllipse::updateCursor(const QPointF &pos)
{
    if(isSelected())
    {
        m_hitVertexIndex = hitVertex(pos);
        switch(m_hitVertexIndex)
        {
            case 0:
            case 4: setCursor(Qt::SizeFDiagCursor); break;
            case 1:
            case 5: setCursor(Qt::SizeVerCursor); break;
            case 2:
            case 6: setCursor(Qt::SizeBDiagCursor); break;
            case 3:
            case 7: setCursor(Qt::SizeHorCursor); break;
            default: setCursor(Qt::ClosedHandCursor); break;
        }
    }
    else
        setCursor(Qt::ArrowCursor);
}

void CGraphicsEllipse::updateBorder(qreal dx, qreal dy)
{
    QRectF rc = rect();
    switch(m_hitVertexIndex)
    {
        case 0: rc.setTopLeft(rc.topLeft()+ QPointF(dx, dy)); break;
        case 1: rc.setTop(rc.top() + dy); break;
        case 2: rc.setTopRight(rc.topRight() + QPointF(dx, dy)); break;
        case 3: rc.setRight(rc.right() + dx); break;
        case 4: rc.setBottomRight(rc.bottomRight() + QPointF(dx, dy)); break;
        case 5: rc.setBottom(rc.bottom() + dy); break;
        case 6: rc.setBottomLeft(rc.bottomLeft() + QPointF(dx, dy)); break;
        case 7: rc.setLeft(rc.left() + dx); break;
    }

    if(m_hitVertexIndex != -1)
    {
        if(rc.width() < 0 || rc.height() < 0)
        {
            switchHitVertex(rc.width() < 0, rc.height() < 0);
            rc =  rc.normalized();
        }
        setRect(rc);
    }
}

int CGraphicsEllipse::hitVertex(const QPointF &pos)
{
    int vertexIndex = -1;
    QVector<qreal> distList;
    const qreal sqrInfluenceDistance = 25.0;
    qreal distMin = std::numeric_limits<qreal>::max();

    QRectF rc = rect();
    const qreal midX = (rc.left() + rc.right()) / 2.0;
    const qreal midY = (rc.top() + rc.bottom()) / 2.0;
    distList.push_back(Utils::Geometry::sqrDistance(pos, rc.topLeft()));
    distList.push_back(Utils::Geometry::sqrDistance(pos, QPointF(midX, rc.top())));
    distList.push_back(Utils::Geometry::sqrDistance(pos, rc.topRight()));
    distList.push_back(Utils::Geometry::sqrDistance(pos, QPoint(rc.right(), midY)));
    distList.push_back(Utils::Geometry::sqrDistance(pos, rc.bottomRight()));
    distList.push_back(Utils::Geometry::sqrDistance(pos, QPointF(midX, rc.bottom())));
    distList.push_back(Utils::Geometry::sqrDistance(pos, rc.bottomLeft()));
    distList.push_back(Utils::Geometry::sqrDistance(pos, QPointF(rc.left(), midY)));

    for(int i=0; i<distList.size(); ++i)
    {
        if(distList[i] < sqrInfluenceDistance && distList[i] < distMin)
        {
            distMin = distList[i];
            vertexIndex = i;
        }
    }
    return vertexIndex;
}

void CGraphicsEllipse::switchHitVertex(bool bHorizontal, bool bVertical)
{
    if(bHorizontal == true)
    {
        switch(m_hitVertexIndex)
        {
            case 0: m_hitVertexIndex = 2; break;
            case 1: break;
            case 2: m_hitVertexIndex = 0; break;
            case 3: m_hitVertexIndex = 7; break;
            case 4: m_hitVertexIndex = 6; break;
            case 5: break;
            case 6: m_hitVertexIndex = 4; break;
            case 7: m_hitVertexIndex = 3; break;
        }
    }

    if(bVertical == true)
    {
        switch(m_hitVertexIndex)
        {
            case 0: m_hitVertexIndex = 6; break;
            case 1: m_hitVertexIndex = 5; break;
            case 2: m_hitVertexIndex = 4; break;
            case 3: break;
            case 4: m_hitVertexIndex = 2; break;
            case 5: m_hitVertexIndex = 1; break;
            case 6: m_hitVertexIndex = 0; break;
            case 7: break;
        }
    }
}

void CGraphicsEllipse::copy(const CGraphicsEllipse &ellipse)
{
    setRect(ellipse.rect());
    setPen(ellipse.pen());
    setBrush(ellipse.brush());
    setCategory(ellipse.getCategory());
    setParentItem(ellipse.parentItem());
}

