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

#include "CSimpleBarPlot.h"
#include <qwt_plot_barchart.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <qwt_column_symbol.h>
#include <qwt_legend_label.h>
#include <cassert>

class CPlotScaleDraw: public QwtScaleDraw
{
    public:
        CPlotScaleDraw( Qt::Orientation orientation, const QStringList &labels ):
            m_labels( labels )
        {
            setTickLength( QwtScaleDiv::MinorTick, 0 );
            setTickLength( QwtScaleDiv::MediumTick, 0 );
            setTickLength( QwtScaleDiv::MajorTick, 2 );

            enableComponent( QwtScaleDraw::Backbone, false );

            if ( orientation == Qt::Vertical )
            {
                setLabelRotation( -60.0 );
            }
            else
            {
                setLabelRotation( -20.0 );
            }

            setLabelAlignment( Qt::AlignLeft | Qt::AlignVCenter );
        }

        virtual QwtText label( double value ) const
        {
            QwtText lbl;

            const int index = qRound( value );
            if ( index >= 0 && index < m_labels.size() )
            {
                lbl = m_labels[ index ];
            }

            return lbl;
        }

    private:
        const QStringList m_labels;
};

class CPlotBarChart: public QwtPlotBarChart
{
    public:
        CPlotBarChart():
            QwtPlotBarChart()
        {
            setLayoutPolicy( AutoAdjustSamples );
            setLayoutHint( 4.0 ); // minimum width for a single bar

            setSpacing( 10 ); // spacing between bars
        }

        void addData( const QVector<double>& data, const QStringList &titles = QStringList(), const QList<QColor> &color = {} )
        {
            if(color.size() < data.size())
            {
                for(int i=color.size(); i<data.size(); ++i)
                    m_colors.push_back(Qt::gray);
            }
            else
                m_colors = color;

            if(titles.size() < data.size())
            {
                for(int i=titles.size(); i<data.size(); ++i)
                    m_titles.push_back("");
            }
            else
                m_titles = titles;

            setSamples(data);
            itemChanged();          
        }

        void setTitle(const QString& name, int id)
        {
            assert(id<m_titles.size());
            m_titles[id] = name;
        }
        void setColor(const QColor& color, int id)
        {
            assert(id<m_colors.size());
            m_colors[id] = color;
        }
        QColor getColor(int id)
        {
            assert(id<m_colors.size());
            return m_colors[id];
        }
        virtual QwtColumnSymbol *specialSymbol(
            int index, const QPointF& ) const
        {
            // we want to have individual colors for each bar

            QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
            symbol->setLineWidth( 2 );
            symbol->setFrameStyle( QwtColumnSymbol::Raised );

            // Default color
            QColor c( Qt::gray );
            if ( index >= 0 && index < m_colors.size() )
                c = m_colors[ index ];

            symbol->setPalette( c );

            return symbol;
        }

        virtual QwtText barTitle( int sampleIndex ) const
        {
            QwtText title;
            if ( sampleIndex >= 0 && sampleIndex < m_titles.size() )
                title = m_titles[ sampleIndex ];

            return title;
        }

    private:
        QList<QColor> m_colors;
        QList<QString> m_titles;
};

CSimpleBarPlot::CSimpleBarPlot(QWidget* pParent) : CPlot(pParent)
{
    setAutoFillBackground( true );
   /* setPalette( QColor( "Linen" ) );

    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 2 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Sunken );
    canvas->setBorderRadius( 10 );

    QPalette canvasPalette( QColor( "Plum" ) );
    canvasPalette.setColor( QPalette::Foreground, QColor( "Indigo" ) );
    canvas->setPalette( canvasPalette );

    setCanvas( canvas );*/

    m_pSimpleBar = new CPlotBarChart;
    m_pSimpleBar->setLegendMode( QwtPlotBarChart::LegendBarTitles );
    m_pSimpleBar->setLegendIconSize( QSize( 10, 14 ) );
    m_pSimpleBar->attach( this );

    setOrientation( Qt::Vertical );
    setAutoReplot( true );
}

void CSimpleBarPlot::setOrientation(Qt::Orientation orient)
{
    if ( orient == Qt::Vertical )
    {
        m_axisX = QwtPlot::xBottom;
        m_axisY = QwtPlot::yLeft;

        m_pSimpleBar->setOrientation( Qt::Vertical );
    }
    else
    {
        m_axisX = QwtPlot::yLeft;
        m_axisY = QwtPlot::xBottom;

        m_pSimpleBar->setOrientation( Qt::Horizontal );
    }

    updateAxis();
}

Qt::Orientation CSimpleBarPlot::getOrientation()
{
    return m_pSimpleBar->orientation();
}

void CSimpleBarPlot::addData(const QVector<double>& data, const QStringList& titles, const QList<QColor>& colors)
{
    assert(m_pSimpleBar);

    m_pSimpleBar->addData(data, titles, colors);
    m_titles = titles;

    updateLegend();
    updateAxis();
}

void CSimpleBarPlot::setPlotName(const QString& name, int id)
{
    assert(id<m_titles.size());
    m_titles[id] = name;
    m_pSimpleBar->setTitle(name, id);

    updateLegend();
    updateAxis();
}

void CSimpleBarPlot::setColor(const QColor& color, int id)
{
    m_pSimpleBar->setColor(color, id);

    updateLegend();
    replot();
}

QColor CSimpleBarPlot::getColor(int id)
{
    return m_pSimpleBar->getColor(id);
}

QStringList CSimpleBarPlot::getPlotTitles()
{
    return m_titles;
}

void CSimpleBarPlot::updateAxis()
{
    setAxisScale( m_axisX, 0, m_pSimpleBar->dataSize(), 1.0 );
    setAxisAutoScale( m_axisY );

    setAxisScaleDraw( m_axisX, new CPlotScaleDraw( m_pSimpleBar->orientation(), m_titles ) );

    QwtScaleDraw *scaleDraw2 = axisScaleDraw( m_axisY );
    scaleDraw2->enableComponent( QwtScaleDraw::Backbone, true );
    scaleDraw2->enableComponent( QwtScaleDraw::Ticks, true );

    plotLayout()->setAlignCanvasToScale( m_axisX, true );
    plotLayout()->setAlignCanvasToScale( m_axisY, false );

    plotLayout()->setCanvasMargin( 0 );
    updateCanvasMargins();

    replot();
}
