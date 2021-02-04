#include "CGraphicsLayer.h"

//----------------------
//- Class CGraphicsLayer
//----------------------
CGraphicsLayer::CGraphicsLayer(const QString &name, CGraphicsLayer *pParent) : QGraphicsItem(pParent)
{
    m_name = name;
}

CGraphicsLayer::CGraphicsLayer(const QString &name, bool bUserLayer, CGraphicsLayer *pParent) : QGraphicsItem(pParent)
{
    m_name = name;
    m_bUserLayer = bUserLayer;
}

CGraphicsLayer::CGraphicsLayer(const CGraphicsLayer &layer) : QGraphicsItem(nullptr)
{
    copy(layer);
}

CGraphicsLayer::~CGraphicsLayer()
{
}

CGraphicsLayer &CGraphicsLayer::operator=(const CGraphicsLayer &layer)
{
    copy(layer);
    return *this;
}

void CGraphicsLayer::setName(const QString &name)
{
    m_name = name;
}

void CGraphicsLayer::setRoot(bool bRoot)
{
    m_bRoot = bRoot;
}

int CGraphicsLayer::type() const
{
    return Type;
}

QRectF CGraphicsLayer::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

void CGraphicsLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QString CGraphicsLayer::getName() const
{
    return m_name;
}

size_t CGraphicsLayer::getChildLayerCount() const
{
    size_t nb = 0;
    auto childs = childItems();

    for(int i=0; i<childs.size(); ++i)
    {
        if(childs[i]->type() == type())
            ++nb;
    }
    return nb;
}

QList<CGraphicsLayer*> CGraphicsLayer::getChildLayers() const
{
    QList<CGraphicsLayer*> childLayers;
    auto childs = childItems();

    for(int i=0; i<childs.size(); ++i)
    {
        if(childs[i]->type() == type())
            childLayers.push_back(static_cast<CGraphicsLayer*>(childs[i]));
    }
    return childLayers;
}

size_t CGraphicsLayer::getChildItemCount() const
{
    size_t nb = 0;
    auto childs = childItems();

    for(int i=0; i<childs.size(); ++i)
    {
        if(childs[i]->type() != type())
            ++nb;
    }
    return nb;
}

QList<QGraphicsItem*> CGraphicsLayer::getChildItems() const
{
    QList<QGraphicsItem*> childItems;
    auto childs = this->childItems();

    for(int i=0; i<childs.size(); ++i)
    {
        int childType = childs[i]->type();
        //If child item is a layer
        if(childType == type())
        {
            auto pChildLayer = static_cast<CGraphicsLayer*>(childs[i]);
            auto subItems = pChildLayer->getChildItems();
            childItems.append(subItems);
        }
        else
            childItems.push_back(childs[i]);
    }
    return childItems;
}

bool CGraphicsLayer::isRoot() const
{
    return m_bRoot;
}

bool CGraphicsLayer::isUserLayer() const
{
    return m_bUserLayer;
}

void CGraphicsLayer::deleteChildItems()
{
    auto childs = this->childItems();
    for(int i=0; i<childs.size(); ++i)
    {
        if(childs[i]->type() != type())
            delete childs[i];
    }
}

void CGraphicsLayer::copy(const CGraphicsLayer &layer)
{
    m_name = layer.m_name;
    m_bRoot = layer.m_bRoot;
    m_bUserLayer = layer.m_bUserLayer;

    //Copy of childs items
    auto childs = layer.childItems();
    for(int i=0; i<childs.size(); ++i)
    {
        if(childs[i]->type() == type())
        {
            //Child layer
            auto pChild = new CGraphicsLayer(*(static_cast<CGraphicsLayer*>(childs[i])));
            pChild->setParentItem(this);
        }
        else
        {
            //CGraphicsItmInterface derived object
            auto pChild = dynamic_cast<CGraphicsItem*>(childs[i]);
            if(pChild)
            {
                QGraphicsItem* pNewGraphic = pChild->clone();
                pNewGraphic->setParentItem(this);
            }
        }
    }
}
