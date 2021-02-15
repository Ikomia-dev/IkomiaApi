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

//-----------------------------------------------------------------
// This class shows how to extend QwtPlotItems. It displays a
// pie chart of user/total/idle cpu usage in percent.
//-----------------------------------------------------------------

#include <qwt_plot_item.h>

/** @cond INTERNAL */

/**
 * @brief The PieMarker class creates the pie chart
 */
class CPieMarker: public QwtPlotItem
{
    private:
        int height = 300;
        int width = 300;
        int margin = 10;
        int numPlots = 0;

    public:
        CPieMarker();
        CPieMarker(int numPlots, int size);

        virtual int rtti() const;

        virtual void draw(QPainter *p, const QwtScaleMap &, const QwtScaleMap &, const QRectF &rect) const;

        void setNumberOfPlots(int totalPlots);
};

/** @endcond */
