#include "CDataDicomInfo.h"

CDataDicomInfo::CDataDicomInfo() : CDataImageInfo()
{
}

CDataDicomInfo::CDataDicomInfo(const CDataDicomInfo &info) : CDataImageInfo(info)
{
    m_nbStudy = info.m_nbStudy;
    m_nbSerie = info.m_nbSerie;
}

CDataDicomInfo::CDataDicomInfo(const CDataDicomInfo&& info) : CDataImageInfo(info)
{
    m_nbStudy = std::move(info.m_nbStudy);
    m_nbSerie = std::move(info.m_nbSerie);
}

CDataDicomInfo::~CDataDicomInfo()
{
}

CDataDicomInfo &CDataDicomInfo::operator=(const CDataDicomInfo& info)
{
    CDataImageInfo::operator=(info);
    m_nbStudy = info.m_nbStudy;
    m_nbSerie = info.m_nbSerie;
    return *this;
}

CDataDicomInfo &CDataDicomInfo::operator=(const CDataDicomInfo&& info)
{
    CDataImageInfo::operator=(info);
    m_nbStudy = std::move(info.m_nbStudy);
    m_nbSerie = std::move(info.m_nbSerie);
    return *this;
}

VectorPairString CDataDicomInfo::getStringList() const
{
    VectorPairString infoList =  CDataImageInfo::getStringList();

    auto it = std::find_if(infoList.begin(), infoList.end(), [](const PairString& elt){ return elt.first == "Metadata"; });
    if(it != infoList.end())
        it->second = "DICOM metadata";

    return infoList;
}
