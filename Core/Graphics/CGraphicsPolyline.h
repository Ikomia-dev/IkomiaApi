#ifndef CGRAPHICSLINE_H
#define CGRAPHICSLINE_H

#include "Main/CoreDefine.hpp"
#include <QGraphicsPathItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPolygon
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPolyline: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPolyline();
        CProxyGraphicsPolyline(const std::vector<CPointF>& points);
        CProxyGraphicsPolyline(const std::vector<CPointF>& points, const GraphicsPolylineProperty& property);

        void            setPoints(const PolygonF& pts);

        PolygonF        getPoints() const;
        QRectF          getBoundingRect() const override;

        void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void            toJson(QJsonObject &obj) const override;

    public:

        std::vector<CPointF>        m_points;
        GraphicsPolylineProperty    m_property;
};

//-------------------------
//- Class CGraphicsPolyline
//-------------------------
class CORESHARED_EXPORT CGraphicsPolyline : public QGraphicsPathItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POLYLINE) };

        CGraphicsPolyline(QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(const GraphicsPolylineProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(size_t id, const GraphicsPolylineProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolyline(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolyline> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPolyline(const CGraphicsPolyline& polyline);

        CGraphicsPolyline&      operator=(const CGraphicsPolyline& polyline);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QRectF                  boundingRect() const override;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        QPolygonF               getPolygon() const;
        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        void                    setPolygon(QPolygonF& poly);

        void                    addPoint(QPointF pt);

        void                    updateLastPoint(QPointF pt);
        void                    updatePoint(int index, const QPointF &pt);

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);
        int                     hitVertex(const QPointF& pos);
        void                    copy(const CGraphicsPolyline& polyline);

    private:

        QPolygonF   m_polygon;
        int         m_hitVertexIndex = -1;
};

//-----------------------------------------------------------------
//- Class CGraphicsPolylineFactory
//- Factory for dynamic CGraphicsPolyline instanciation (from type)
//-----------------------------------------------------------------
class CGraphicsPolylineFactory: public CGraphicsFactory
{
    public:

        CGraphicsPolylineFactory()
        {
            m_type = CGraphicsPolyline::UserType + static_cast<size_t>(GraphicsItem::POLYLINE);
        }

        ~CGraphicsPolylineFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPolyline(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSLINE_H
