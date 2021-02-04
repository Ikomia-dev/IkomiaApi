#ifndef CCURVEPLOT_H
#define CCURVEPLOT_H

#include "CPlot.h"

class QwtPlotIntervalCurve;

class DATAPLOTSHARED_EXPORT CCurvePlot : public CPlot
{
    Q_OBJECT
    public:
        enum Mode
        {
            Bars,
            Tube
        };

        CCurvePlot(QWidget* pParent = nullptr);

        QwtPlotCurve*           addPlot(const QString& name, std::vector<double> x, std::vector<double> y, int thickness = 4,  QColor color = QColor(), const QString& symbol = "");
        QwtPlotIntervalCurve*   addErrorBars(const QString& name, std::vector<double> x, std::vector<double> ymin, std::vector<double> ymax, const QColor& colorPen = Qt::blue, QColor colorBrush = Qt::blue);

        void                    removePlot(QwtPlotCurve* pCurve);
        void                    removeErrorBar(QwtPlotIntervalCurve* pIntervalCurve);

        void                    setCurveArea(bool bEnable);
        void                    setErrorBarMode(int mode);
        void                    setThickness(int thickness, int id = 0);
        void                    setLineType(const QString& type, int id = 0);
        void                    setMarker(const QString& marker, QColor brushColor = QColor(), QColor penColor = QColor(), int id = 0);

        int                     getThickness(int id = 0);
        QString                 getLineType(int id = 0);
        QString                 getMarker(int id = 0);
        QColor                  getMarkerBrushColor(int id = 0);
        QColor                  getMarkerPenColor(int id = 0);

        virtual void            setPlotName(const QString& name, int id = 0) override;
        virtual void            setColor(const QColor &color, int id = 0) override;
        virtual QColor          getColor(int id = 0) override;
        virtual QStringList     getPlotTitles() override;
        virtual int             getNumberOfPlots() override;
        virtual void            syncLegendView() override;

    protected:
        QList<QwtPlotCurve*>            m_curveList;
        QList<QwtPlotIntervalCurve*>    m_pIntervalCurveList;
        QStringList                     m_titles;
        bool                            m_bArea = false;
};

#endif // CCURVEPLOT_H
