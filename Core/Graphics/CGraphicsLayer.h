#ifndef CGRAPHICSLAYER_H
#define CGRAPHICSLAYER_H

#include <QGraphicsItem>
#include "CGraphicsItem.hpp"

/** @cond INTERNAL */

//----------------------
//- Class CGraphicsLayer
//----------------------
class CORESHARED_EXPORT CGraphicsLayer : public QGraphicsItem
{
    public:

        enum { Type = UserType + static_cast<size_t>(GraphicsItem::LAYER) };

        CGraphicsLayer(const QString& name, CGraphicsLayer* pParent = nullptr);
        CGraphicsLayer(const QString& name, bool bUserLayer, CGraphicsLayer* pParent = nullptr);
        CGraphicsLayer(const CGraphicsLayer& layer);
        ~CGraphicsLayer();

        CGraphicsLayer&         operator=(const CGraphicsLayer& layer);

        void                    setName(const QString& name);
        void                    setRoot(bool bRoot);

        int                     type() const override;
        QRectF                  boundingRect() const override;
        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        QString                 getName() const;
        size_t                  getChildLayerCount() const;
        QList<CGraphicsLayer*>  getChildLayers() const;
        size_t                  getChildItemCount() const;
        QList<QGraphicsItem*>   getChildItems() const;

        bool                    isRoot() const;
        bool                    isUserLayer() const;

        void                    deleteChildItems();

    private:

        void                    copy(const CGraphicsLayer& layer);

    private:

        QString         m_name;
        bool            m_bRoot = false;
        bool            m_bUserLayer = false;
};

/** @endcond */

#endif // CGRAPHICSLAYER_H
