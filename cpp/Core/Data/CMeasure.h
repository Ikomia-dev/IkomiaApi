/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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

    public:

        static std::array<Id,9> m_internalMeasures;
        int                     m_id = Id::SURFACE;
        std::string             m_name = "";
        std::string             m_formula = "";
};

#endif // CMEASUREINFO_HPP
