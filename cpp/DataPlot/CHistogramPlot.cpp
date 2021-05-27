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

#include "CHistogramPlot.h"
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_grid.h>
#include <qwt_legend_label.h>

class CHistogram: public QwtPlotHistogram
{
    public:
        CHistogram( const QString& title, const QColor& symbolColor ) : QwtPlotHistogram(title)
        {
            setStyle( QwtPlotHistogram::Columns );
            setColor( symbolColor );

            m_pSymbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
            m_pSymbol->setFrameStyle( QwtColumnSymbol::Raised );
            m_pSymbol->setLineWidth( 2 );
            m_pSymbol->setPalette( QPalette( brush().color() ) );

            setSymbol( m_pSymbol );
        }

        void setColor( const QColor& color )
        {
            QColor c = color;
            c.setAlpha( 180 );
            setBrush( QBrush( c ) );
        }
        void setValues( const QVector<double>& values)
        {
            QVector<QwtIntervalSample> samples( values.size() );
            for ( int i = 0; i < values.size(); i++ )
            {
                QwtInterval interval( double( i ), i + 1.0 );
                interval.setBorderFlags( QwtInterval::ExcludeMaximum );

                samples[i] = QwtIntervalSample( values[i], interval );
            }

            setData( new QwtIntervalSeriesData( samples ) );
        }
        void setSymbolColor(const QColor& color)
        {
            setColor(color);
            m_pSymbol->setPalette( QPalette( brush().color()) );
        }

    private:
        QwtColumnSymbol* m_pSymbol = nullptr;
};

CHistogramPlot::CHistogramPlot(QWidget* pParent) : CPlot(pParent)
{
    /*QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::gray );
    canvas->setBorderRadius( 10 );
    setCanvas( canvas );*/

    plotLayout()->setAlignCanvasToScales( true );
}

void CHistogramPlot::addHistogram(QVector<double> data, const QString& title, const QColor& color)
{
    m_histograms += data;

    if(title.isEmpty())
        m_titles += tr("No title");
    else
        m_titles += title;

    if(color.isValid())
        m_colors += color;
    else
        m_colors += QColor(QString::fromStdString(m_colorList[qrand()%m_colorList.size()]));

    populate();
}

void CHistogramPlot::setPlotName(const QString& name, int id)
{
    assert(id>-1 && id<m_histoList.size() && id<m_titles.size());
    m_histoList[id]->setTitle(name);
    m_titles[id] = name;
}

void CHistogramPlot::setColor(const QColor& color, int id)
{
    assert(id>-1 && id<m_histoList.size() && id<m_colors.size());
    m_colors[id] = color;
    m_histoList[id]->setSymbolColor(color);
    replot();
}

QColor CHistogramPlot::getColor(int id)
{
    assert(id>-1 && id<m_colors.size());
    return m_colors[id];
}

int CHistogramPlot::getNumberOfPlots()
{
    return m_histoList.size();
}

void CHistogramPlot::setGrid(bool bEnable)
{
    if(m_bGrid == bEnable)
        return;

    if(m_pGrid == nullptr)
        m_pGrid = new QwtPlotGrid();

    m_pGrid->enableX( false );
    m_pGrid->enableY( true );
    m_pGrid->enableXMin( false );
    m_pGrid->enableYMin( false );
    m_pGrid->setMajorPen( Qt::black, 0, Qt::DotLine );

    if(bEnable == true)
        m_pGrid->attach( this );
    else
        m_pGrid->detach();

    m_bGrid = bEnable;
}

QStringList CHistogramPlot::getPlotTitles()
{
    return m_titles;
}

void CHistogramPlot::populate()
{
    CHistogram *pHisto = new CHistogram( m_titles.back(), m_colors.back());
    pHisto->setValues(m_histograms.back());
    pHisto->attach( this );

    m_histoList.append(pHisto);

    syncLegendView();
}

void CHistogramPlot::syncLegendView()
{
    if(m_pLegend == nullptr)
        return;

    replot(); // creating the legend items

    // Override this function and choose the right QwtPlotItem instead of Rtti_PlotItem
    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotHistogram );
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
