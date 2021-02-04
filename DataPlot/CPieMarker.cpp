#include <qpainter.h>
#include <qwt_scale_map.h>
#include <qwt_plot_curve.h>
#include "CPiePlot.h"
#include "CPieMarker.h"


/*!
 * \brief PieMarker::PieMarker constructor of PieMarker class
 */
CPieMarker::CPieMarker()
{
    setZ(1000);
    setRenderHint(QwtPlotItem::RenderAntialiased, true);
}

CPieMarker::CPieMarker(int numPlots, int size)
{
    setZ(1000);
    setRenderHint(QwtPlotItem::RenderAntialiased, true);

    /* Defining some parameters*/
    this->height = size;
    this->width = size;
    this->margin = 10;
    this->numPlots = numPlots;
}


/*!
 * \brief PieMarker::rtti
 * \return
 */
int CPieMarker::rtti() const
{
    return QwtPlotItem::Rtti_PlotUserItem;
}


/*!
 * \brief PieMarker::draw
 * \param p
 * \param rect
 */
void CPieMarker::draw(QPainter *p,
                     const QwtScaleMap &, const QwtScaleMap &,
                     const QRectF &rect) const
{
    const CPiePlot *piePlot = (CPiePlot *)plot();

    int minSize = std::min(rect.height(), rect.width()) - margin*2;
    minSize = std::min(minSize, width);

    int x = (rect.width() - minSize)/2;
    int y = (rect.height() - minSize)/2;
    QRect pieRect;
    pieRect.setX(x);
    pieRect.setY(y);
    pieRect.setHeight( minSize );
    pieRect.setWidth( minSize );

    //////////////////////////////////////////
    /*
      With the following line is possible to set how many and which component should be whown in the pie chart
     */
    //int dataType[numPlots]; -> not allowed in msvc
    int* dataType = new int[numPlots];
    for (int i = 0; i < numPlots; i++ )
        dataType[i] = i;

    //////////////////////////////////////////

    int angle = (int)(5760 * 0.75);
    for ( unsigned int i = 0;
          i < sizeof(dataType) / sizeof(dataType[0]); i++ )
    {
        const QwtPlotCurve *curve = piePlot->pieCurve(dataType[i]);
        if ( curve->dataSize() > 0 )
        {
            const int value = (int)(5760 * curve->sample(0).y() / 100.0);

            p->save();
            p->setPen(QPen(p->background(), 2));
            p->setBrush(QBrush(curve->pen().color(), Qt::SolidPattern));
            if ( value != 0 )
                p->drawPie(pieRect, -angle, -value);
            p->restore();

            angle += value;
        }
    }
    delete[] dataType;
}

void CPieMarker::setNumberOfPlots(int totalPlots)
{
    numPlots = totalPlots;
}

