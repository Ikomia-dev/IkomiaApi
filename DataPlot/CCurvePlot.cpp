#include "CCurvePlot.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_interval_symbol.h>

CCurvePlot::CCurvePlot(QWidget* pParent) : CPlot(pParent)
{
}

int CCurvePlot::getNumberOfPlots()
{
    return m_curveList.size();
}

QwtPlotCurve* CCurvePlot::addPlot(const QString& name, std::vector<double> x, std::vector<double> y, int thickness, QColor color, const QString& symbol)
{
    QwtPlotCurve *curve = new QwtPlotCurve;
    curve->setTitle( name );
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    if(thickness == 0)
        curve->setStyle(QwtPlotCurve::NoCurve);
    else
    {
        QPen pen;

        if(!color.isValid())
        {
            auto rand = qrand();
            color = QColor(QString::fromStdString(m_colorList[rand%m_colorList.size()]));
        }

        if(symbol.contains("-."))
            pen = QPen( color, thickness, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
        else if(symbol.contains("..-"))
            pen = QPen( color, thickness, Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin);
        else if(symbol.contains("-"))
            pen = QPen( color, thickness, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
        else if(symbol.contains("."))
            pen = QPen( color, thickness, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        else
            pen = QPen( color, thickness);

        curve->setPen(pen);
    }

    QwtSymbol* pSymbol;
    auto randIn = std::rand();
    auto randOut = std::rand();
    auto colIn = QColor(QString::fromStdString(m_colorList.at(randIn % m_colorList.size())));
    auto colOut = QColor(QString::fromStdString(m_colorList.at(randOut % m_colorList.size())));
    if(symbol.contains("o"))
        pSymbol = new QwtSymbol( QwtSymbol::Ellipse, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 )  );
    else if(symbol.contains("d"))
        pSymbol = new QwtSymbol( QwtSymbol::Diamond, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("s"))
        pSymbol = new QwtSymbol( QwtSymbol::Rect, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("+"))
        pSymbol = new QwtSymbol( QwtSymbol::Cross, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("x"))
        pSymbol = new QwtSymbol( QwtSymbol::XCross, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("^"))
        pSymbol = new QwtSymbol( QwtSymbol::UTriangle, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("v"))
        pSymbol = new QwtSymbol( QwtSymbol::DTriangle, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains(">"))
        pSymbol = new QwtSymbol( QwtSymbol::RTriangle, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("<"))
        pSymbol = new QwtSymbol( QwtSymbol::LTriangle, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("*"))
        pSymbol = new QwtSymbol( QwtSymbol::Star2, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else if(symbol.contains("h"))
        pSymbol = new QwtSymbol( QwtSymbol::Hexagon, QBrush( colIn ), QPen( colOut, 2 ), QSize( 8, 8 ) );
    else
        pSymbol = new QwtSymbol( QwtSymbol::NoSymbol);

    curve->setSymbol( pSymbol );

    QPolygonF points;
    auto minSize = std::min(x.size(), y.size());
    for(size_t i=0; i<minSize; ++i)
    {
        points << QPointF( x[i], y[i] );
    }
    curve->setSamples( points );

    curve->attach( this );

    syncLegendView();

    m_curveList.append(curve);
    m_titles.append(name);

    return curve;
}

void CCurvePlot::removePlot(QwtPlotCurve* pCurve)
{
    assert(pCurve);

    pCurve->detach();
    m_curveList.removeOne(pCurve);

    delete pCurve;
    pCurve = nullptr;
}

QwtPlotIntervalCurve* CCurvePlot::addErrorBars(const QString& name, std::vector<double> x, std::vector<double> ymin, std::vector<double> ymax, const QColor& colorPen, QColor colorBrush)
{
    QwtPlotIntervalCurve* pIntervalCurve = new QwtPlotIntervalCurve(name);
    pIntervalCurve->setRenderHint( QwtPlotItem::RenderAntialiased );
    pIntervalCurve->setPen( colorPen );

    colorBrush.setAlpha( 150 );
    pIntervalCurve->setBrush( QBrush( colorBrush ) );
    pIntervalCurve->setStyle( QwtPlotIntervalCurve::Tube );

    QVector<QwtIntervalSample> rangeData;
    auto minSize = std::min(x.size(), ymin.size());
    for(size_t i=0; i<minSize; ++i)
    {
        rangeData.append(QwtIntervalSample(double(i), QwtInterval(ymin[i], ymax[i])));
    }

    pIntervalCurve->setSamples( rangeData );
    pIntervalCurve->attach( this );

    m_pIntervalCurveList.append(pIntervalCurve);

    syncLegendView();

    return pIntervalCurve;
}

void CCurvePlot::removeErrorBar(QwtPlotIntervalCurve* pIntervalCurve)
{
    assert(pIntervalCurve);

    pIntervalCurve->detach();
    m_pIntervalCurveList.removeOne(pIntervalCurve);

    delete pIntervalCurve;
    pIntervalCurve = nullptr;
}

void CCurvePlot::setCurveArea(bool bEnable)
{
    m_bArea = bEnable;

    for(auto&& it : m_curveList)
    {
        if(m_bArea)
        {
            auto p = it->pen();
            QColor col(p.color());
            col.setAlpha(150);
            it->setBrush(QBrush(col));
        }
        else
            it->setBrush(QBrush());
    }

    replot();
}

void CCurvePlot::setErrorBarMode(int mode)
{
    if ( mode == Tube )
    {
        for(auto&& it : m_pIntervalCurveList)
        {
            it->setStyle( QwtPlotIntervalCurve::Tube );
            it->setSymbol( NULL );
            it->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        }
    }
    else
    {
        for(auto&& it : m_pIntervalCurveList)
        {
            it->setStyle( QwtPlotIntervalCurve::NoCurve );

            QColor c( it->brush().color().rgb() ); // skip alpha

            QwtIntervalSymbol *errorBar =
                new QwtIntervalSymbol( QwtIntervalSymbol::Bar );
            errorBar->setWidth( 8 ); // should be something even
            errorBar->setPen( c );

            it->setSymbol( errorBar );
            it->setRenderHint( QwtPlotItem::RenderAntialiased, false );
        }
    }

    replot();
}

void CCurvePlot::setThickness(int thickness, int id)
{
    assert(id<m_curveList.size());
    auto pen = m_curveList[id]->pen();
    pen.setWidth(thickness);
    m_curveList[id]->setPen(pen);
}

void CCurvePlot::setLineType(const QString& type, int id)
{
    assert(id<m_curveList.size());
    auto pen = m_curveList[id]->pen();

    if(type.contains("-."))
        pen = QPen( pen.color(), pen.width(), Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    else if(type.contains("..-"))
        pen = QPen( pen.color(), pen.width(), Qt::DashDotDotLine, Qt::RoundCap, Qt::RoundJoin);
    else if(type.contains("-"))
        pen = QPen( pen.color(), pen.width(), Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    else if(type.contains("."))
        pen = QPen( pen.color(), pen.width(), Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    else
        pen = QPen( pen.color(), pen.width(), Qt::SolidLine);

    m_curveList[id]->setPen(pen);
}

void CCurvePlot::setMarker(const QString& marker, QColor brushColor, QColor penColor, int id)
{
    assert(id<m_curveList.size());

    if(!brushColor.isValid())
    {
        auto randIn = std::rand();
        brushColor = QColor(QString::fromStdString(m_colorList.at(randIn % m_colorList.size())));
    }
    if(!penColor.isValid())
    {
        auto randOut = std::rand();
        penColor = QColor(QString::fromStdString(m_colorList.at(randOut % m_colorList.size())));
    }

    QwtSymbol* pSymbol;
    if(marker.contains("o"))
        pSymbol = new QwtSymbol( QwtSymbol::Ellipse, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 )  );
    else if(marker.contains("d"))
        pSymbol = new QwtSymbol( QwtSymbol::Diamond, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("s"))
        pSymbol = new QwtSymbol( QwtSymbol::Rect, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("+"))
        pSymbol = new QwtSymbol( QwtSymbol::Cross, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("x"))
        pSymbol = new QwtSymbol( QwtSymbol::XCross, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("^"))
        pSymbol = new QwtSymbol( QwtSymbol::UTriangle, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("v"))
        pSymbol = new QwtSymbol( QwtSymbol::DTriangle, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains(">"))
        pSymbol = new QwtSymbol( QwtSymbol::RTriangle, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("<"))
        pSymbol = new QwtSymbol( QwtSymbol::LTriangle, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("*"))
        pSymbol = new QwtSymbol( QwtSymbol::Star2, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else if(marker.contains("h"))
        pSymbol = new QwtSymbol( QwtSymbol::Hexagon, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );
    else
        pSymbol = new QwtSymbol( QwtSymbol::NoSymbol, QBrush( brushColor ), QPen( penColor, 2 ), QSize( 8, 8 ) );

    m_curveList[id]->setSymbol( pSymbol );
}

int CCurvePlot::getThickness(int id)
{
    assert(id<m_curveList.size());
    auto pen = m_curveList[id]->pen();

    return pen.width();
}

QString CCurvePlot::getLineType(int id)
{
    assert(id<m_curveList.size());
    auto pen = m_curveList[id]->pen();

    if(pen.style() == Qt::DashDotLine)
        return "-.";
    else if(pen.style() == Qt::DashDotDotLine)
        return "..-";
    else if(pen.style() == Qt::DashLine)
        return "-";
    else if(pen.style() == Qt::DotLine)
        return ".";
    else
        return "Default";
}

QString CCurvePlot::getMarker(int id)
{
    assert(id<m_curveList.size());
    auto pSymbol = m_curveList[id]->symbol();

    if(pSymbol->style() == QwtSymbol::Ellipse)
        return "o";
    else if(pSymbol->style() == QwtSymbol::Diamond)
        return "d";
    else if(pSymbol->style() == QwtSymbol::Rect)
        return "s";
    else if(pSymbol->style() == QwtSymbol::Cross)
        return "+";
    else if(pSymbol->style() == QwtSymbol::XCross)
        return "x";
    else if(pSymbol->style() == QwtSymbol::UTriangle)
        return "^";
    else if(pSymbol->style() == QwtSymbol::DTriangle)
        return "v";
    else if(pSymbol->style() == QwtSymbol::RTriangle)
        return ">";
    else if(pSymbol->style() == QwtSymbol::LTriangle)
        return "<";
    else if(pSymbol->style() == QwtSymbol::Star2)
        return "*";
    else if(pSymbol->style() == QwtSymbol::Hexagon)
        return "h";
    else
        return "Default";
}

QColor CCurvePlot::getMarkerBrushColor(int id)
{
    assert(id<m_curveList.size());
    return m_curveList[id]->symbol()->brush().color();
}

QColor CCurvePlot::getMarkerPenColor(int id)
{
    assert(id<m_curveList.size());
     return m_curveList[id]->symbol()->pen().color();
}

void CCurvePlot::setPlotName(const QString& name, int id)
{
    assert(id<m_curveList.size() && id<m_titles.size());
    m_curveList[id]->setTitle(name);
    m_titles[id] = name;
}

void CCurvePlot::setColor(const QColor& color, int id)
{
    assert(id<m_curveList.size());
    auto pen = m_curveList[id]->pen();
    pen.setColor(color);
    m_curveList[id]->setPen(pen);
}

QColor CCurvePlot::getColor(int id)
{
    assert(id<m_curveList.size());
    return m_curveList[id]->pen().color();
}

QStringList CCurvePlot::getPlotTitles()
{
    return m_titles;
}

void CCurvePlot::syncLegendView()
{
    if(m_pLegend == nullptr)
        return;

    replot(); // creating the legend items

    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve );
    items += itemList( QwtPlotItem::Rtti_PlotIntervalCurve );
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
