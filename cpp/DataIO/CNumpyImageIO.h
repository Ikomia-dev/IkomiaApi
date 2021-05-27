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

#ifndef CNUMPYIMAGEIO_H
#define CNUMPYIMAGEIO_H

#include "CVirtualImageIO.h"
#include "DataIOGlobal.hpp"

//Avoid conflict with Qt slots keyword
#undef slots
#include <Python.h>
#define slots

#include "boost/python.hpp"


class DATAIOSHARED_EXPORT CNumpyImageIO : public CVirtualImageIO
{
    public:

        CNumpyImageIO(const std::string& fileName);

        virtual ~CNumpyImageIO();

        VectorString    fileNames(const SubsetBounds& bounds) override;

        Dimensions      dimensions() override;
        Dimensions      dimensions(const SubsetBounds& bounds) override;

        CDataInfoPtr    dataInfo() override;
        CDataInfoPtr    dataInfo(const SubsetBounds& bounds) override;

        CMat            read() override;
        CMat            read(const SubsetBounds& bounds) override;

    private:

        //bool            initNumpy();
        void            loadNumpyArray();

    private:

        std::vector<int>    m_dims;
        std::vector<CMat>   m_images;
};

#endif // CNUMPYIMAGEIO_H
