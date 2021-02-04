#ifndef CDATAPLOTTESTS_H
#define CDATAPLOTTESTS_H

#include <QString>
#include <QtTest>

class QwtPlot;

class CDataPlotTests : public QObject
{
        Q_OBJECT

    public:
        CDataPlotTests();

    private slots:
        void simplePlot();
        void multiplePlot();
        void rangePlot();
        void removePlot();
        void manageLegend();
        void cosinePlot();

        void simplePiePlot();

        void simpleBarPlot();
        void multiBarPlot();

        void simpleHistogram();

    private:
        void displayPlot(QwtPlot* pPlot);
};

#endif // CDATAPLOTTESTS_H
