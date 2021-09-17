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

#include "CGraphicsText.h"
#include <QFont>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

//--------------------------
//- Class CProxyGraphicsText
//--------------------------
CProxyGraphicsText::CProxyGraphicsText()
    : CProxyGraphicsItem(GraphicsItem::TEXT)
{
}

CProxyGraphicsText::CProxyGraphicsText(const std::string &text)
    : CProxyGraphicsItem(GraphicsItem::TEXT)
{
    m_text = text;
}

CProxyGraphicsText::CProxyGraphicsText(const std::string &text, float x, float y)
    : CProxyGraphicsItem(GraphicsItem::TEXT)
{
    m_text = text;
    m_x = x;
    m_y = y;
}

CProxyGraphicsText::CProxyGraphicsText(const std::string &text, float x, float y, const CGraphicsTextProperty &property)
    : CProxyGraphicsItem(GraphicsItem::TEXT)
{
    m_text = text;
    m_x = x;
    m_y = y;
    m_property = property;
    m_bUseGlobalContext = false;
}

void CProxyGraphicsText::setX(float x)
{
    m_x = x;
}

void CProxyGraphicsText::setY(float y)
{
    m_y = y;
}

void CProxyGraphicsText::setText(const std::string& text)
{
    m_text = text;
}

void CProxyGraphicsText::setProperty(const CGraphicsTextProperty &prop)
{
    m_property = prop;
}

void CProxyGraphicsText::setCategory(const std::string &categ)
{
    m_property.m_category = categ;
}

float CProxyGraphicsText::getX() const
{
    return m_x;
}

float CProxyGraphicsText::getY() const
{
    return m_y;
}

std::string CProxyGraphicsText::getText() const
{
    return m_text;
}

QRectF CProxyGraphicsText::getBoundingRect() const
{
    CGraphicsTextProperty prop;
    if(m_bUseGlobalContext == false)
        prop = m_property;

    QFont font;
    font.setFamily(QString::fromStdString(prop.m_fontName));
    font.setPointSize(prop.m_fontSize);
    font.setBold(prop.m_bBold);
    font.setItalic(prop.m_bItalic);
    font.setUnderline(prop.m_bUnderline);
    font.setStrikeOut(prop.m_bStrikeOut);

    QFontMetrics fontMetrics(font);
    int width = fontMetrics.horizontalAdvance(QString::fromStdString(m_text));
    int height = fontMetrics.height();
    return QRect(m_x, m_y, width, height);
}

CGraphicsTextProperty CProxyGraphicsText::getProperty() const
{
    return m_property;
}

std::string CProxyGraphicsText::getCategory() const
{
    return m_property.m_category;
}

bool CProxyGraphicsText::isTextItem() const
{
    return true;
}

void CProxyGraphicsText::translate(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

void CProxyGraphicsText::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

std::shared_ptr<CProxyGraphicsItem> CProxyGraphicsText::clone() const
{
    return std::make_shared<CProxyGraphicsText>(*this);
}

void CProxyGraphicsText::toJson(QJsonObject &obj) const
{
    CProxyGraphicsItem::toJson(obj);
    obj["x"] = m_x;
    obj["y"] = m_y;
    obj["text"] = QString::fromStdString(m_text);
    obj["properties"] = CGraphicsJSON::toJsonObject(m_property);
}

//---------------------
//- Class CGraphicsText
//---------------------
CGraphicsText::CGraphicsText(QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(GraphicsItem::TEXT)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QFont font = qApp->font();
    font.setPointSize(12);
    setFont(font);
    setPlainText(tr("Text Area"));
}

CGraphicsText::CGraphicsText(const QString &text, QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(GraphicsItem::TEXT)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QFont font = qApp->font();
    font.setPointSize(12);
    setFont(font);
    setPlainText(text);
}

CGraphicsText::CGraphicsText(const QString &text, const CGraphicsTextProperty &property, QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(GraphicsItem::TEXT)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    QFont font;
    font.setFamily(QString::fromStdString(property.m_fontName));
    font.setPointSize(property.m_fontSize);
    font.setBold(property.m_bBold);
    font.setItalic(property.m_bItalic);
    font.setUnderline(property.m_bUnderline);
    font.setStrikeOut(property.m_bStrikeOut);
    setFont(font);

    setDefaultTextColor(Utils::Graphics::toQColor(property.m_color));
    setPlainText(text);
}

CGraphicsText::CGraphicsText(const QString& text, const CGraphicsTextProperty &property, QPointF position, QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(GraphicsItem::TEXT)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    QFont font;
    font.setFamily(QString::fromStdString(property.m_fontName));
    font.setPointSize(property.m_fontSize);
    font.setBold(property.m_bBold);
    font.setItalic(property.m_bItalic);
    font.setUnderline(property.m_bUnderline);
    font.setStrikeOut(property.m_bStrikeOut);
    setFont(font);

    setDefaultTextColor(Utils::Graphics::toQColor(property.m_color));
    setPlainText(text);
    setPos(position);
}

CGraphicsText::CGraphicsText(size_t id, const QString &text, const CGraphicsTextProperty &property, QPointF position, QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(id, GraphicsItem::TEXT)
{
    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    QFont font;
    font.setFamily(QString::fromStdString(property.m_fontName));
    font.setPointSize(property.m_fontSize);
    font.setBold(property.m_bBold);
    font.setItalic(property.m_bItalic);
    font.setUnderline(property.m_bUnderline);
    font.setStrikeOut(property.m_bStrikeOut);
    setFont(font);

    setDefaultTextColor(Utils::Graphics::toQColor(property.m_color));
    setPlainText(text);
    setPos(position);
}

CGraphicsText::CGraphicsText(const GraphicsContextPtr &globalContext, const std::shared_ptr<CProxyGraphicsText> &proxyItem, QGraphicsItem *pParent)
    : QGraphicsTextItem(pParent),
      CGraphicsItem(proxyItem->getId(), GraphicsItem::TEXT)
{
    CGraphicsTextProperty prop;
    if(proxyItem->isUsedGlobalContext())
        prop = globalContext->getTextProperty();
    else
        prop = proxyItem->m_property;

    setFlag(GraphicsItemFlag::ItemIsSelectable, true);
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setFlag(GraphicsItemFlag::ItemIgnoresTransformations);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    QFont font;
    font.setFamily(QString::fromStdString(prop.m_fontName));
    font.setPointSize(prop.m_fontSize);
    font.setBold(prop.m_bBold);
    font.setItalic(prop.m_bItalic);
    font.setUnderline(prop.m_bUnderline);
    font.setStrikeOut(prop.m_bStrikeOut);
    setFont(font);

    setDefaultTextColor(Utils::Graphics::toQColor(prop.m_color));
    setPlainText(QString::fromStdString(proxyItem->m_text));
    setPos(QPointF(proxyItem->m_x, proxyItem->m_y));
}

CGraphicsText::CGraphicsText(const CGraphicsText &text)
    : QGraphicsTextItem(text.parentItem()),
      CGraphicsItem(GraphicsItem::TEXT)
{
    copy(text);
}

CGraphicsText &CGraphicsText::operator=(const CGraphicsText &text)
{
    copy(text);
    return *this;
}

QGraphicsItem *CGraphicsText::clone() const
{
    return new CGraphicsText(*this);
}

void CGraphicsText::setPenColor(QColor color)
{
    setDefaultTextColor(color);
}

void CGraphicsText::setBrushColor(QColor color)
{
    Q_UNUSED(color);
}

int CGraphicsText::type() const
{
    return Type;
}

QByteArray CGraphicsText::getJsonData(CGraphicsJSON &jsonMgr) const
{
    return jsonMgr.getBinaryData(this);
}

void CGraphicsText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(200,200,200,150));
    painter->drawRect(boundingRect());
    QGraphicsTextItem::paint(painter, option, widget);
}

ProxyGraphicsItemPtr CGraphicsText::createProxyGraphicsItem() const
{
    std::string text = toPlainText().toStdString();

    CGraphicsTextProperty prop;
    QFont f = font();
    prop.m_fontName = f.family().toStdString();
    prop.m_fontSize = f.pointSize();
    prop.m_bBold = f.bold();
    prop.m_bItalic = f.italic();
    prop.m_bUnderline = f.underline();
    prop.m_bStrikeOut = f.strikeOut();
    prop.m_color = Utils::Graphics::toCColor(defaultTextColor());

    QPointF position = mapToScene(pos());

    return std::make_shared<CProxyGraphicsText>(text, position.x(), position.y(), prop);
}

void CGraphicsText::buildFromJsonData(CGraphicsJSON &jsonMgr, QByteArray data)
{
    jsonMgr.buildObject(this, data);
}

void CGraphicsText::insertToImage(CMat &image, CGraphicsConversion &filler, bool bForceFill, bool bBinary) const
{
    filler.insertToImage(image, this, bForceFill, bBinary);
}

int CGraphicsText::pixelToPointSize(int size)
{
    double dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
    return size * 72 / dpi;
}

void CGraphicsText::copy(const CGraphicsText &text)
{
    setPos(text.pos());
    setDefaultTextColor(text.defaultTextColor());
    setPlainText(text.toPlainText());
    setFont(text.font());
}
