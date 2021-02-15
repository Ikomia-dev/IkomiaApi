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

#ifndef CVIRTUALIMAGEIO_H
#define CVIRTUALIMAGEIO_H

#include <string>
#include "Data/CDataSubset.hpp"

/**
 * @brief
 *
 */
class CVirtualImageIO
{
    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CVirtualImageIO(const std::string& fileName);
        /**
         * @brief
         *
         */
        virtual ~CVirtualImageIO();

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual VectorString    fileNames(const SubsetBounds& bounds) = 0;

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual Dimensions      dimensions() = 0;

        /**
         * @brief
         *
         * @return Dimensions
         */
        virtual Dimensions      dimensions(const SubsetBounds& subset) = 0;
        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        virtual CDataInfoPtr    dataInfo() = 0;

        virtual CDataInfoPtr    dataInfo(const SubsetBounds& subset) = 0;
        /**
         * @brief
         *
         * @return CMat
         */
        virtual CMat            read() = 0;
        /**
         * @brief
         *
         * @param subset
         * @return CMat
         */
        virtual CMat            read(const SubsetBounds& subset) = 0;

        virtual void            write(const CMat& /*image*/){}

    protected:

        std::string             extension();

    protected:

        std::string m_fileName;
};

#endif // CVIRTUALIMAGEIO_H
