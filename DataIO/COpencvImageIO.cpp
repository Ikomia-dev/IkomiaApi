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

#include "COpencvImageIO.h"
#include "Data/CDataImageInfo.h"

COpencvImageIO::COpencvImageIO(const std::string &fileName) : CVirtualImageIO(fileName)
{
}

COpencvImageIO::~COpencvImageIO()
{
}

VectorString COpencvImageIO::fileNames(const SubsetBounds &bounds)
{
    Q_UNUSED(bounds);
    VectorString files = {m_fileName};
    return files;
}

CMat COpencvImageIO::read()
{
    CMat img = cv::imread(m_fileName, cv::IMREAD_UNCHANGED);
    if(img.channels() > 1)
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

    return img;
}

CMat COpencvImageIO::read(const SubsetBounds &subset)
{
    cv::Rect roi;
    CMat img = cv::imread(m_fileName, cv::IMREAD_UNCHANGED);

    if(img.channels() > 1)
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

    for(size_t i=0; i<subset.size(); ++i)
    {
        if(subset[i].first == DataDimension::X)
        {
            roi.x = static_cast<int>(subset[i].second.first);
            roi.width = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
        else if(subset[i].first == DataDimension::Y)
        {
            roi.y = static_cast<int>(subset[i].second.first);
            roi.height = static_cast<int>(subset[i].second.second - subset[i].second.first);
        }
    }

    if(roi.width == 0 && roi.height == 0)
        return img;
    else
    {
        if(roi.width == 0)
            roi.width = img.cols;
        if(roi.height == 0)
            roi.height = img.rows;

        return img(roi);
    }
}

void COpencvImageIO::write(const CMat &image)
{
    if(image.data)
    {
        if(image.dims == 3)
        {
            auto folder = Utils::File::getParentPath(m_fileName);
            auto baseName = Utils::File::getFileNameWithoutExtension(m_fileName);
            auto extension = Utils::File::extension(m_fileName);

            for(size_t i=0; i<image.getNbStacks(); ++i)
            {
                std::string strNumber = std::to_string(i);
                std::string path = folder + "/" + baseName + std::string(4 - strNumber.length(), '0') + strNumber + extension;
                writeSingle(path, image.getPlane(i));
            }
        }
        else if(image.dims == 2)
            writeSingle(m_fileName, image);
        else
            throw CException(CoreExCode::INVALID_SIZE, "Invalid image dimensions", __func__, __FILE__, __LINE__);
    }
}

void COpencvImageIO::writeSingle(const std::string &path, const CMat &image)
{
    CMat imgTmp;

    if(image.channels() > 1)
        cv::cvtColor(image, imgTmp, cv::COLOR_RGB2BGR);
    else
        imgTmp = image;

    cv::imwrite(m_fileName, imgTmp);
}

Dimensions COpencvImageIO::dimensions()
{
    Dimensions dims = {{DataDimension::IMAGE, 1}};
    return dims;
}

Dimensions COpencvImageIO::dimensions(const SubsetBounds &bounds)
{
    Q_UNUSED(bounds);
    return dimensions();
}

CDataInfoPtr COpencvImageIO::dataInfo()
{
    cv::Mat img = cv::imread(m_fileName, cv::IMREAD_UNCHANGED);
    auto pInfo = std::make_shared<CDataImageInfo>();
    pInfo->setFileName(m_fileName);
    pInfo->setCvType(img.type());
    pInfo->m_width = img.cols;
    pInfo->m_height = img.rows;
    return pInfo;
}

CDataInfoPtr COpencvImageIO::dataInfo(const SubsetBounds &/*subset*/)
{
    return dataInfo();
}

