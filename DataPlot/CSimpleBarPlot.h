#ifndef CSIMPLEBARPLOT_H
#define CSIMPLEBARPLOT_H

#include "CPlot.h"

class CPlotBarChart;

class DATAPLOTSHARED_EXPORT CSimpleBarPlot : public CPlot
{
    public:
        CSimpleBarPlot(QWidget* pParent = nullptr);

        void setOrientation(Qt::Orientation orient);

        Qt::Orientation getOrientation();

        void addData(const QVector<double>& data, const QStringList& titles = QStringList(), const QList<QColor>& colors = {});

        virtual void    setPlotName(const QString &name, int id  = 0) override;
        virtual void    setColor(const QColor& color, int id = 0) override;
        virtual QColor  getColor(int id = 0) override;
        virtual QStringList getPlotTitles() override;

    private:
        void updateAxis();

    private:
        CPlotBarChart*              m_pSimpleBar = nullptr;
        QwtPlot::Axis               m_axisX;
        QwtPlot::Axis               m_axisY;
        QStringList                 m_titles;
};

#endif // CSIMPLEBARPLOT_H
