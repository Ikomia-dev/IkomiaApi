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

#include "CColorPushButton.h"
#include <QApplication>
#include <QColorDialog>

CColorPushButton::CColorPushButton(const QString& title, const QColor& color, QWidget *parent) : QPushButton(parent)
{
    m_title = title;
    m_color = color;

    init();
    initConnections();
}

QColor CColorPushButton::getColor() const
{
    return m_color;
}

void CColorPushButton::setColor(const QColor &color)
{
    m_color = color;
    updateColor();
}

void CColorPushButton::init()
{
    auto pal = qApp->palette();
    auto hoverBorderColor = pal.light().color();

    setFlat(true);
    setToolTip(m_title);
    setStyleSheet(QString("QPushButton { background-color: %1; border: none; } QPushButton:hover {border: 1px solid %2;}")
                  .arg(m_color.name())
                  .arg(hoverBorderColor.name()));
}

void CColorPushButton::initConnections()
{
    connect(this, &CColorPushButton::clicked, [&]
    {
        QColor newColor = QColorDialog::getColor(m_color, nullptr, tr("Color choice"));
        if(newColor.isValid() && newColor != m_color)
        {
            m_color = newColor;
            updateColor();
            emit colorChanged(m_color);
        }
    });
}

void CColorPushButton::updateColor()
{
    auto pal = qApp->palette();
    auto hoverBorderColor = pal.light().color();
    setStyleSheet(QString("QPushButton { background-color: %1; border: none; } QPushButton:hover {border: 1px solid %2;}")
                  .arg(m_color.name())
                  .arg(hoverBorderColor.name()));
}

#include "moc_CColorPushButton.cpp"
