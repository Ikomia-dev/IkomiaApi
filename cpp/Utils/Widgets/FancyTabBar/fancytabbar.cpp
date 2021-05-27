/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "fancytabbar.h"
#include <stylehelper.h>

#include <QMouseEvent>
#include <QCommonStyle>
#include <QPainter>
#include <QColor>
#include <QStackedLayout>
#include <QToolTip>

const int FancyTabBar::m_rounding = 22;
const int FancyTabBar::m_textPadding = 4;

FancyTabBar::FancyTabBar(const TabBarPosition position, QWidget *parent)
    : QWidget(parent), mPosition(position)
{
    mHoverIndex = -1;
    mCurrentIndex = -1;

    if (mPosition == FancyTabBar::Above || mPosition == FancyTabBar::Below)
    {
        setMinimumHeight(qMax(2 * m_rounding, 40));
        setMaximumHeight(tabSizeHint(false).height());
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }
    else
    {
        setMinimumWidth(qMax(2 * m_rounding, 40));
        setMaximumWidth(tabSizeHint(false).width());
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    }

    setStyle(new QCommonStyle);
    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(true); // Needed for hover events
    mTimerTriggerChangedSignal.setSingleShot(true);

    // We use a zerotimer to keep the sidebar responsive
    connect(&mTimerTriggerChangedSignal, SIGNAL(timeout()), this, SLOT(emitCurrentIndex()));
}

FancyTabBar::~FancyTabBar()
{
    delete style();
}

QSize FancyTabBar::tabSizeHint(bool minimum) const
{
    QFont boldFont(font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    QFontMetrics fm(boldFont);
    int spacing = 8;
    int width = 60 + spacing + 2;
    int maxLabelwidth = 0;
    for (int tab=0 ; tab<count() ;++tab) {
        int width = fm.width(tabText(tab));
        if (width > maxLabelwidth)
            maxLabelwidth = width;
    }
    int iconHeight = minimum ? 0 : 32;

    return QSize(qMax(width, maxLabelwidth + 4), iconHeight + spacing + fm.height());
}

QPoint FancyTabBar::getCorner(const QRect& rect, const Corner corner) const
{
    if (mPosition == Above)
    {
        if (corner == OutsideBeginning)
            return rect.topLeft();
        if (corner == OutsideEnd)
            return rect.topRight();
        if (corner == InsideBeginning)
            return rect.bottomLeft();
        if (corner == InsideEnd)
            return rect.bottomRight();
    }
    else if (mPosition == Below)
    {
        if (corner == OutsideBeginning)
            return rect.bottomLeft();
        if (corner == OutsideEnd)
            return rect.bottomRight();
        if (corner == InsideBeginning)
            return rect.topLeft();
        if (corner == InsideEnd)
            return rect.topRight();
    }
    else if (mPosition == Left)
    {
        if (corner == OutsideBeginning)
            return rect.topLeft();
        if (corner == OutsideEnd)
            return rect.bottomLeft();
        if (corner == InsideBeginning)
            return rect.topRight();
        if (corner == InsideEnd)
            return rect.bottomRight();
    }
    else if (mPosition == Right)
    {
        if (corner == OutsideBeginning)
            return rect.topRight();
        if (corner == OutsideEnd)
            return rect.bottomRight();
        if (corner == InsideBeginning)
            return rect.topLeft();
        if (corner == InsideEnd)
            return rect.bottomLeft();
    }

    Q_ASSERT("that's impossible!");

    return QPoint();
}

QRect FancyTabBar::adjustRect(const QRect& rect, const qint8 offsetOutside, const qint8 offsetInside, const qint8 offsetBeginning, const qint8 offsetEnd) const
{
    if (mPosition == Above)
        return rect.adjusted(-offsetBeginning, -offsetOutside, offsetEnd, offsetInside);
    else if (mPosition == Below)
        return rect.adjusted(-offsetBeginning, -offsetInside, -offsetBeginning, offsetOutside);
    else if (mPosition == Left)
        return rect.adjusted(-offsetOutside, -offsetBeginning, offsetInside, offsetEnd);
    else if (mPosition == Right)
        return rect.adjusted(-offsetInside, -offsetBeginning, offsetOutside, offsetEnd);

    Q_ASSERT("that's impossible!");
    return QRect();
}

// Same with a point: + means towards Outside/End, - means towards Inside/Beginning
QPoint FancyTabBar::adjustPoint(const QPoint& point, const qint8 offsetInsideOutside, const qint8 offsetBeginningEnd) const
{
    if (mPosition == Above)
        return point + QPoint(offsetBeginningEnd, -offsetInsideOutside);
    else if (mPosition == Below)
        return point + QPoint(offsetBeginningEnd, offsetInsideOutside);
    else if (mPosition == Left)
        return point + QPoint(-offsetInsideOutside, offsetBeginningEnd);
    else if (mPosition == Right)
        return point + QPoint(offsetInsideOutside, offsetBeginningEnd);

    Q_ASSERT("that's impossible!");
    return QPoint();
}

void FancyTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // paint background
    QRect rectangle = adjustRect(rect(), 0, -1, 0, 0);
    QLinearGradient lg;

    lg.setStart(getCorner(rectangle, OutsideBeginning));
    lg.setFinalStop(getCorner(rectangle, InsideBeginning));
    lg.setColorAt(0.0, QColor(64, 64, 64, 255));
    lg.setColorAt(1.0, QColor(130, 130, 130, 255));
    painter.fillRect(rectangle, lg);

    // draw dark widget bordert on inner inside (e.g. bottom if the widget position is top)
    painter.setPen(StyleHelper::borderColor());
    painter.drawLine(adjustPoint(getCorner(rectangle, InsideBeginning), -1, 0), adjustPoint(getCorner(rectangle, InsideEnd), -1, 0));

    // draw bright widget border on outer inside (e.g. bottom if the widget position is top)
    painter.setPen(StyleHelper::sidebarHighlight());
    painter.drawLine(getCorner(rectangle, InsideBeginning), getCorner(rectangle, InsideEnd));

    // paint inactive tabs
    for (int i = 0; i < count(); ++i)
        if (i != currentIndex())
            paintTab(&painter, i);

    // paint active tab last, since it overlaps the neighbors
    if (currentIndex() != -1)
        paintTab(&painter, currentIndex());
}

// Handle hover events for mouse fade ins
void FancyTabBar::mouseMoveEvent(QMouseEvent *e)
{
    int newHover = -1;
    for (int i = 0; i < count(); ++i)
    {
        QRect area = tabRect(i);
        if (area.contains(e->pos())) {
            newHover = i;
            break;
        }
    }
    if (newHover == mHoverIndex)
        return;

    if (validIndex(mHoverIndex))
        mAttachedTabs[mHoverIndex]->fadeOut();

    mHoverIndex = newHover;

    if (validIndex(mHoverIndex)) {
        mAttachedTabs[mHoverIndex]->fadeIn();
        mHoverRect = tabRect(mHoverIndex);
    }
}

bool FancyTabBar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        if (validIndex(mHoverIndex)) {
            QString tt = tabToolTip(mHoverIndex);
            if (!tt.isEmpty()) {
                QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tt, this);
                return true;
            }
        }
    }
    return QWidget::event(event);
}

// Resets hover animation on mouse enter
void FancyTabBar::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    mHoverRect = QRect();
    mHoverIndex = -1;
}

// Resets hover animation on mouse leave
void FancyTabBar::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    mHoverIndex = -1;
    mHoverRect = QRect();
    for (int i = 0 ; i < mAttachedTabs.count() ; ++i) {
        mAttachedTabs[i]->fadeOut();
    }
}

QSize FancyTabBar::sizeHint() const
{
    QSize sh = tabSizeHint();
//    return QSize(sh.width(), sh.height() * mAttachedTabs.count());

    if (mPosition == Above || mPosition == Below)
        return QSize(sh.width() * mAttachedTabs.count(), sh.height());
    else
        return QSize(sh.width(), sh.height() * mAttachedTabs.count());
}

QSize FancyTabBar::minimumSizeHint() const
{
    QSize sh = tabSizeHint(true);
//    return QSize(sh.width(), sh.height() * mAttachedTabs.count());

    if(mPosition == Above || mPosition == Below)
        return QSize(sh.width() * mAttachedTabs.count(), sh.height());
    else
        return QSize(sh.width(), sh.height() * mAttachedTabs.count());
}

QRect FancyTabBar::tabRect(int index) const
{
    QSize sh = tabSizeHint();

    if(mPosition == Above || mPosition == Below)
    {
        if (sh.width() * mAttachedTabs.count() > width())
            sh.setWidth(width() / mAttachedTabs.count());

        return QRect(index * sh.width(), 0, sh.width(), sh.height());
    }
    else
    {
        if (sh.height() * mAttachedTabs.count() > height())
            sh.setHeight(height() / mAttachedTabs.count());

        return QRect(0, index * sh.height(), sh.width(), sh.height());
    }

}

// This keeps the sidebar responsive since
// we get a repaint before loading the
// mode itself
void FancyTabBar::emitCurrentIndex()
{
    emit currentChanged(mCurrentIndex);
}

void FancyTabBar::mousePressEvent(QMouseEvent *e)
{
    e->accept();
    for (int index = 0; index < mAttachedTabs.count(); ++index)
    {
        if (tabRect(index).contains(e->pos()))
        {
            if (isTabEnabled(index))
            {
                mCurrentIndex = index;
                update();
                mTimerTriggerChangedSignal.start(0);
            }
            break;
        }
    }
}

void FancyTabBar::paintTab(QPainter *painter, int tabIndex) const
{
    if (!validIndex(tabIndex))
    {
        qWarning("invalid index");
        return;
    }
    painter->save();

    QRect rect = tabRect(tabIndex);
    bool selected = (tabIndex == mCurrentIndex);
    bool enabled = isTabEnabled(tabIndex);

    if(selected)
    {
        // background
        painter->save();
        QLinearGradient grad(getCorner(rect, OutsideBeginning), getCorner(rect, InsideBeginning));
        grad.setColorAt(0, QColor(255, 255, 255, 140));
        grad.setColorAt(1, QColor(255, 255, 255, 210));
        painter->fillRect(adjustRect(rect, 0, 0, 0, -1), grad);
        painter->restore();

        // shadows (the black lines immediately before/after (active && selected)-backgrounds)
        painter->setPen(QColor(0, 0, 0, 110));
        painter->drawLine(adjustPoint(getCorner(rect, OutsideBeginning), 0, -1), adjustPoint(getCorner(rect, InsideBeginning), 0, -1));
        painter->drawLine(getCorner(rect, OutsideEnd), getCorner(rect, InsideEnd));

        // thin shadow on the outside of active tab
        painter->setPen(QColor(0, 0, 0, 40));
        painter->drawLine(getCorner(rect, OutsideBeginning), getCorner(rect, OutsideEnd));

        // highlights
        painter->setPen(QColor(255, 255, 255, 50));
        painter->drawLine(adjustPoint(getCorner(rect, OutsideBeginning), 0, -2), adjustPoint(getCorner(rect, InsideBeginning), 0, -2));
        painter->drawLine(adjustPoint(getCorner(rect, OutsideEnd), 0, 1), adjustPoint(getCorner(rect, InsideEnd), 0, 1));

        painter->setPen(QColor(255, 255, 255, 40));
        // thin white line towards beginning
        painter->drawLine(adjustPoint(getCorner(rect, OutsideBeginning), 0, 0), adjustPoint(getCorner(rect, InsideBeginning), 0, 0));
        // thin white line on inside border
        painter->drawLine(adjustPoint(getCorner(rect, InsideBeginning), 0, 1), adjustPoint(getCorner(rect, InsideEnd), 0, -1));
        // thin white line towards end
        painter->drawLine(adjustPoint(getCorner(rect, OutsideEnd), 0, -1), adjustPoint(getCorner(rect, InsideEnd), 0, -1));
    }

    QString tabText(this->tabText(tabIndex));
    QRect tabTextRect(rect);
    const bool drawIcon = rect.height() > 36;
    QRect tabIconRect(tabTextRect);
    tabTextRect.translate(0, drawIcon ? -2 : 1);
    QFont boldFont(painter->font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->setPen(selected ? QColor(255, 255, 255, 160) : QColor(0, 0, 0, 110));
    const int textFlags = Qt::AlignCenter | (drawIcon ? Qt::AlignBottom : Qt::AlignVCenter) | Qt::TextWordWrap;
    if (enabled) {
        painter->drawText(tabTextRect, textFlags, tabText);
        painter->setPen(selected ? QColor(60, 60, 60) : StyleHelper::panelTextColor());
    } else {
        painter->setPen(selected ? StyleHelper::panelTextColor() : QColor(255, 255, 255, 120));
    }

#if defined(Q_OS_MAC)
    bool isMac=true;
#else
    bool isMac = false;
#endif

    // hover
    if(!isMac && !selected && enabled)
    {
        painter->save();
        int fader = int(mAttachedTabs[tabIndex]->fader());
        QLinearGradient grad(getCorner(rect, OutsideBeginning), getCorner(rect, InsideBeginning));

        grad.setColorAt(0, Qt::transparent);
        grad.setColorAt(0.5, QColor(255, 255, 255, fader));
        grad.setColorAt(1, Qt::transparent);
        painter->fillRect(rect, grad);
        painter->setPen(QPen(grad, 1.0));

        if(mPosition == Above || mPosition == Below)
        {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
            painter->drawLine(rect.topRight(), rect.bottomRight());
        }
        else
        {
            painter->drawLine(rect.topLeft(), rect.topRight());
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        }

        painter->restore();
    }

    if (!enabled)
        painter->setOpacity(0.7);

    if (drawIcon) {
        int textHeight = painter->fontMetrics().boundingRect(QRect(0, 0, width(), height()), Qt::TextWordWrap, tabText).height();
        tabIconRect.adjust(0, 4, 0, -textHeight);
        StyleHelper::drawIconWithShadow(tabIcon(tabIndex), tabIconRect, painter, enabled ? QIcon::Normal : QIcon::Disabled);
    }

    painter->translate(0, -1);
    painter->drawText(tabTextRect, textFlags, tabText);
    painter->restore();
}

void FancyTabBar::setCurrentIndex(int index) {
    if (isTabEnabled(index)) {
        mCurrentIndex = index;
        update();
        emit currentChanged(mCurrentIndex);
    }
}

void FancyTabBar::setTabEnabled(int index, bool enable)
{
    Q_ASSERT(index < mAttachedTabs.size());
    Q_ASSERT(index >= 0);

    if (index < mAttachedTabs.size() && index >= 0) {
        mAttachedTabs[index]->enabled = enable;
        update(tabRect(index));
    }
}

bool FancyTabBar::isTabEnabled(int index) const
{
    Q_ASSERT(index < mAttachedTabs.size());
    Q_ASSERT(index >= 0);

    if (index < mAttachedTabs.size() && index >= 0)
        return mAttachedTabs[index]->enabled;

    return false;
}
