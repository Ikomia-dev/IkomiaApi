#include "CDataImageInfo.h"
#include "opencv2/opencv.hpp"
#include <QObject>

CDataImageInfo::CDataImageInfo() : CDataInfo(IODataType::IMAGE)
{
}

CDataImageInfo::CDataImageInfo(IODataType type) : CDataInfo(type)
{
}

CDataImageInfo::CDataImageInfo(const std::string& fileName) : CDataInfo(IODataType::IMAGE, fileName)
{
}

CDataImageInfo::CDataImageInfo(IODataType type, const std::string& fileName) : CDataInfo(type, fileName)
{
}

CDataImageInfo::CDataImageInfo(const CMat &image) : CDataInfo(IODataType::IMAGE)
{
    updateImage(image);
}

CDataImageInfo::CDataImageInfo(const CDataImageInfo &info) : CDataInfo(info)
{
    m_cvType = info.m_cvType;
    m_width = info.m_width;
    m_height = info.m_height;
    m_depth = info.m_depth;
    m_nbBand = info.m_nbBand;
    m_nbBit = info.m_nbBit;
    m_bitMin = info.m_bitMin;
    m_bitMax = info.m_bitMax;
    m_minValue = info.m_minValue;
    m_maxValue = info.m_maxValue;
}

CDataImageInfo::CDataImageInfo(CDataImageInfo &&info) : CDataInfo(info)
{
    m_cvType = std::move(info.m_cvType);
    m_width = std::move(info.m_width);
    m_height = std::move(info.m_height);
    m_depth = std::move(info.m_depth);
    m_nbBand = std::move(info.m_nbBand);
    m_nbBit = std::move(info.m_nbBit);
    m_bitMin = std::move(info.m_bitMin);
    m_bitMax = std::move(info.m_bitMax);
    m_minValue = std::move(info.m_minValue);
    m_maxValue = std::move(info.m_maxValue);
}

CDataImageInfo::~CDataImageInfo()
{
}

CDataImageInfo& CDataImageInfo::operator=(const CDataImageInfo &info)
{
    CDataInfo::operator =(info);
    m_cvType = info.m_cvType;
    m_width = info.m_width;
    m_height = info.m_height;
    m_depth = info.m_depth;
    m_nbBand = info.m_nbBand;
    m_nbBit = info.m_nbBit;
    m_bitMin = info.m_bitMin;
    m_bitMax = info.m_bitMax;
    m_minValue = info.m_minValue;
    m_maxValue = info.m_maxValue;
    return *this;
}

CDataImageInfo &CDataImageInfo::operator=(CDataImageInfo &&info)
{
    m_cvType = std::move(info.m_cvType);
    m_width = std::move(info.m_width);
    m_height = std::move(info.m_height);
    m_depth = std::move(info.m_depth);
    m_nbBand = std::move(info.m_nbBand);
    m_nbBit = std::move(info.m_nbBit);
    m_bitMin = std::move(info.m_bitMin);
    m_bitMax = std::move(info.m_bitMax);
    m_minValue = std::move(info.m_minValue);
    m_maxValue = std::move(info.m_maxValue);
    return *this;
}

void CDataImageInfo::setCvType(int type)
{
    m_cvType = type;
    switch(type)
    {
        case CV_8UC1:
            m_nbBand = 1;
            m_nbBit = 8;
            m_bitMin = 0;
            m_bitMax = 7;
            break;

        case CV_8UC3:
            m_nbBand = 3;
            m_nbBit = 8;
            m_bitMin = 0;
            m_bitMax = 7;
            break;

        case CV_8UC4:
            m_nbBand = 4;
            m_nbBit = 8;
            m_bitMin = 0;
            m_bitMax = 7;
            break;

        case CV_16UC1:
            m_nbBand = 1;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_16UC3:
            m_nbBand = 3;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_16UC4:
            m_nbBand = 4;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_16SC1:
            m_nbBand = 1;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_16SC3:
            m_nbBand = 3;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_16SC4:
            m_nbBand = 4;
            m_nbBit = 16;
            m_bitMin = 0;
            m_bitMax = 15;
            break;

        case CV_32SC1:
            m_nbBand = 1;
            m_nbBit = 32;
            m_bitMin = 0;
            m_bitMax = 31;
            break;

        case CV_32SC3:
            m_nbBand = 3;
            m_nbBit = 32;
            m_bitMin = 0;
            m_bitMax = 31;
            break;

        case CV_32SC4:
            m_nbBand = 4;
            m_nbBit = 32;
            m_bitMin = 0;
            m_bitMax = 31;
            break;

        case CV_32FC1:
            m_nbBand = 1;
            m_nbBit = 32;
            m_bitMin = -1;
            m_bitMax = -1;
            break;

        case CV_32FC3:
            m_nbBand = 3;
            m_nbBit = 32;
            m_bitMin = -1;
            m_bitMax = -1;
            break;

        case CV_32FC4:
            m_nbBand = 4;
            m_nbBit = 32;
            m_bitMin = -1;
            m_bitMax = -1;
            break;

        case CV_64FC1:
            m_nbBand = 1;
            m_nbBit = 64;
            m_bitMin = -1;
            m_bitMax = -1;
            break;

        case CV_64FC3:
            m_nbBand = 3;
            m_nbBit = 64;
            m_bitMin = -1;
            m_bitMax = -1;
            break;

        case CV_64FC4:
            m_nbBand = 4;
            m_nbBit = 64;
            m_bitMin = -1;
            m_bitMax = -1;
            break;
    }
}

VectorPairString CDataImageInfo::getStringList() const
{
    VectorPairString infoList = CDataInfo::getStringList();

    VectorPairString newList;
    newList.push_back(std::make_pair(QObject::tr("Width").toStdString(), std::to_string(m_width)));
    newList.push_back(std::make_pair(QObject::tr("Height").toStdString(), std::to_string(m_height)));
    newList.push_back(std::make_pair(QObject::tr("Channels").toStdString(), std::to_string(m_nbBand)));
    newList.push_back(std::make_pair(QObject::tr("Type").toStdString(), getTypeString()));
    newList.push_back(std::make_pair(QObject::tr("Bit depth").toStdString(), std::to_string(m_nbBit)));
    newList.push_back(std::make_pair(QObject::tr("Min bit").toStdString(), std::to_string(m_bitMin)));
    newList.push_back(std::make_pair(QObject::tr("Max bit").toStdString(), std::to_string(m_bitMax)));
    newList.push_back(std::make_pair(QObject::tr("Min value").toStdString(), std::to_string(m_minValue)));
    newList.push_back(std::make_pair(QObject::tr("Max value").toStdString(), std::to_string(m_maxValue)));

    infoList.insert(infoList.begin() + 1, newList.begin(), newList.end());
    return infoList;
}

void CDataImageInfo::updateImage(const CMat &image)
{
    if(image.empty())
        return;

    setCvType(image.type());
    m_width = (int)image.getNbCols();
    m_height = (int)image.getNbRows();
    computeMinMax(image);
    setElementSize(m_nbBit / 8);
}

void CDataImageInfo::computeMinMax(const CMat &image)
{
    //cv::minMaxLoc does not support multi-channels input
    CMat srcImg;
    if(image.dims == 3)
        srcImg = image.getPlane(0);
    else
        srcImg = image;

    if(image.channels() > 1)
    {
        auto tmpImg = srcImg.reshape(1, 0);
        cv::minMaxLoc(tmpImg, &m_minValue, &m_maxValue);
    }
    else
        cv::minMaxLoc(srcImg, &m_minValue, &m_maxValue);
}

std::string CDataImageInfo::getTypeString() const
{
    std::string type;
    switch(m_cvType)
    {
        case CV_8UC1:
        case CV_8UC3:
        case CV_8UC4:
        case CV_16UC1:
        case CV_16UC3:
        case CV_16UC4:
            type = "unsigned integer";
            break;

        case CV_16SC1:
        case CV_16SC3:
        case CV_16SC4:
        case CV_32SC1:
        case CV_32SC3:
        case CV_32SC4:
            type = "signed integer";
            break;

        case CV_32FC1:
        case CV_32FC3:
        case CV_32FC4:
        case CV_64FC1:
        case CV_64FC3:
        case CV_64FC4:
            type = "float";
            break;
    }
    return type;
}

