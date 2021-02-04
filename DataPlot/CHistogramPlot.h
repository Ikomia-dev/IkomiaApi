#ifndef CHISTOGRAMPLOT_H
#define CHISTOGRAMPLOT_H

#include "CPlot.h"

class QwtLegend;
class CHistogram;

class DATAPLOTSHARED_EXPORT CHistogramPlot : public CPlot
{
    public:
        CHistogramPlot(QWidget* pParent = nullptr);

        void            addHistogram(QVector<double> data, const QString& title = QString(), const QColor& color = QColor());

        virtual void    setPlotName(const QString &name, int id  = 0) override;
        virtual void    setColor(const QColor& color, int id = 0) override;
        virtual QColor  getColor(int id = 0) override;
        virtual int     getNumberOfPlots() override;
        virtual void    setGrid(bool bEnable) override;
        virtual QStringList getPlotTitles() override;

        virtual void    syncLegendView() override;
    private:
        void populate();

    private:
        QVector<QVector<double>>    m_histograms;
        QList<QString>              m_titles;
        QList<QColor>               m_colors;
        QList<CHistogram*>          m_histoList;
};

#endif // CHISTOGRAMPLOT_H
