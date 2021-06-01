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

#ifndef CCURVEPLOT_H
#define CCURVEPLOT_H

#include "CPlot.h"

class QwtPlotIntervalCurve;

class DATAPLOTSHARED_EXPORT CCurvePlot : public CPlot
{
    Q_OBJECT
    public:
        enum Mode
        {
            Bars,
            Tube
        };

        CCurvePlot(QWidget* pParent = nullptr);

        QwtPlotCurve*           addPlot(const QString& name, std::vector<double> x, std::vector<double> y, int thickness = 4,  QColor color = QColor(), const QString& symbol = "");
        QwtPlotIntervalCurve*   addErrorBars(const QString& name, std::vector<double> x, std::vector<double> ymin, std::vector<double> ymax, const QColor& colorPen = Qt::blue, QColor colorBrush = Qt::blue);

        void                    removePlot(QwtPlotCurve* pCurve);
        void                    removeErrorBar(QwtPlotIntervalCurve* pIntervalCurve);

        void                    setCurveArea(bool bEnable);
        void                    setErrorBarMode(int mode);
        void                    setThickness(int thickness, int id = 0);
        void                    setLineType(const QString& type, int id = 0);
        void                    setMarker(const QString& marker, QColor brushColor = QColor(), QColor penColor = QColor(), int id = 0);

        int                     getThickness(int id = 0);
        QString                 getLineType(int id = 0);
        QString                 getMarker(int id = 0);
        QColor                  getMarkerBrushColor(int id = 0);
        QColor                  getMarkerPenColor(int id = 0);

        virtual void            setPlotName(const QString& name, int id = 0) override;
        virtual void            setColor(const QColor &color, int id = 0) override;
        virtual QColor          getColor(int id = 0) override;
        virtual QStringList     getPlotTitles() override;
        virtual int             getNumberOfPlots() override;
        virtual void            syncLegendView() override;

    protected:
        QList<QwtPlotCurve*>            m_curveList;
        QList<QwtPlotIntervalCurve*>    m_pIntervalCurveList;
        QStringList                     m_titles;
        bool                            m_bArea = false;
};

#endif // CCURVEPLOT_H
