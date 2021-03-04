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

#ifndef CDATAVIDEOIO_H
#define CDATAVIDEOIO_H

#include <string>
#include <memory>
#include "CVirtualVideoIO.h"
#include "CDataVideoBuffer.h"

/**
 * @brief
 *
 */
class DATAIOSHARED_EXPORT CDataVideoIO
{
    using CVideoIOPtr = std::unique_ptr<CVirtualVideoIO>;

    public:

        CDataVideoIO();
        CDataVideoIO(const std::string& fileName);

        std::vector<std::string>        fileNames(const SubsetBounds& bounds);

        Dimensions                      dimensions();
        Dimensions                      dimensions(const SubsetBounds& bounds);

        CDataInfoPtr                    dataInfo();
        CDataInfoPtr                    dataInfo(const SubsetBounds& bounds);

        CMat                            read();
        CMat                            read(const SubsetBounds& bounds);
        CMat                            readLive();
        CMat                            readLive(const SubsetBounds& bounds);

        void                            write(const CMat &image);
        void                            write(const CMat &image, const std::string& path);

        void                            stopWrite();
        void                            stopRead();

        void                            waitWriteFinished();

        void                            closeCamera();

        static IODataType               dataType(const std::string& fileName);

        static Dimensions               dimensions(const std::string& fileName);

        static bool                     isVideoFormat(const std::string& extension, bool bVideoOnly);
        static bool                     isSameImageSequence(const std::string& fileRenamed, const std::string& fileName);

        static std::pair<std::string, int>  getImageSequenceInfo(const std::string& fileName);

    private:

        void                            allocateDataIOPtr(const std::string& fileName);
        static CVideoIOPtr              _allocateDataIOPtr(const std::string& fileName);

        static bool                     isImageFormat(const std::string& extension);

    private:

        CVideoIOPtr         m_pVideoIO;
};

#endif // CDATAVIDEOIO_H
