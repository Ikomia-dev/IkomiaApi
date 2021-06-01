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

#ifndef CDATAIMAGEIO_H
#define CDATAIMAGEIO_H

#include <string>
#include <memory>
#include "CVirtualImageIO.h"
#include "DataIOGlobal.hpp"
/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CDataImageIO
{
    using CImageIOPtr = std::unique_ptr<CVirtualImageIO>;

    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CDataImageIO(const std::string& fileName);
        /**
         * @brief
         *
         * @return Dimensions
         */
        std::vector<std::string>        fileNames(const SubsetBounds& bounds);

        /**
         * @brief
         *
         * @return Dimensions
         */
        Dimensions                      dimensions();
        /**
         * @brief
         *
         * @return Dimensions
         */
        Dimensions                      dimensions(const SubsetBounds& bounds);
        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        CDataInfoPtr                    dataInfo();

        /**
         * @brief
         *
         * @return CDataInfoPtr
         */
        CDataInfoPtr                    dataInfo(const SubsetBounds& bounds);
        /**
         * @brief
         *
         * @return CMat
         */
        CMat                            read();
        /**
         * @brief
         *
         * @param subset
         * @return CMat
         */
        CMat                            read(const SubsetBounds& bounds);

        void                            write(const CMat &image);

        /**
         * @brief
         *
         * @param fileName
         * @return DataType
         */
        static IODataType               dataType(const std::string& fileName);

        static Dimensions               dimensions(const std::string& fileName);

        static std::set<std::string>    getOpencvExtensions();
        static bool                     isImageFormat(const std::string& extension);
        static bool                     isOpencvCompatible(const std::string& extension);

    private:

        void                            allocateDataIOPtr(const std::string& fileName);
        static CImageIOPtr              _allocateDataIOPtr(const std::string& fileName);

    private:

        CImageIOPtr         m_pImgIO;
};

#endif // CDATAIMAGEIO_H
