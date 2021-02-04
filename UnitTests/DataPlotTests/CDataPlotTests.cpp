#include "CDataPlotTests.h"
#include <CDataPlot.h>
#include <QDialog>
#include <QGridLayout>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_legenditem.h>
#include <qwt_plot_intervalcurve.h>

CDataPlotTests::CDataPlotTests()
{
}

void CDataPlotTests::simplePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();
    QVERIFY(pPlot != nullptr);

    pPlot->setTitle("Plot DEMO");
    QVERIFY(pPlot->title().text() == "Plot DEMO");

    QVERIFY(pPlot->hasGrid() == false);
    pPlot->setGrid(true);
    QVERIFY(pPlot->hasGrid() == true);

    QVERIFY(pPlot->hasLegend() == false);
    pPlot->setLegend(true);
    QVERIFY(pPlot->hasLegend() == true);

    QVERIFY(pPlot->hasInternalLegend() == false);
    pPlot->setInternalLegend(true);
    QVERIFY(pPlot->hasInternalLegend() == true);

    pPlot->setAxisTitle(QwtPlot::Axis::xBottom, "x");
    QVERIFY(pPlot->axisTitle(QwtPlot::Axis::xBottom).text() == "x");

    pPlot->setAxisTitle(QwtPlot::Axis::yLeft, "y");
    QVERIFY(pPlot->axisTitle(QwtPlot::Axis::yLeft).text() == "y");

    pPlot->addPlot("First", {0,1,2,3,4}, {0,10,5,20,15});

    QVERIFY(pPlot->getNumberOfPlots() == 1);

    QVERIFY(pPlot->isZoomable() == false);
    pPlot->setZoomable(true);
    QVERIFY(pPlot->isZoomable() == true);

    QVERIFY(pPlot->isMovable() == false);
    pPlot->setMovable(true);
    QVERIFY(pPlot->isMovable() == true);

    //displayPlot(pPlot);

}

void CDataPlotTests::multiplePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();

    pPlot->setTitle("Multiple Plot DEMO");
    pPlot->setGrid(true);
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setAxisTitle(QwtPlot::Axis::xBottom, "x");
    pPlot->setAxisTitle(QwtPlot::Axis::yLeft, "y");

    pPlot->addPlot("First", {0,1,2,3,4}, {0,1,2,3,4});
    pPlot->addPlot("Second", {0,1,2,3,4}, {0,2,5,4,10}, 1, QColor("LightSalmon"), "d-.");
    pPlot->addPlot("Third", {0,1,2,3,4}, {0,3,4,12,1}, 2, Qt::magenta, "o..-");
    pPlot->addPlot("Fourth", {0,1,2,3,4}, {0,5,5,5,5}, 3, Qt::green, "d.");
    auto pCurve = pPlot->addPlot("Fifth", {0,1,2,3,4}, {0,6,7,15,14}, 4, Qt::cyan, "o-");
    pPlot->addPlot("Sixth", {0,1,2,3,4}, {0,10,15,10,5}, 0, Qt::yellow, "<");

    QVERIFY(pPlot->getNumberOfPlots() == 6);

    QVERIFY(pCurve->title().text() == "Fifth");
    QVERIFY(pCurve->minXValue() == 0);
    QVERIFY(pCurve->maxXValue() == 4);
    QVERIFY(pCurve->minYValue() == 0);
    QVERIFY(pCurve->maxYValue() == 15);
    QVERIFY(pCurve->pen().color() == Qt::cyan);
    QVERIFY(pCurve->symbol()->style() == QwtSymbol::Style::Ellipse);

    //displayPlot(pPlot);
}

void CDataPlotTests::rangePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();
    QVERIFY(pPlot != nullptr);

    pPlot->setTitle("Range plot DEMO");
    pPlot->setLegend(true);;
    pPlot->setInternalLegend(true);
    pPlot->setAxisTitle(QwtPlot::Axis::xBottom, "x");
    pPlot->setAxisTitle(QwtPlot::Axis::yLeft, "y");
    pPlot->addPlot("First", {0,1,2,3,4}, {0,10,5,20,15}, 2, Qt::cyan, "*");
    auto pRange = pPlot->addErrorBars("Range", {0,1,2,3,4}, {0,5,2,10,12}, {0,20,15,40,25}, Qt::red, Qt::magenta);
    pPlot->setZoomable(true);
    pPlot->setMovable(true);

    QVERIFY(pRange->title().text() == "Range");
    QVERIFY(pRange->dataSize() == 5);

    //pPlot->setErrorBarMode(CCurvePlot::Mode::Bars);

    //displayPlot(pPlot);
}

void CDataPlotTests::removePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();

    auto pCurve1 = pPlot->addPlot("First", {0,1,2,3,4}, {0,1,2,3,4});
    auto pCurve2 = pPlot->addPlot("Second", {0,1,2,3,4}, {0,2,5,4,10}, 4, QColor("LightSalmon"), "d-.");
    QVERIFY(pPlot->getNumberOfPlots() == 2);

    pPlot->removePlot(pCurve1);
    QVERIFY(pPlot->getNumberOfPlots() == 1);

    pPlot->removePlot(pCurve2);
    QVERIFY(pPlot->getNumberOfPlots() == 0);

    //displayPlot(pPlot);
}

void CDataPlotTests::manageLegend()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();

    pPlot->setTitle("Plot DEMO");
    pPlot->setGrid(true);
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setAxisTitle(QwtPlot::Axis::xBottom, "x");
    pPlot->setAxisTitle(QwtPlot::Axis::yLeft, "y");

    pPlot->addPlot("First", {0,1,2,3,4}, {0,1,2,3,4});
    pPlot->addPlot("Second", {0,1,2,3,4}, {0,2,5,4,10}, 4, QColor("LightSalmon"), "h-.");

    pPlot->setLegendPos(QwtPlot::LegendPosition::TopLegend);
    QVERIFY(pPlot->getLegendPos() == QwtPlot::LegendPosition::TopLegend);

    auto pParam = pPlot->getInternalLegendParam();
    QVERIFY(pParam);
    QVERIFY(pParam->m_maxCols == 1);
    QVERIFY(pParam->m_align == (Qt::AlignRight | Qt::AlignBottom));
    QVERIFY(pParam->m_bgMode == QwtPlotLegendItem::LegendBackground);
    QVERIFY(pParam->m_size == 10);

    pPlot->setInternalLegendParam(2, Qt::AlignLeft, QwtPlotLegendItem::ItemBackground, 12);
    QVERIFY(pParam->m_maxCols == 2);
    QVERIFY(pParam->m_align == Qt::AlignLeft);
    QVERIFY(pParam->m_bgMode == QwtPlotLegendItem::ItemBackground);
    QVERIFY(pParam->m_size == 12);

    //displayPlot(pPlot);
}

void CDataPlotTests::cosinePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createCurvePlot();

    pPlot->setTitle("Cosine Plot DEMO");
    pPlot->setGrid(true);
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setAxisTitle(QwtPlot::Axis::xBottom, "x");
    pPlot->setAxisTitle(QwtPlot::Axis::yLeft, "y");

    std::vector<double> x, y;
    for(double i=-2*std::acos(-1); i<2*std::acos(-1); i+=0.1)
    {
        x.push_back(i);
        y.push_back(std::cos(i));
    }

    auto max = x.back();
    x.push_back(2*std::acos(-1));

    QVERIFY(x.size() != y.size());

    auto pCurve = pPlot->addPlot("Cosine", x, y, 2, QColor("LightSalmon"), "s");

    QVERIFY(pCurve->maxXValue() == max);
    QVERIFY(pPlot->getNumberOfPlots() == 1);

    //displayPlot(pPlot);
}

void CDataPlotTests::simplePiePlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createPiePlot();

    QVERIFY(pPlot);

    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setLegendItemMode(QwtLegendData::ReadOnly);

    pPlot->addPie("Plot 1", 50, Qt::red);
    pPlot->addPie("Plot 2", 50);
    pPlot->addPie("Plot 3", 50);
    pPlot->addPie("Plot 4", 100);

    QVERIFY(pPlot->getNumberOfPie() == 4);

    //displayPlot(pPlot);
}

void CDataPlotTests::simpleBarPlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createSimpleBarPlot();

    QVERIFY(pPlot);

    pPlot->setTitle("Simple Bar Plot");
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setLegendItemMode(QwtLegendData::ReadOnly);
    pPlot->addData({100, 50, 125, 75}, {"Arch","Ubuntu", "Debian"}, {Qt::red, Qt::green});

    //displayPlot(pPlot);
}

void CDataPlotTests::multiBarPlot()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createMultiBarPlot();

    QVERIFY(pPlot);

    pPlot->setTitle("Bar Chart");
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);
    pPlot->setLegendItemMode(QwtLegendData::ReadOnly);

    QVector<double> serie1 = {10, 20, 5, 30};
    QVector<double> serie2 = {5, 10, 20, 30, 40};
    QVector<double> serie3 = {10, 20, 30};

    pPlot->addSerie(serie1);
    pPlot->addSerie(serie2);
    pPlot->addSerie(serie3);
    pPlot->addSerie({100});

    QVERIFY(pPlot->getNumberOfSeries() == 4);

    pPlot->removeSerie(1);

    QVERIFY(pPlot->getNumberOfSeries() == 3);

    //displayPlot(pPlot);
}

void CDataPlotTests::simpleHistogram()
{
    CDataPlot myPlot;
    auto pPlot = myPlot.createHistogramPlot();

    QVERIFY(pPlot);

    pPlot->setTitle("Simple histogram");
    pPlot->setLegend(true);
    pPlot->setInternalLegend(true);

    pPlot->addHistogram({10, 20, 50, 180, 50}, "First", Qt::darkMagenta);
    pPlot->addHistogram({15, 10, 5, 200, 100});
    pPlot->addHistogram({20, 15, 10, 50, 80, 150});

    //displayPlot(pPlot);
}

void CDataPlotTests::displayPlot(QwtPlot* pPlot)
{
    QDialog* pPopup = new QDialog;
    QGridLayout* pLayout = new QGridLayout;
    pLayout->addWidget(pPlot);
    pPopup->setLayout(pLayout);
    pPopup->exec();
}

QTEST_MAIN(CDataPlotTests)
