/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CSIMPLEBARPLOT_H
#define CSIMPLEBARPLOT_H

#include "CPlot.h"

class CPlotBarChart;

class DATAPLOTSHARED_EXPORT CSimpleBarPlot : public CPlot
{
    public:
        CSimpleBarPlot(QWidget* pParent = nullptr);

        void setOrientation(Qt::Orientation orient);

        Qt::Orientation getOrientation();

        void addData(const QVector<double>& data, const QStringList& titles = QStringList(), const QList<QColor>& colors = {});

        virtual void    setPlotName(const QString &name, int id  = 0) override;
        virtual void    setColor(const QColor& color, int id = 0) override;
        virtual QColor  getColor(int id = 0) override;
        virtual QStringList getPlotTitles() override;

    private:
        void updateAxis();

    private:
        CPlotBarChart*              m_pSimpleBar = nullptr;
        QwtPlot::Axis               m_axisX;
        QwtPlot::Axis               m_axisY;
        QStringList                 m_titles;
};

#endif // CSIMPLEBARPLOT_H
