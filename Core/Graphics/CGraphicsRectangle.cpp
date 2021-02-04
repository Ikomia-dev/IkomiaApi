#include "CGraphicsRectangle.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//-----------------------------
//- Class CProxyGraphicsEllipse
//-----------------------------
CProxyGraphicsRect::CProxyGraphicsRect()
    : CProxyGraphicsItem(GraphicsItem::RECTANGLE)
{
}

CProxyGraphicsRect::CProxyGraphicsRect(float x, float y, float w, float h)
    : CProxyGraphicsItem(GraphicsItem::RECTANGLE)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
}

CProxyGraphicsRect::CProxyGraphicsRect(float x, float y, float w, float h, const GraphicsRectProperty &property)
    : CProxyGraphicsItem(GraphicsItem::RECTANGLE)
{
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsRect::setX(float x)
{
    m_x = x;
}

void CProxyGraphicsRect::setY(float y)
{
    m_y = y;
}

void CProxyGraphicsRect::setWidth(float w)
{
    m_width = w;
}

void CProxyGraphicsRect::setHeight(float h)
{
    m_height = h;
}

float CProxyGraphicsRect::getX() const
{
    return m_x;
}

float CProxyGraphicsRect::getY() const
{
    return m_y;
}

float CProxyGraphicsRect::getWidth() const
{
    return m_width;
}

float CProxyGraphicsRect::getHeight() const
{
    return m_height;
}

QRectF CProxyGraphicsRect::getBoundingRect() const
{
    return QRectF(m_x, m_y, m_width, m_height);
}

void CProxyGraphicsRect::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CProxyGraphicsRect::translate(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsRect::clone() const
{
    return std::make_shared<CProxyGraphicsRect>(*this);
}

void CProxyGraphicsRect::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);
    obj["x"] = m_x;
    obj["y"] = m_y;
    obj["width"] = m_width;
    obj["height"] = m_height;
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//--------------------------
//- Class CGraphicsRectangle
//--------------------------
CGraphicsRectangle::CGraphicsRectangle(QGraphicsItem *pParent)
    : QGraphicsRectItem(pParent),
      CGraphicsItem(GraphicsItem::RECTANGLE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);

    QPen pen(QColor(255,0,0));
    pen.setCosmetic(true);
    setPen(pen);
    setBrush(QBrush(QColor(255,0,0,10)));
}

CGraphicsRectangle::CGraphicsRectangle(const GraphicsRectProperty &property, QGraphicsItem *pParent)
    : QGraphicsRectItem(pParent),
      CGraphicsItem(GraphicsItem::RECTANGLE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(property.m_category);

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsRectangle::CGraphicsRectangle(size_t id, const GraphicsRectProperty &property, QGraphicsItem *pParent)
    : QGraphicsRectItem(pParent),
      CGraphicsItem(id, GraphicsItem::RECTANGLE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(property.m_category);

    QPen pen(Utils::Graphics::toQColor(property.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(property.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(property.m_brushColor));
}

CGraphicsRectangle::CGraphicsRectangle(const cv::Rect &rect, QGraphicsItem *pParent)
    : QGraphicsRectItem(pParent),
      CGraphicsItem(GraphicsItem::RECTANGLE)
{
    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);

    QPen pen(QColor(255,0,0));
    pen.setCosmetic(true);
    setPen(pen);
    setBrush(QBrush(QColor(255,0,0,10)));
    setRect(rect.x, rect.y, rect.width, rect.height);
}

CGraphicsRectangle::CGraphicsRectangle(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsRect> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsRectItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::RECTANGLE)
{
    GraphicsRectProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getRectProperty();
    else
        prop = proxyItem->m_property;

    setAcceptHoverEvents(true);
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setCategory(prop.m_category);

    QPen pen(Utils::Graphics::toQColor(prop.m_penColor));
    pen.setCosmetic(true);
    pen.setWidth(prop.m_lineSize);
    setPen(pen);
    setBrush(Utils::Graphics::toQColor(prop.m_brushColor));

    setRect(proxyItem->m_x, proxyItem->m_y, proxyItem->m_width, proxyItem->m_height);
}

CGraphicsRectangle::CGraphicsRectangle(const CGraphicsRectangle &rectangle)
    : QGraphicsRectItem(rectangle.parentItem()),
      CGraphicsItem(GraphicsItem::RECTANGLE)
{
    copy(rectangle);
}

CGraphicsRectangle &CGraphicsRectangle::operator=(const CGraphicsRectangle &rectangle)
{
    copy(rectangle);
    return *this;
}

QGraphicsItem *CGraphicsRectangle::clone() const
{
    return new CGraphicsRectangle(*this);
}

void CGraphicsRectangle::setPenColor(QColor color)
{
    setPen(color);
}

void CGraphicsRectangle::setBrushColor(QColor color)
{
    setBrush(color);
}

int CGraphicsRectangle::type() const
{
    return Type;
}

QRectF CGraphicsRectangle::boundingRect() const
{
    if(isSelected())
    {
        //Add vertex size margins
        QRectF rect = QGraphicsRectItem::boundingRect();
        rect = rect.marginsAdded(QMarginsF() + Utils::Graphics::getSelectionMargin(pen().widthF()));
        return rect;
    }
    else
        return QGraphicsRectItem::boundingRect();
}

void CGraphicsRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected())
    {
        //Call parent rectangle paint without selected flag
        QStyleOptionGraphicsItem myOption(*option);
        myOption.state &= ~QStyle::State_Selected;
        QGraphicsRectItem::paint(painter, &myOption, widget);

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
        QGraphicsRectItem::paint(painter, option, widget);
}

void CGraphicsRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
        m_clickPt = event->pos();
    else
        QGraphicsRectItem::mousePressEvent(event);
}

void CGraphicsRectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_hitVertexIndex != -1)
    {
        updateBorder(event->pos().x() - m_clickPt.x(), event->pos().y() - m_clickPt.y());
        m_clickPt = event->pos();
    }
    else
    {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void CGraphicsRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void CGraphicsRectangle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateCursor(event->pos());
}

QByteArray CGraphicsRectangle::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

ProxyGraphicsItemPtr CGraphicsRectangle::createProxyGraphicsItem() const
{
    GraphicsRectProperty prop;
    prop.m_category = getCategory().toStdString();
    prop.m_penColor = Utils::Graphics::toCColor(pen().color());
    prop.m_lineSize = pen().width();
    prop.m_brushColor = Utils::Graphics::toCColor(brush().color());

    auto rc = mapRectToScene(rect());
    return std::make_shared<CProxyGraphicsRect>(rc.x(), rc.y(), rc.width(), rc.height(), prop);
}

void CGraphicsRectangle::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsRectangle::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

void CGraphicsRectangle::updateCursor(const QPointF &pos)
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

void CGraphicsRectangle::updateBorder(qreal dx, qreal dy)
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

int CGraphicsRectangle::hitVertex(const QPointF &pos)
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

void CGraphicsRectangle::switchHitVertex(bool bHorizontal, bool bVertical)
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

void CGraphicsRectangle::copy(const CGraphicsRectangle &rectangle)
{
    setRect(rectangle.rect());
    setPen(rectangle.pen());
    setBrush(rectangle.brush());
    setCategory(rectangle.getCategory());
    setParentItem(rectangle.parentItem());
}
