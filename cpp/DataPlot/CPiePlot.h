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

#include <cassert>
#include "CPlot.h"

#define HISTORY 1 // number of items

class QwtPlotCurve;
class CPieMarker;

/** @cond INTERNAL */

/**
 * @brief The PiePlot class makes possible the pie chart creation
 * Inside this class are defined the exact number of curves that should be shown in the
 * pie chart.
 */
class DATAPLOTSHARED_EXPORT CPiePlot : public CPlot
{
    Q_OBJECT
    public:

        CPiePlot(QWidget* pParent = nullptr);
        ~CPiePlot();

        const QwtPlotCurve *pieCurve(size_t id) const
        {
            assert(id<m_data.size());
            return m_data[id].curve;
        }

        void addPie(const QString& name, double value, QColor color = QColor());

        size_t getNumberOfPie();

        virtual void            syncLegendView() override;

    private:

        struct pieCurves
        {
            QString name = "";
            QwtPlotCurve *curve = nullptr;
            double data = 0.0;
            double realValue = 0.0;
        };

        std::vector<pieCurves> m_data;
        double timeData = 0.0;
        CPieMarker* m_pie = nullptr;
};

/** @endcond */
