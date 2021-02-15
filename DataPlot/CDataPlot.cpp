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

#include "CDataPlot.h"

CDataPlot::CDataPlot()
{
}

CCurvePlot* CDataPlot::createCurvePlot()
{
    auto pPlot = new CCurvePlot;
    if(!m_dataList.empty())
    {      
        for(size_t k=0; k<m_dataList.size(); ++k)
        {
            std::vector<double> x;
            for(size_t i=0; i<m_dataList[k].size(); ++i)
                x.push_back(i);           
            pPlot->addPlot("Plot", x, m_dataList[k]);
        }
    }
    return pPlot;
}

CPiePlot* CDataPlot::createPiePlot()
{
    auto pPlot = new CPiePlot;
    if(!m_stringList.empty() && !m_dataList.empty())
    {
        // A modifier si on veut plusieurs PieChart
        for(size_t k=0; k<m_dataList.size(); ++k)
            for(size_t i=0; i<m_dataList[k].size(); ++i)
                pPlot->addPie(QString::fromStdString(m_stringList[k][i]), m_dataList[k][i]);
    }
    return pPlot;
}

CSimpleBarPlot* CDataPlot::createSimpleBarPlot()
{
    auto pPlot = new CSimpleBarPlot;
    if(!m_stringList.empty() && !m_dataList.empty())
    {
        for(size_t k=0; k<m_dataList.size(); ++k)
        {
            QVector<double> data = QVector<double>::fromStdVector(m_dataList[k]);
            QStringList titles;
            for(auto&& it : m_stringList[k])
                titles << QString::fromStdString(it);

            pPlot->addData(data, titles);
        }
    }
    return pPlot;
}

CMultiBarPlot* CDataPlot::createMultiBarPlot()
{
    auto pPlot = new CMultiBarPlot;
    if(!m_dataList.empty())
    {
        for(size_t k=0; k<m_dataList.size(); ++k)
        {
            QVector<double> data = QVector<double>::fromStdVector(m_dataList[k]);
            pPlot->addSerie(data);
        }
    }
    return pPlot;
}

CHistogramPlot* CDataPlot::createHistogramPlot()
{
    auto pPlot = new CHistogramPlot;
    if(!m_dataList.empty())
    {
        for(size_t k=0; k<m_dataList.size(); ++k)
        {
            QVector<double> data = QVector<double>::fromStdVector(m_dataList[k]);
            pPlot->addHistogram(data, "Histogram");
        }
    }
    return pPlot;
}

CPlot* CDataPlot::create()
{
    switch(m_type)
    {
        case Curve:
            return createCurvePlot();
        case Bar:
            return createSimpleBarPlot();
        case MultiBar:
            return createMultiBarPlot();
        case Histogram:
            return createHistogramPlot();
        case Pie:
            return createPiePlot();
        default:
            return nullptr;
    }
}

void CDataPlot::setType(CDataPlot::Type type)
{
    m_type = type;
}

void CDataPlot::setValueList(const std::vector<std::vector<double> >& valueList)
{
    m_dataList = valueList;
}

void CDataPlot::setStringList(const std::vector<std::vector<std::string> >& stringList)
{
    m_stringList = stringList;
}

void CDataPlot::addValueList(const std::vector<double>& values)
{
    m_dataList.push_back(values);
}

void CDataPlot::addStringList(const std::vector<std::string>& values)
{
    m_stringList.push_back(values);
}

CDataPlot::Type CDataPlot::getType()
{
    return m_type;
}
