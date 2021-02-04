#ifndef CMEASUREINFO_HPP
#define CMEASUREINFO_HPP

#include <vector>
#include <string>
#include <array>
#include "Main/CoreGlobal.hpp"

class CORESHARED_EXPORT CMeasure
{
    public:

        //Measures list
        enum Id
        {
            SURFACE,
            PERIMETER,
            CENTROID,
            BBOX,
            ORIENTED_BBOX,
            EQUIVALENT_DIAMETER,
            ELONGATION,
            CIRCULARITY,
            SOLIDITY,
            CUSTOM,
        };

        CMeasure();
        CMeasure(int id);
        CMeasure(int id, const std::string& name);
        CMeasure(int id, const std::string& name, const std::string& formula);

        static std::vector<CMeasure>    getAvailableMeasures();
        static std::string              getName(int id);
        std::string                     getName() const;

    public:

        static std::array<Id,9> m_internalMeasures;
        int                     m_id = Id::SURFACE;
        std::string             m_name = "";
        std::string             m_formula = "";
};

#endif // CMEASUREINFO_HPP
