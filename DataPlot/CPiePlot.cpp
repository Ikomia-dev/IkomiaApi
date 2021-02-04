#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_legenditem.h>
#include <qwt_plot_canvas.h>
#include "CPieMarker.h"
#include "CPiePlot.h"

/*!
 * \brief The PiePlotCurve class that creates a curve.
 */
class CPiePlotCurve: public QwtPlotCurve
{
    public:
        CPiePlotCurve(const QString &title):
            QwtPlotCurve(title)
        {
            setRenderHint(QwtPlotItem::RenderAntialiased);
        }

        void setColor(const QColor &color)
        {
            QColor c = color;
            c.setAlpha(150);

            setPen(c);
            setBrush(c);
        }
};

/*!
 * \brief PiePlot::PiePlot Constructor of PiePlot Class, it initializes all the curves ready to be plotted
 * \param parent is the widget that will contain the plot (Pie Chart)
 */
CPiePlot::CPiePlot(QWidget* pParent) : CPlot(pParent)
{
    /*QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setBorderRadius( 10 );
    setCanvas( canvas );*/

    plotLayout()->setAlignCanvasToScales(true);

    /*
     In situations, when there is a label at the most right position of the
     scale, additional space is needed to display the overlapping part
     of the label would be taken by reducing the width of scale and canvas.
     To avoid this "jumping canvas" effect, we add a permanent margin.
     We don't need to do the same for the left border, because there
     is enough space for the overlapping label below the left scale.
     */

    enableAxis(QwtPlot::yLeft,false);
    enableAxis(QwtPlot::xBottom,false);

    m_pie = new CPieMarker;
    m_pie->attach(this);
}

CPiePlot::~CPiePlot()
{
    if(m_data.size() > 0)
    {
        for (size_t i = 0; i < m_data.size(); i++)
        {
            delete m_data[i].curve;
        }
        m_data.clear();
    }
}

void CPiePlot::addPie(const QString& name, double value, QColor color)
{
    auto rand = std::rand();
    auto col = QColor(QString::fromStdString(m_colorList.at(rand % m_colorList.size())));

    if(!color.isValid())
        color = col;

    m_data.push_back(pieCurves());

    auto pCurve = new CPiePlotCurve(name);
    pCurve->setColor(color);
    pCurve->attach(this);

    m_data.back().name = name;
    m_data.back().curve = pCurve;
    m_data.back().realValue = value;

    double sum = 0.;
    for(auto&& it : m_data)
        sum += it.realValue;

    for(auto&& it : m_data)
    {
        it.data = it.realValue/sum*100;
        it.curve->setTitle(it.name+" ("+QString::number(it.data)+"%)");
    }

    for ( int j = 0; j < HISTORY; j++ )
        timeData++;

    for ( size_t c = 0; c < m_data.size(); ++c )
    {
        m_data.at(c).curve->setRawSamples(&timeData, &m_data.at(c).data, 1);
    }

    m_pie->setNumberOfPlots(static_cast<int>(m_data.size()));
}

size_t CPiePlot::getNumberOfPie()
{
    return m_data.size();
}

void CPiePlot::syncLegendView()
{
    if(m_pLegend == nullptr)
        return;

    replot(); // creating the legend items

    // Override this function and choose the right QwtPlotItem instead of Rtti_PlotItem
    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotUserItem );
    for ( int i = 0; i < items.size(); i++ )
    {
        if ( i == 0 )
        {
            const QVariant itemInfo = itemToInfo( items[i] );

            QwtLegendLabel *legendLabel =
                qobject_cast<QwtLegendLabel *>( m_pLegend->legendWidget( itemInfo ) );
            if ( legendLabel )
                legendLabel->setChecked( true );

            items[i]->setVisible( true );
        }
        else
        {
            items[i]->setVisible( false );
        }
    }
}
