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

#ifndef CDATASETINFO_H
#define CDATASETINFO_H

#include "Main/CoreDefine.hpp"
#include "Main/CoreGlobal.hpp"
#include <string>

/** @cond INTERNAL */

class CORESHARED_EXPORT CDatasetInfo
{
    public:

        //Constructors
        CDatasetInfo();
        /**
         * @brief
         *
         * @param info
         */
        CDatasetInfo(const CDatasetInfo& info);
        /**
         * @brief
         *
         * @param info
         */
        CDatasetInfo(CDatasetInfo&& info);

        //Operators
        /**
         * @brief
         *
         * @param info
         * @return CDatasetInfo &&operator
         */
        CDatasetInfo&   operator=(const CDatasetInfo& info);
        /**
         * @brief
         *
         * @param info
         * @return CDatasetInfo &&operator
         */
        CDatasetInfo&   operator=(CDatasetInfo&& info);

        bool            operator==(const CDatasetInfo& other) const;

        //Getters
        /**
         * @brief
         *
         * @return std::string
         */
        std::string     getName() const;

        //Setters
        /**
         * @brief
         *
         * @param name
         */
        void            setName(const std::string &getName);

    private:

        std::string m_name;
};

/** @endcond */

#endif // CDATASETINFO_H
