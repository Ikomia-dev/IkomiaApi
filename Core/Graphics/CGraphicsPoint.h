#ifndef CGRAPHICSPOINT_H
#define CGRAPHICSPOINT_H

#include "Main/CoreDefine.hpp"
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPoint
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPoint: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPoint();
        CProxyGraphicsPoint(const CPointF& point);
        CProxyGraphicsPoint(const CPointF& point, const GraphicsPointProperty& property);

        void            setPoint(const CPointF& pt);

        CPointF         getPoint() const;
        QRectF          getBoundingRect() const override;

        void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void            toJson(QJsonObject &obj) const override;

    public:

        CPointF                 m_point;
        GraphicsPointProperty   m_property;
};

//----------------------
//- Class CGraphicsPoint
//----------------------
class CORESHARED_EXPORT CGraphicsPoint: public QGraphicsEllipseItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POINT) };

        CGraphicsPoint(QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(const QPointF& position, const GraphicsPointProperty& property , QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(size_t id, const QPointF& position, const GraphicsPointProperty& property , QGraphicsItem* pParent=nullptr);
        CGraphicsPoint(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPoint> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPoint(const CGraphicsPoint& point);

        CGraphicsPoint&         operator=(const CGraphicsPoint& point);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;
        QRectF                  boundingRect() const override;
        QPainterPath            shape() const override;
        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mousePressEvent(QGraphicsSceneMouseEvent * event) override;
        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);

        void                    copy(const CGraphicsPoint& point);
};

//--------------------------------------------------------------
//- Class CGraphicsPointFactory
//- Factory for dynamic CGraphicsPoint instanciation (from type)
//--------------------------------------------------------------
class CGraphicsPointFactory: public CGraphicsFactory
{
    public:

        CGraphicsPointFactory()
        {
            m_type = CGraphicsPoint::UserType + static_cast<size_t>(GraphicsItem::POINT);
        }

        ~CGraphicsPointFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPoint(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSPOINT_H
