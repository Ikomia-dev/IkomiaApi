#ifndef CGRAPHICSJSON_H
#define CGRAPHICSJSON_H

#include <QByteArray>
#include <QJsonObject>
#include <QPen>
#include "Main/CoreGlobal.hpp"
#include "GraphicsProperty.hpp"

/** @cond INTERNAL */

class QGraphicsItem;
class CGraphicsPoint;
class CGraphicsEllipse;
class CGraphicsRectangle;
class CGraphicsPolygon;
class CGraphicsPolyline;
class CGraphicsComplexPolygon;
class CGraphicsText;

class CORESHARED_EXPORT CGraphicsJSON
{
    public:

        CGraphicsJSON();

        static QByteArray  getBinaryData(const QGraphicsItem* pItem);
        static QByteArray  getBinaryData(const CGraphicsPoint* pItem);
        static QByteArray  getBinaryData(const CGraphicsEllipse* pItem);
        static QByteArray  getBinaryData(const CGraphicsRectangle* pItem);
        static QByteArray  getBinaryData(const CGraphicsPolygon* pItem);
        static QByteArray  getBinaryData(const CGraphicsPolyline* pItem);
        static QByteArray  getBinaryData(const CGraphicsComplexPolygon* pItem);
        static QByteArray  getBinaryData(const CGraphicsText* pItem);

        static QJsonObject toJsonObject(const QPen& pen);
        static QJsonObject toJsonObject(const QBrush& brush);
        static QJsonObject toJsonObject(const QColor& color);
        static QJsonObject toJsonObject(const QFont& font);
        static QJsonObject toJsonObject(const CColor& color);
        static QJsonObject toJsonObject(const GraphicsPointProperty& prop);
        static QJsonObject toJsonObject(const GraphicsRectProperty& prop);
        static QJsonObject toJsonObject(const GraphicsEllipseProperty& prop);
        static QJsonObject toJsonObject(const GraphicsPolygonProperty& prop);
        static QJsonObject toJsonObject(const GraphicsPolylineProperty& prop);
        static QJsonObject toJsonObject(const GraphicsTextProperty& prop);

        static void        buildObject(QGraphicsItem* pItem, QByteArray data);
        static void        buildObject(CGraphicsPoint* pItem, QByteArray data);
        static void        buildObject(CGraphicsEllipse* pItem, QByteArray data);
        static void        buildObject(CGraphicsRectangle* pItem, QByteArray data);
        static void        buildObject(CGraphicsPolygon* pItem, QByteArray data);
        static void        buildObject(CGraphicsPolyline* pItem, QByteArray data);
        static void        buildObject(CGraphicsComplexPolygon* pItem, QByteArray data);
        static void        buildObject(CGraphicsText* pItem, QByteArray data);

        static void        buildObject(QColor& color, const QJsonObject& json);
        static void        buildObject(QPen& pen, const QJsonObject& json);
        static void        buildObject(QBrush& brush, const QJsonObject& json);
        static void        buildObject(QFont& font, const QJsonObject& json);
};

/** @endcond */

#endif // CGRAPHICSJSON_H
