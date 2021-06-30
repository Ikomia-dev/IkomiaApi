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

#include "CGraphicsJSON.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFont>
#include "CGraphicsPoint.h"
#include "CGraphicsEllipse.h"
#include "CGraphicsRectangle.h"
#include "CGraphicsPolygon.h"
#include "CGraphicsPolyline.h"
#include "CGraphicsComplexPolygon.h"
#include "CGraphicsText.h"

CGraphicsJSON::CGraphicsJSON()
{
}

QByteArray CGraphicsJSON::getBinaryData(const QGraphicsItem *pItem)
{
    Q_UNUSED(pItem);
    return nullptr;
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsPoint *pItem)
{
    assert(pItem);

    QJsonObject json;
    json["x"] = pItem->rect().x();
    json["y"] = pItem->rect().y();
    json["width"] = pItem->rect().width();
    json["height"] = pItem->rect().height();
    json["pen"] = toJsonObject(pItem->pen());
    json["brush"] = toJsonObject(pItem->brush());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsEllipse *pItem)
{
    assert(pItem);

    QJsonObject json;
    json["x"] = pItem->rect().x();
    json["y"] = pItem->rect().y();
    json["width"] = pItem->rect().width();
    json["height"] = pItem->rect().height();
    json["pen"] = toJsonObject(pItem->pen());
    json["brush"] = toJsonObject(pItem->brush());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsRectangle *pItem)
{
    assert(pItem);

    QJsonObject json;
    json["x"] = pItem->rect().x();
    json["y"] = pItem->rect().y();
    json["width"] = pItem->rect().width();
    json["height"] = pItem->rect().height();
    json["pen"] = toJsonObject(pItem->pen());
    json["brush"] = toJsonObject(pItem->brush());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsPolygon *pItem)
{
    assert(pItem);

    QJsonObject json;
    QJsonArray pointArray;
    QPolygonF polygon = pItem->polygon();

    for(int i=0; i<polygon.size(); ++i)
    {
         QJsonObject point;
         point["x"] = polygon[i].x();
         point["y"] = polygon[i].y();
         pointArray.append(point);
    }
    json["polygon"] = pointArray;
    json["pen"] = toJsonObject(pItem->pen());
    json["brush"] = toJsonObject(pItem->brush());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsPolyline *pItem)
{
    assert(pItem);

    QJsonObject json;
    QJsonArray pointArray;
    QPolygonF polygon = pItem->getPolygon();

    for(int i=0; i<polygon.size(); ++i)
    {
         QJsonObject point;
         point["x"] = polygon[i].x();
         point["y"] = polygon[i].y();
         pointArray.append(point);
    }
    json["pointList"] = pointArray;
    json["pen"] = toJsonObject(pItem->pen());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsComplexPolygon *pItem)
{
    assert(pItem);

    QJsonObject json;
    QJsonArray outerPtArray;
    auto outer = pItem->getOuterPolygon();

    for(int i=0; i<outer.size(); ++i)
    {
         QJsonObject point;
         point["x"] = outer[i].x();
         point["y"] = outer[i].y();
         outerPtArray.append(point);
    }
    json["outer"] = outerPtArray;

    QJsonArray innerArray;
    auto inners = pItem->getInnerPolygons();

    for(int i=0; i<inners.size(); ++i)
    {
        QJsonArray innerPtArray;
        for(int j=0; j<inners[i].size(); ++j)
        {
             QJsonObject point;
             point["x"] = inners[i][j].x();
             point["y"] = inners[i][j].y();
             innerPtArray.append(point);
        }
        innerArray.append(innerPtArray);
    }
    json["inners"] = innerArray;
    json["pen"] = toJsonObject(pItem->pen());
    json["brush"] = toJsonObject(pItem->brush());
    json["category"] = pItem->getCategory();

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QByteArray CGraphicsJSON::getBinaryData(const CGraphicsText *pItem)
{
    assert(pItem);

    QJsonObject json;
    json["x"] = pItem->pos().x();
    json["y"] = pItem->pos().y();
    json["color"] = toJsonObject(pItem->defaultTextColor());
    json["text"] = pItem->toPlainText();
    json["font"] = toJsonObject(pItem->font());

    QJsonDocument doc(json);
    return doc.toBinaryData();
}

QJsonObject CGraphicsJSON::toJsonObject(const QPen &pen)
{
    QJsonObject json;
    json["width"] = pen.width();
    json["style"] = pen.style();
    json["color"] = toJsonObject(pen.color());
    return json;
}

QJsonObject CGraphicsJSON::toJsonObject(const QBrush& brush)
{
    QJsonObject json;
    json["style"] = brush.style();
    json["color"] = toJsonObject(brush.color());
    return json;
}

QJsonObject CGraphicsJSON::toJsonObject(const QColor &color)
{
    QJsonObject json;
    json["r"] = color.red();
    json["g"] = color.green();
    json["b"] = color.blue();
    json["a"] = color.alpha();
    return json;
}

QJsonObject CGraphicsJSON::toJsonObject(const QFont &font)
{
    QJsonObject json;
    json["family"] = font.family();
    json["pointSize"] = font.pointSize();
    json["weight"] = font.weight();
    json["bold"] = font.bold();
    json["italic"] = font.italic();
    json["underline"] = font.underline();
    json["strikeout"] = font.strikeOut();
    return json;
}

QJsonObject CGraphicsJSON::toJsonObject(const CColor &color)
{
    QJsonObject obj;
    obj["r"] = color[0];
    obj["g"] = color[1];
    obj["b"] = color[2];
    obj["a"] = color[3];
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsPointProperty &prop)
{
    QJsonObject obj;
    obj["pen"] = toJsonObject(prop.m_penColor);
    obj["brush"] = toJsonObject(prop.m_brushColor);
    obj["size"] = prop.m_size;
    obj["category"] = QString::fromStdString(prop.m_category);
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsRectProperty &prop)
{
    QJsonObject obj;
    obj["pen"] = toJsonObject(prop.m_penColor);
    obj["brush"] = toJsonObject(prop.m_brushColor);
    obj["line"] = prop.m_lineSize;
    obj["category"] = QString::fromStdString(prop.m_category);
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsEllipseProperty &prop)
{
    QJsonObject obj;
    obj["pen"] = toJsonObject(prop.m_penColor);
    obj["brush"] = toJsonObject(prop.m_brushColor);
    obj["line"] = prop.m_lineSize;
    obj["category"] = QString::fromStdString(prop.m_category);
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsPolygonProperty &prop)
{
    QJsonObject obj;
    obj["pen"] = toJsonObject(prop.m_penColor);
    obj["brush"] = toJsonObject(prop.m_brushColor);
    obj["line"] = prop.m_lineSize;
    obj["category"] = QString::fromStdString(prop.m_category);
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsPolylineProperty &prop)
{
    QJsonObject obj;
    obj["pen"] = toJsonObject(prop.m_penColor);
    obj["line"] = prop.m_lineSize;
    obj["category"] = QString::fromStdString(prop.m_category);
    return obj;
}

QJsonObject CGraphicsJSON::toJsonObject(const CGraphicsTextProperty &prop)
{
    QJsonObject obj;
    obj["color"] = toJsonObject(prop.m_color);
    obj["font"] = QString::fromStdString(prop.m_fontName);
    obj["size"] = prop.m_fontSize;
    obj["bold"] = prop.m_bBold;
    obj["italic"] = prop.m_bItalic;
    obj["underline"] = prop.m_bUnderline;
    obj["strikeout"] = prop.m_bStrikeOut;
    return obj;
}

void CGraphicsJSON::buildObject(QGraphicsItem *pItem, QByteArray data)
{
    Q_UNUSED(pItem);
    Q_UNUSED(data);
}

void CGraphicsJSON::buildObject(CGraphicsPoint *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for point", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for point", __func__, __FILE__, __LINE__);

    //Build item
    qreal x = json["x"].toDouble();
    qreal y = json["y"].toDouble();
    qreal width = json["width"].toDouble();
    qreal height = json["height"].toDouble();
    pItem->setRect(x, y, width, height);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    QBrush brush;
    buildObject(brush, json["brush"].toObject());
    pItem->setBrush(brush);

    pItem->setCategory(json["category"].toString());
}

void CGraphicsJSON::buildObject(CGraphicsEllipse *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for ellipse", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for ellipse", __func__, __FILE__, __LINE__);

    //Build item
    qreal x = json["x"].toDouble();
    qreal y = json["y"].toDouble();
    qreal width = json["width"].toDouble();
    qreal height = json["height"].toDouble();
    pItem->setRect(x, y, width, height);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    QBrush brush;
    buildObject(brush, json["brush"].toObject());
    pItem->setBrush(brush);

    pItem->setCategory(json["category"].toString());
}

void CGraphicsJSON::buildObject(CGraphicsRectangle *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for rectangle", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for rectangle", __func__, __FILE__, __LINE__);

    //Build item
    qreal x = json["x"].toDouble();
    qreal y = json["y"].toDouble();
    qreal width = json["width"].toDouble();
    qreal height = json["height"].toDouble();
    pItem->setRect(x, y, width, height);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    QBrush brush;
    buildObject(brush, json["brush"].toObject());
    pItem->setBrush(brush);

    pItem->setCategory(json["category"].toString());
}

void CGraphicsJSON::buildObject(CGraphicsPolygon *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polygon", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polygon", __func__, __FILE__, __LINE__);

    //Build item
    QPolygonF polygon;
    QJsonArray pointArray;

    pointArray = json["polygon"].toArray();
    for(int i=0; i<pointArray.size(); ++i)
    {
        QJsonObject point = pointArray[i].toObject();
        polygon.push_back(QPointF(point["x"].toDouble(), point["y"].toDouble()));
    }
    pItem->setPolygon(polygon);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    QBrush brush;
    buildObject(brush, json["brush"].toObject());
    pItem->setBrush(brush);

    pItem->setCategory(json["category"].toString());
}

void CGraphicsJSON::buildObject(CGraphicsPolyline *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polyline", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polyline", __func__, __FILE__, __LINE__);

    //Build item
    QPolygonF poly;
    QJsonArray pointArray;
    pointArray = json["pointList"].toArray();

    for(int i=0; i<pointArray.size(); ++i)
    {
        QJsonObject point = pointArray[i].toObject();
        poly.push_back(QPointF(point["x"].toDouble(), point["y"].toDouble()));
    }

    pItem->setPolygon(poly);
    QPainterPath path;
    path.addPolygon(poly);
    pItem->setPath(path);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    pItem->setCategory(json["category"].toString());
}

void CGraphicsJSON::buildObject(CGraphicsComplexPolygon *pItem, QByteArray data)
{
    assert(pItem);

    if(data.isNull() || data.isEmpty())
        throw CException(CoreExCode::NULL_POINTER, QObject::tr("Graphics object can't be created from empty data").toStdString(), __func__, __FILE__, __LINE__);

    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for complex polygon", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for complex polygon", __func__, __FILE__, __LINE__);

    //Outer polygon
    pItem->clear();
    QPolygonF outer;
    QJsonArray outerPtArray = json["outer"].toArray();

    for(int i=0; i<outerPtArray.size(); ++i)
    {
        QJsonObject point = outerPtArray[i].toObject();
        outer.push_back(QPointF(point["x"].toDouble(), point["y"].toDouble()));
    }
    pItem->setOuterPolygon(outer, false);

    //Inner polygons
    QList<QPolygonF> inners;
    QJsonArray innerArray = json["inners"].toArray();

    for(int i=0; i<innerArray.size(); ++i)
    {
        QPolygonF inner;
        QJsonArray innerPtArray = innerArray[i].toArray();
        for(int j=0; j<innerPtArray.size(); ++j)
        {
            QJsonObject point = innerPtArray[j].toObject();
            inner.push_back(QPointF(point["x"].toDouble(), point["y"].toDouble()));
        }
        inners.push_back(inner);
    }
    pItem->setInnerPolygons(inners, false);

    QPen pen;
    buildObject(pen, json["pen"].toObject());
    pItem->setPen(pen);

    QBrush brush;
    buildObject(brush, json["brush"].toObject());
    pItem->setBrush(brush);

    pItem->setCategory(json["category"].toString());

    pItem->buildPainterPath();
}

void CGraphicsJSON::buildObject(CGraphicsText *pItem, QByteArray data)
{
    //Retrieve JSON document and object
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    if(doc.isNull())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polyline", __func__, __FILE__, __LINE__);

    QJsonObject json = doc.object();
    if(json.isEmpty())
        throw CException(CoreExCode::INVALID_JSON_FORMAT, "Invalid JSON data for polyline", __func__, __FILE__, __LINE__);

    //Build item
    pItem->setPos(json["x"].toDouble(), json["y"].toDouble());

    QColor color;
    buildObject(color, json["color"].toObject());
    pItem->setDefaultTextColor(color);

    pItem->setPlainText(json["text"].toString());

    QFont font;
    buildObject(font, json["font"].toObject());
    pItem->setFont(font);
}

void CGraphicsJSON::buildObject(QColor &color, const QJsonObject &json)
{
    int red = json["r"].toInt();
    int green = json["g"].toInt();
    int blue = json["b"].toInt();
    int alpha = json["a"].toInt();
    color.setRgb(red, green, blue, alpha);
}

void CGraphicsJSON::buildObject(QPen &pen, const QJsonObject &json)
{
    pen.setWidth(json["width"].toInt());
    pen.setStyle(static_cast<Qt::PenStyle>(json["style"].toInt()));
    QColor color;
    buildObject(color, json["color"].toObject());
    pen.setColor(color);
}

void CGraphicsJSON::buildObject(QBrush &brush, const QJsonObject &json)
{
    brush.setStyle(static_cast<Qt::BrushStyle>(json["style"].toInt()));
    QColor color;
    buildObject(color, json["color"].toObject());
    brush.setColor(color);
}

void CGraphicsJSON::buildObject(QFont &font, const QJsonObject &json)
{
    font.setFamily(json["family"].toString());
    font.setPointSize(json["pointSize"].toInt());
    font.setWeight(json["weight"].toInt());
    font.setBold(json["bold"].toBool());
    font.setItalic(json["italic"].toBool());
    font.setUnderline(json["underline"].toBool());
    font.setStrikeOut(json["strikeout"].toBool());
}
