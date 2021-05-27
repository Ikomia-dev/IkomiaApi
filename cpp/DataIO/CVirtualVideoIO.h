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

#ifndef CVIRTUALVIDEOIO_H
#define CVIRTUALVIDEOIO_H

#include <string>
#include "DataIOGlobal.hpp"
#include "Main/CoreDefine.hpp"
#include "Data/CMat.hpp"
#include "Data/CDataInfo.h"

/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CVirtualVideoIO
{
    public:

        /**
         * @brief
         *
         * @param fileName
         */
        CVirtualVideoIO(const std::string& fileName);
        /**
         * @brief
         *
         */
        virtual ~CVirtualVideoIO();

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
        virtual CMat            readLive() = 0;
        virtual CMat            read(const SubsetBounds& subset) = 0;
        virtual CMat            readLive(const SubsetBounds& subset) = 0;

        virtual void            write(const CMat& /*image*/) = 0;
        virtual void            write(const CMat& /*image*/, const std::string& /*path*/) = 0;

        virtual void            stopWrite() = 0;
        virtual void            stopRead() = 0;

        virtual void            waitWriteFinished() = 0;

        virtual void            closeCamera();

    protected:

        std::string             extension();

    protected:

        std::string m_fileName;
};

#endif // CVIRTUALVIDEOIO_H
