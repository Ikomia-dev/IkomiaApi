#include <cassert>
#include "CPlot.h"

#define HISTORY 1 // number of items

class QwtPlotCurve;
class CPieMarker;

/** @cond INTERNAL */

/**
 * @brief The PiePlot class makes possible the pie chart creation
 * Inside this class are defined the exact number of curves that should be shown in the
 * pie chart.
 */
class DATAPLOTSHARED_EXPORT CPiePlot : public CPlot
{
    Q_OBJECT
    public:

        CPiePlot(QWidget* pParent = nullptr);
        ~CPiePlot();

        const QwtPlotCurve *pieCurve(size_t id) const
        {
            assert(id<m_data.size());
            return m_data[id].curve;
        }

        void addPie(const QString& name, double value, QColor color = QColor());

        size_t getNumberOfPie();

        virtual void            syncLegendView() override;

    private:

        struct pieCurves
        {
            QString name = "";
            QwtPlotCurve *curve = nullptr;
            double data = 0.0;
            double realValue = 0.0;
        };

        std::vector<pieCurves> m_data;
        double timeData = 0.0;
        CPieMarker* m_pie = nullptr;
};

/** @endcond */
