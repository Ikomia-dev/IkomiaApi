#ifndef CPLOT_H
#define CPLOT_H

#include "DataPlotGlobal.h"
#include <qwt_plot.h>
#include <qwt_plot_legenditem.h>
#include <qwt_plot_legenditem.h>

class QwtPlotGrid;
class QwtLegend;
class QwtPlotCurve;
class QwtPlotZoomer;
class QwtPlotPanner;

class CLegendItemParam
{
    public:
        int m_maxCols = 1;
        Qt::Alignment m_align = Qt::AlignRight | Qt::AlignBottom;
        QwtPlotLegendItem::BackgroundMode m_bgMode = QwtPlotLegendItem::LegendBackground;
        int m_size = 10;
};

class CLegendItem : public QwtPlotLegendItem
{
    public:
        CLegendItem()
        {
            setRenderHint( QwtPlotItem::RenderAntialiased );

            QColor color( Qt::white );

            setTextPen( color );
            setBorderPen( color );

            QColor c( Qt::gray );
            c.setAlpha( 200 );

            setBackgroundBrush( c );
        }
};

class DATAPLOTSHARED_EXPORT CPlot : public QwtPlot
{
        Q_OBJECT
    public:
        CPlot(QWidget* pParent = nullptr);

        void setLegend(bool bEnable);
        void setLegendPos(QwtPlot::LegendPosition position);
        void setLegendItemMode(QwtLegendData::Mode mode);
        void setInternalLegend(bool bEnable);
        void setInternalLegendParam(int maxCols = 1, Qt::Alignment align = Qt::AlignRight | Qt::AlignBottom, QwtPlotLegendItem::BackgroundMode bgMode = QwtPlotLegendItem::LegendBackground, int size = 10);
        void setZoomable(bool bEnable);
        void setMovable(bool bEnable);
        void setBackgroundColor(const QColor& color);

        QwtPlot::LegendPosition getLegendPos();
        CLegendItemParam*       getInternalLegendParam();

        bool hasGrid();
        bool hasLegend();
        bool hasInternalLegend();
        bool isZoomable();
        bool isMovable();

        void updateInternalLegend();

        void exportChart(const QString& path);

        virtual void            setGrid(bool bEnable);
        virtual void            setColor(const QColor& color, int id = 0);
        virtual QColor          getColor(int id = 0);
        virtual int             getNumberOfPlots();
        virtual QStringList     getPlotTitles();
        virtual void            setPlotName(const QString& name, int id = 0);
        virtual void            syncLegendView();

    public slots:
        void onSetTitle(const QString& title);
        void onSetLabelX(const QString& text);
        void onSetLabelY(const QString& text);

    protected slots:
        void onShowItem( const QVariant &itemInfo, bool on );

    protected:
        QwtPlotGrid*                m_pGrid = nullptr;
        QwtLegend*                  m_pLegend = nullptr;
        CLegendItem*                m_pLegendItem = nullptr;
        CLegendItemParam*           m_pLegendItemParam = nullptr;
        QwtPlotZoomer*              m_pZoomer = nullptr;
        QwtPlotPanner*              m_pPanner = nullptr;
        std::vector<std::string>    m_colorList;
        QwtPlot::LegendPosition     m_legendPos = LegendPosition::RightLegend;
        bool                        m_bGrid = false;
        bool                        m_bLegend = false;
        bool                        m_bLegendIn = false;
        bool                        m_bZoom = false;
        bool                        m_bPan = false;
};

#endif // CPLOT_H
