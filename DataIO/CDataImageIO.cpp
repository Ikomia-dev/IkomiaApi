// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "CDataImageIO.h"
#include <algorithm>
#include "COpencvImageIO.h"
#include "CDicomImageIO.h"
#include "DataIOTools.hpp"
#include "CNumpyImageIO.h"

CDataImageIO::CDataImageIO(const std::string &fileName)
{
    m_pImgIO = nullptr;
    allocateDataIOPtr(fileName);
}

std::vector<std::string> CDataImageIO::fileNames(const SubsetBounds &bounds)
{
    if(m_pImgIO)
        return m_pImgIO->fileNames(bounds);
    else
        return std::vector<std::string>();
}

Dimensions CDataImageIO::dimensions()
{
    if(m_pImgIO)
        return m_pImgIO->dimensions();
    else
        return Dimensions();
}

Dimensions CDataImageIO::dimensions(const SubsetBounds &bounds)
{
    if(m_pImgIO)
        return m_pImgIO->dimensions(bounds);
    else
        return Dimensions();
}

CDataInfoPtr CDataImageIO::dataInfo()
{
    if(m_pImgIO)
        return m_pImgIO->dataInfo();
    else
        return nullptr;
}

CDataInfoPtr CDataImageIO::dataInfo(const SubsetBounds &bounds)
{
    if(m_pImgIO)
        return m_pImgIO->dataInfo(bounds);
    else
        return nullptr;
}

CMat CDataImageIO::read()
{
    if(m_pImgIO)
        return m_pImgIO->read();
    else
        return CMat();
}

CMat CDataImageIO::read(const SubsetBounds &bounds)
{
    if(m_pImgIO)
        return m_pImgIO->read(bounds);
    else
        return CMat();
}

void CDataImageIO::write(const CMat& image)
{
    if(m_pImgIO)
        m_pImgIO->write(image);
    else
        throw CException(CoreExCode::NULL_POINTER, "Write error: invalid DataImageIO object", __func__, __FILE__, __LINE__);
}

IODataType CDataImageIO::dataType(const std::string &fileName)
{
    if(isImageFormat(Utils::File::extension(fileName)))
        return IODataType::IMAGE;
    else
        return IODataType::IMAGE;
}

Dimensions CDataImageIO::dimensions(const std::string &fileName)
{
    auto pImageIO = _allocateDataIOPtr(fileName);
    if(pImageIO)
        return pImageIO->dimensions();
    else
        return Dimensions();
}

std::set<std::string> CDataImageIO::getOpencvExtensions()
{
    /* Extract from OpenCV documentation
    Windows bitmaps - *.bmp, *.dib (always supported)
    JPEG files - *.jpeg, *.jpg, *.jpe (see the Notes section)
    JPEG 2000 files - *.jp2 (see the Notes section)
    Portable Network Graphics - *.png (see the Notes section)
    WebP - *.webp (see the Notes section)
    Portable image format - *.pbm, *.pgm, *.ppm *.pxm, *.pnm (always supported)
    Sun rasters - *.sr, *.ras (always supported)
    TIFF files - *.tiff, *.tif (see the Notes section)
    OpenEXR Image files - *.exr (see the Notes section)
    Radiance HDR - *.hdr, *.pic (always supported)
    Raster and Vector geospatial data supported by Gdal (see the Notes section)*/
    std::set<std::string> extensions =
    {
        ".bmp", ".dib", ".jpeg", ".jpg", ".jpe", ".jp2", ".png", ".webp", ".pbm", ".pgm",
        ".ppm", ".pxm", ".pnm", ".sr", ".ras", ".tiff", ".tif", ".exr", ".hdr", ".pic"
    };
    return extensions;
}

void CDataImageIO::allocateDataIOPtr(const std::string &fileName)
{
    if(boost::filesystem::is_directory(fileName))
        m_pImgIO = std::make_unique<CDicomImageIO>(fileName);
    else
    {
        std::string extension = Utils::File::extension(fileName);

        if(isOpencvCompatible(extension))
            m_pImgIO = std::make_unique<COpencvImageIO>(fileName);
        else if(extension == ".dcm" || extension == "")
            m_pImgIO = std::make_unique<CDicomImageIO>(fileName);
        else if(extension == ".npz")
            m_pImgIO = std::make_unique<CNumpyImageIO>(fileName);
    }
}

CDataImageIO::CImageIOPtr CDataImageIO::_allocateDataIOPtr(const std::string &fileName)
{
    if(boost::filesystem::is_directory(fileName))
        return std::make_unique<CDicomImageIO>(fileName);
    else
    {
        std::string extension = Utils::File::extension(fileName);

        if(isOpencvCompatible(extension))
            return std::make_unique<COpencvImageIO>(fileName);
        else if(extension == ".dcm")
            return std::make_unique<CDicomImageIO>(fileName);
        else if(extension == ".npz")
            return std::make_unique<CNumpyImageIO>(fileName);
        else
            return nullptr;
    }
}

bool CDataImageIO::isImageFormat(const std::string &extension)
{
    return isOpencvCompatible(extension);
}

bool CDataImageIO::isOpencvCompatible(const std::string &extension)
{
    auto extensions = getOpencvExtensions();
    auto it = extensions.find(extension);
    return it != extensions.end();
}

