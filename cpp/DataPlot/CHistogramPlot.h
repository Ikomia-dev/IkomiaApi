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

#ifndef CHISTOGRAMPLOT_H
#define CHISTOGRAMPLOT_H

#include "CPlot.h"

class QwtLegend;
class CHistogram;

class DATAPLOTSHARED_EXPORT CHistogramPlot : public CPlot
{
    public:
        CHistogramPlot(QWidget* pParent = nullptr);

        void            addHistogram(QVector<double> data, const QString& title = QString(), const QColor& color = QColor());

        virtual void    setPlotName(const QString &name, int id  = 0) override;
        virtual void    setColor(const QColor& color, int id = 0) override;
        virtual QColor  getColor(int id = 0) override;
        virtual int     getNumberOfPlots() override;
        virtual void    setGrid(bool bEnable) override;
        virtual QStringList getPlotTitles() override;

        virtual void    syncLegendView() override;
    private:
        void populate();

    private:
        QVector<QVector<double>>    m_histograms;
        QList<QString>              m_titles;
        QList<QColor>               m_colors;
        QList<CHistogram*>          m_histoList;
};

#endif // CHISTOGRAMPLOT_H
