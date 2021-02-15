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

#include "CPlot.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include "UtilsTools.hpp"

CPlot::CPlot(QWidget* pParent) : QwtPlot(pParent)
{
    setObjectName("CPlot");
    m_colorList = Utils::Color::getColorNameCSS();
    setAutoReplot( true );

    setAxisTitle( QwtPlot::yLeft, "y" );
    setAxisTitle( QwtPlot::xBottom, "x" );
}

void CPlot::setGrid(bool bEnable)
{
    if(m_bGrid == bEnable)
        return;

    if(m_pGrid == nullptr)
        m_pGrid = new QwtPlotGrid();

    if(bEnable == true)
        m_pGrid->attach( this );
    else
        m_pGrid->detach();

    m_bGrid = bEnable;
}

void CPlot::setLegend(bool bEnable)
{
    if(m_bLegend == bEnable)
        return;

    if(m_pLegend == nullptr)
        m_pLegend = new QwtLegend;

    connect( this, &CPlot::legendDataChanged, m_pLegend, &QwtLegend::updateLegend);

    if(bEnable == true)
    {
        insertLegend(m_pLegend, m_legendPos);
        setLegendItemMode(QwtLegendData::Checkable);
        connect( m_pLegend, &QwtLegend::checked, this, &CPlot::onShowItem);
        updateLegend();
    }
    else
    {
        m_pLegend->disconnect();
        // InsertLegend already delete pointer if different
        insertLegend(nullptr);
        m_pLegend = nullptr;       
    }

    m_bLegend = bEnable;

    syncLegendView();
}

void CPlot::setLegendPos(QwtPlot::LegendPosition position)
{
    assert(position <= QwtPlot::TopLegend);

    if(m_legendPos == position)
        return;

    m_legendPos = position;

    if(m_bLegend)
    {
        delete m_pLegend;
        m_pLegend = new QwtLegend;
        insertLegend(m_pLegend, m_legendPos);
    }
}

void CPlot::setLegendItemMode(QwtLegendData::Mode mode)
{
    if(m_pLegend)
        m_pLegend->setDefaultItemMode(mode);
}

void CPlot::setInternalLegend(bool bEnable)
{
    if(m_bLegendIn == bEnable)
        return;

    if(m_pLegendItem == nullptr)
        m_pLegendItem = new CLegendItem;

    if(m_pLegendItemParam == nullptr)
        m_pLegendItemParam = new CLegendItemParam;

    if(bEnable == true)
    {
        m_pLegendItem->attach(this);
        updateInternalLegend();
    }
    else
    {
        m_pLegendItem->detach();
        delete m_pLegendItem;
        m_pLegendItem = nullptr;
    }

    m_bLegendIn = bEnable;
}

void CPlot::setInternalLegendParam(int maxCols, Qt::Alignment align, QwtPlotLegendItem::BackgroundMode bgMode, int size)
{
    m_pLegendItemParam->m_maxCols = maxCols;
    m_pLegendItemParam->m_align = align;
    m_pLegendItemParam->m_bgMode = bgMode;
    m_pLegendItemParam->m_size = size;

    updateInternalLegend();
}

void CPlot::setZoomable(bool bEnable)
{
    m_bZoom = bEnable;
    if(m_bZoom)
    {
        assert(m_pZoomer == nullptr);
        m_pZoomer = new QwtPlotZoomer( canvas() );
        m_pZoomer->setRubberBandPen( QColor( Qt::black ) );
        m_pZoomer->setTrackerPen( QColor( Qt::black ) );
        m_pZoomer->setMousePattern( QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier );
        m_pZoomer->setMousePattern( QwtEventPattern::MouseSelect3,
            Qt::RightButton );
    }
    else
    {
        if(m_pZoomer)
        {
            delete m_pZoomer;
            m_pZoomer = nullptr;
        }
    }
}

void CPlot::setMovable(bool bEnable)
{
    m_bPan = bEnable;
    if(m_bPan)
    {
        assert(m_pPanner == nullptr);
        m_pPanner = new QwtPlotPanner( canvas() );
        m_pPanner->setMouseButton( Qt::MidButton );
    }
    else
    {
        if(m_pPanner)
        {
            delete m_pPanner;
            m_pPanner = nullptr;
        }
    }

}

void CPlot::setBackgroundColor(const QColor& color)
{
    setCanvasBackground(QBrush(color));
}

void CPlot::setPlotName(const QString& , int )
{

}

QwtPlot::LegendPosition CPlot::getLegendPos()
{
    return m_legendPos;
}

CLegendItemParam* CPlot::getInternalLegendParam()
{
    return m_pLegendItemParam;
}

bool CPlot::hasGrid()
{
    return m_bGrid;
}

bool CPlot::hasLegend()
{
    return m_bLegend;
}

bool CPlot::hasInternalLegend()
{
    return m_bLegendIn;
}

bool CPlot::isZoomable()
{
    return m_bZoom;
}

bool CPlot::isMovable()
{
    return m_bPan;
}

void CPlot::updateInternalLegend()
{
    m_pLegendItem->setMaxColumns( m_pLegendItemParam->m_maxCols );
    m_pLegendItem->setAlignment( Qt::Alignment( m_pLegendItemParam->m_align ) );
    m_pLegendItem->setBackgroundMode( QwtPlotLegendItem::BackgroundMode( m_pLegendItemParam->m_bgMode ) );
    if ( m_pLegendItemParam->m_bgMode == QwtPlotLegendItem::ItemBackground )
    {
        m_pLegendItem->setBorderRadius( 4 );
        m_pLegendItem->setMargin( 0 );
        m_pLegendItem->setSpacing( 4 );
        m_pLegendItem->setItemMargin( 2 );
    }
    else
    {
        m_pLegendItem->setBorderRadius( 8 );
        m_pLegendItem->setMargin( 4 );
        m_pLegendItem->setSpacing( 2 );
        m_pLegendItem->setItemMargin( 0 );
    }

    QFont font = m_pLegendItem->font();
    font.setPointSize( m_pLegendItemParam->m_size );
    m_pLegendItem->setFont( font );
}

void CPlot::exportChart(const QString& path)
{
    QwtPlotRenderer renderer;
    renderer.exportTo(this, path);
}

void CPlot::setColor(const QColor&, int)
{

}

QColor CPlot::getColor(int)
{
    return QColor();
}

int CPlot::getNumberOfPlots()
{
    return 0;
}

QStringList CPlot::getPlotTitles()
{
    return QStringList();
}

void CPlot::syncLegendView()
{
    if(m_pLegend == nullptr)
        return;

    replot(); // creating the legend items

    // Override this function and choose the right QwtPlotItem instead of Rtti_PlotItem
    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotItem );
    for ( int i = 0; i < items.size(); i++ )
    {
        const QVariant itemInfo = itemToInfo( items[i] );

        QwtLegendLabel *legendLabel =
            qobject_cast<QwtLegendLabel *>( m_pLegend->legendWidget( itemInfo ) );
        if ( legendLabel )
            legendLabel->setChecked( true );

        items[i]->setVisible( true );
    }
}

void CPlot::onShowItem(const QVariant& itemInfo, bool on)
{
    QwtPlotItem *plotItem = infoToItem( itemInfo );
    if ( plotItem )
        plotItem->setVisible( on );
}

void CPlot::onSetTitle(const QString& title)
{
    setTitle(title);
}

void CPlot::onSetLabelX(const QString& text)
{
    setAxisTitle( QwtPlot::xBottom, text );
}

void CPlot::onSetLabelY(const QString& text)
{
    setAxisTitle( QwtPlot::yLeft, text );
}
