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

#ifndef DATAPLOT_H
#define DATAPLOT_H

#include "DataPlotGlobal.h"
#include "CCurvePlot.h"
#include "CPiePlot.h"
#include "CMultiBarPlot.h"
#include "CSimpleBarPlot.h"
#include "CHistogramPlot.h"
#include <unordered_map>

class DATAPLOTSHARED_EXPORT CDataPlot
{

    public:
        enum Type
        {
            Curve,
            Bar,
            MultiBar,
            Histogram,
            Pie
        };
        CDataPlot();

        CCurvePlot*     createCurvePlot();
        CPiePlot*       createPiePlot();
        CSimpleBarPlot* createSimpleBarPlot();
        CMultiBarPlot*  createMultiBarPlot();
        CHistogramPlot* createHistogramPlot();
        CPlot*          create();

        void            setType(Type type);
        void            setValueList(const std::vector<std::vector<double>>& valueList);
        void            setStringList(const std::vector<std::vector<std::string>>& stringList);
        void            addValueList(const std::vector<double>& values);
        void            addStringList(const std::vector<std::string>& values);
        Type            getType();

    private:
        Type                                    m_type;
        std::vector<std::vector<double>>        m_dataList;
        std::vector<std::vector<std::string>>   m_stringList;
};

#endif // DATAPLOT_H
