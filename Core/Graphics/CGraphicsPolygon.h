#ifndef CGRAPHICSPOLYGON_H
#define CGRAPHICSPOLYGON_H

#include "Main/CoreDefine.hpp"
#include "QGraphicsPolygonItem"
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//-----------------------------------
//- Class CProxyGraphicsPolygon
//- Proxy class without Qt dependency
//-----------------------------------
class CORESHARED_EXPORT CProxyGraphicsPolygon: public CProxyGraphicsItem
{
    public:

        CProxyGraphicsPolygon();
        CProxyGraphicsPolygon(const std::vector<CPointF>& points);
        CProxyGraphicsPolygon(const std::vector<CPointF>& points, const GraphicsPolygonProperty& property);

        void            setPoints(const PolygonF& pts);

        PolygonF        getPoints() const;
        QRectF          getBoundingRect() const override;

        void            insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

        void            translate(float dx, float dy) override;

        std::shared_ptr<CProxyGraphicsItem> clone() const override;

        void            toJson(QJsonObject& obj) const override;

    public:

        std::vector<CPointF>    m_points;
        GraphicsPolygonProperty m_property;
};

//------------------------
//- Class CGraphicsPolygon
//------------------------
class CORESHARED_EXPORT CGraphicsPolygon : public QGraphicsPolygonItem, public CGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::POLYGON) };

        CGraphicsPolygon(QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(const GraphicsPolygonProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(size_t id, const GraphicsPolygonProperty& property, QGraphicsItem* pParent=nullptr);
        CGraphicsPolygon(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsPolygon> &proxyItem, QGraphicsItem *pParent=nullptr);
        CGraphicsPolygon(const CGraphicsPolygon& polygon);

        CGraphicsPolygon&       operator=(const CGraphicsPolygon& polygon);

        QGraphicsItem*          clone() const override;

        void                    setPenColor(QColor color) override;
        void                    setBrushColor(QColor color) override;

        int                     type() const override;

        QRectF                  boundingRect() const override;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        void                    mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
        void                    hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;

        void                    addPoint(QPointF pt);

        void                    updateLastPoint(const QPointF &pt);
        void                    updatePoint(int index, const QPointF &pt);

        QByteArray              getJsonData(CGraphicsJSON &jsonMgr) const override;

        ProxyGraphicsItemPtr    createProxyGraphicsItem() const override;

        void                    buildFromJsonData(CGraphicsJSON& jsonMgr, QByteArray data) override;

        void                    insertToImage(CMat& image, CGraphicsConversion& filler, bool bForceFill, bool bBinary) const override;

    private:

        void                    updateCursor(const QPointF &pos);

        int                     hitVertex(const QPointF& pos);

        void                    copy(const CGraphicsPolygon& poly);

    private:

        int     m_hitVertexIndex = -1;
};

//----------------------------------------------------------------
//- Class CGraphicsPolygonFactory
//- Factory for dynamic CGraphicsPolygon instanciation (from type)
//----------------------------------------------------------------
class CGraphicsPolygonFactory: public CGraphicsFactory
{
    public:

        CGraphicsPolygonFactory()
        {
            m_type = CGraphicsPolygon::UserType + static_cast<size_t>(GraphicsItem::POLYGON);
        }

        ~CGraphicsPolygonFactory(){}

        QGraphicsItem*  create(QGraphicsItem* pParent) override
        {
            return new CGraphicsPolygon(pParent);
        }
};

/** @endcond */

#endif // CGRAPHICSPOLYGON_H
