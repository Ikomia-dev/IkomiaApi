#ifndef DATAPLOT_H
#define DATAPLOT_H

#include "DataPlotGlobal.h"
#include "CCurvePlot.h"
#include "CPiePlot.h"
#include "CMultiBarPlot.h"
#include "CSimpleBarPlot.h"
#include "CHistogramPlot.h"
#include <unordered_map>

class DATAPLOTSHARED_EXPORT CDataPlot
{

    public:
        enum Type
        {
            Curve,
            Bar,
            MultiBar,
            Histogram,
            Pie
        };
        CDataPlot();

        CCurvePlot*     createCurvePlot();
        CPiePlot*       createPiePlot();
        CSimpleBarPlot* createSimpleBarPlot();
        CMultiBarPlot*  createMultiBarPlot();
        CHistogramPlot* createHistogramPlot();
        CPlot*          create();

        void            setType(Type type);
        void            setValueList(const std::vector<std::vector<double>>& valueList);
        void            setStringList(const std::vector<std::vector<std::string>>& stringList);
        void            addValueList(const std::vector<double>& values);
        void            addStringList(const std::vector<std::string>& values);
        Type            getType();

    private:
        Type                                    m_type;
        std::vector<std::vector<double>>        m_dataList;
        std::vector<std::vector<std::string>>   m_stringList;
};

#endif // DATAPLOT_H
