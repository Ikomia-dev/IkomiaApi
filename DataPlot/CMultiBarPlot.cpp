#include "CMultiBarPlot.h"
#include <qwt_legend.h>
#include <qwt_plot_multi_barchart.h>
#include <qwt_plot_barchart.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <qwt_column_symbol.h>
#include <qwt_legend_label.h>

CMultiBarPlot::CMultiBarPlot(QWidget* pParent) : CPlot(pParent)
{
    setAutoFillBackground( true );

    /*setPalette( Qt::white );
    canvas()->setPalette( QColor( "LemonChiffon" ) );*/

    initBar();

    setMode(0);
    setOrientation( Qt::Vertical );
}

void CMultiBarPlot::setMode(int mode)
{
    if ( mode == 0 )
    {
        m_pMultiBar->setStyle( QwtPlotMultiBarChart::Grouped );
    }
    else
    {
        m_pMultiBar->setStyle( QwtPlotMultiBarChart::Stacked );
    }
}

void CMultiBarPlot::setOrientation(Qt::Orientation orient)
{
    if ( orient == Qt::Vertical )
    {
        m_axisX = QwtPlot::xBottom;
        m_axisY = QwtPlot::yLeft;

        m_pMultiBar->setOrientation( Qt::Vertical );
    }
    else
    {
        m_axisX = QwtPlot::yLeft;
        m_axisY = QwtPlot::xBottom;

        m_pMultiBar->setOrientation( Qt::Horizontal );
    }

    updateAxis();
}

int CMultiBarPlot::getNumberOfSeries()
{
    return m_series.size();
}

void CMultiBarPlot::addSerie(QVector<double> serie, const QStringList& titles)
{
    assert(titles.size()<=serie.size());

    m_series += serie;
    m_maxBar = std::max(m_maxBar, serie.size());

    if(titles.isEmpty() && m_serieTitles.isEmpty())
    {
        for(int i=0; i<m_maxBar; ++i)
            m_serieTitles.push_back("bar_"+QString::number(i));
    }
    else if(!titles.isEmpty() && m_serieTitles.isEmpty())
    {
        for(auto&& it : titles)
            m_serieTitles.push_back(it);

        for(int i=titles.size(); i<m_maxBar; ++i)
            m_serieTitles.push_back("bar_"+QString::number(i));
    }
    else if(titles.isEmpty() && !m_serieTitles.isEmpty())
    {
        for(int i=m_serieTitles.size(); i<m_maxBar; ++i)
            m_serieTitles.push_back("bar_"+QString::number(i));
    }
    else
    {
        m_serieTitles.reserve(m_maxBar);
        for(int i=0; i<titles.size(); ++i)
            m_serieTitles[i] = titles[i];

        for(int i=titles.size(); i<m_maxBar; ++i)
            m_serieTitles[i] = "bar_"+QString::number(i);
    }

    populate();
}

void CMultiBarPlot::removeSerie(int i)
{
    assert(i<m_series.size());

    m_series.removeAt(i);

    populate();
}

void CMultiBarPlot::setPlotName(const QString& name, int id)
{
    assert(id<m_serieTitles.size());
    m_serieTitles[id] = name;
    m_pMultiBar->setBarTitles( m_serieTitles );

    updateLegend();
}

void CMultiBarPlot::setColor(const QColor& color, int id)
{
    assert(id<m_maxBar);
    QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
    symbol->setLineWidth( 2 );
    symbol->setFrameStyle( QwtColumnSymbol::Raised );
    symbol->setPalette( QPalette( color ) );

    m_pMultiBar->setSymbol( id, symbol );
}

QColor CMultiBarPlot::getColor(int id)
{
    assert(id<m_maxBar);
    return m_pMultiBar->getSymbolColor(id);
}

QStringList CMultiBarPlot::getPlotTitles()
{
    QStringList list;
    for(auto&& it : m_serieTitles)
        list.push_back(it.text());
    return list;
}

void CMultiBarPlot::populate()
{
    if(!m_serieTitles.isEmpty())
        m_pMultiBar->setBarTitles( m_serieTitles );

    for ( int i = 0; i < m_maxBar; i++ )
    {
        auto color = m_colorList[qrand()%m_colorList.size()];
        QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol->setLineWidth( 2 );
        symbol->setFrameStyle( QwtColumnSymbol::Raised );
        symbol->setPalette( QPalette( QString::fromStdString(color) ) );

        m_pMultiBar->setSymbol( i, symbol );
    }
    m_pMultiBar->setSamples( m_series );

    updateAxis();
}

void CMultiBarPlot::updateAxis()
{
    setAxisScale( m_axisX, 0, m_series.size()-1, 1.0 );
    //setAxisAutoScale( m_axisX );
    setAxisAutoScale( m_axisY );

    QwtScaleDraw *scaleDraw1 = axisScaleDraw( m_axisX );
    scaleDraw1->enableComponent( QwtScaleDraw::Backbone, false );
    scaleDraw1->enableComponent( QwtScaleDraw::Ticks, false );

    QwtScaleDraw *scaleDraw2 = axisScaleDraw( m_axisY );
    scaleDraw2->enableComponent( QwtScaleDraw::Backbone, true );
    scaleDraw2->enableComponent( QwtScaleDraw::Ticks, true );

    plotLayout()->setAlignCanvasToScale( m_axisX, true );
    plotLayout()->setAlignCanvasToScale( m_axisY, false );

    plotLayout()->setCanvasMargin( 0 );
    updateCanvasMargins();

    replot();
}

void CMultiBarPlot::initBar()
{
    if(m_pMultiBar)
        return;

    m_pMultiBar = new CPlotMultiBarChart;
    m_pMultiBar->setLayoutPolicy( QwtPlotMultiBarChart::AutoAdjustSamples );
    m_pMultiBar->setSpacing( 20 );
    m_pMultiBar->setMargin( 3 );
    m_pMultiBar->setLegendIconSize( QSize( 10, 14 ) );
    m_pMultiBar->attach( this );
}
