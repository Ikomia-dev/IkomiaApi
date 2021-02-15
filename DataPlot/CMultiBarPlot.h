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

#ifndef CBARPLOT_H
#define CBARPLOT_H

#include "CPlot.h"
#include <qwt_plot_multi_barchart.h>
#include <qwt_column_symbol.h>

class CPlotMultiBarChart : public QwtPlotMultiBarChart
{
    public:
        const QColor& getSymbolColor(int id) const
        {
            return symbol(id)->palette().color(QPalette::Window);
        }

        void setSymbolColor(QColor color, int id)
        {
            symbol(id)->setPalette(QPalette(color));
        }
};

class DATAPLOTSHARED_EXPORT CMultiBarPlot : public CPlot
{
    public:
        CMultiBarPlot(QWidget* pParent = nullptr);

        void    setMode(int mode);
        void    setOrientation(Qt::Orientation orient);

        int     getNumberOfSeries();

        void    addSerie(QVector<double> serie, const QStringList& titles = QStringList());
        void    removeSerie(int i);

        virtual void    setPlotName(const QString &name, int id  = 0) override;
        virtual void    setColor(const QColor& color, int id = 0) override;
        virtual QColor  getColor(int id = 0) override;
        virtual QStringList getPlotTitles() override;

    private:
        void    populate();
        void    updateAxis();
        void    initBar();

    private:
        CPlotMultiBarChart*         m_pMultiBar = nullptr;
        QwtPlot::Axis               m_axisX;
        QwtPlot::Axis               m_axisY;
        QVector< QVector<double> >  m_series;
        QList<QwtText>              m_serieTitles;
        int                         m_maxBar = 0;
};

#endif // CBARPLOT_H
