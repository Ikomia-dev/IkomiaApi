#include "CDataInfo.h"
#include <QObject>

CDataInfo::CDataInfo(IODataType type)
{
    m_type = type;
    m_fileName.empty();
}

CDataInfo::CDataInfo(IODataType type, const std::string& fileName)
{
    m_type = type;
    m_fileName = fileName;
}

CDataInfo::CDataInfo(const CDataInfo &data)
{
    m_type = data.m_type;
    m_fileName = data.m_fileName;
    m_elementSize = data.m_elementSize;
}

CDataInfo::CDataInfo(CDataInfo &&data)
{
    m_type = std::move(data.m_type);
    m_fileName = std::move(data.m_fileName);
    m_elementSize = std::move(data.m_elementSize);
}

CDataInfo::~CDataInfo()
{
}

CDataInfo& CDataInfo::operator=(const CDataInfo &data)
{
    m_type = data.m_type;
    m_fileName = data.m_fileName;
    m_elementSize = data.m_elementSize;
    return *this;
}

CDataInfo &CDataInfo::operator=(CDataInfo &&data)
{
    m_type = std::move(data.m_type);
    m_fileName = std::move(data.m_fileName);
    m_elementSize = std::move(data.m_elementSize);
    return *this;
}

void CDataInfo::setFileName(const std::string &fileName)
{
    m_fileName = fileName;
}

void CDataInfo::setElementSize(int size)
{
    m_elementSize = size;
}

std::string CDataInfo::getFileName() const
{
    return m_fileName;
}

IODataType CDataInfo::getType() const
{
    return m_type;
}

int CDataInfo::getElementSize() const
{
    return m_elementSize;
}

UMapString CDataInfo::getMetadata() const
{
    return m_metadata;
}

UMapString &CDataInfo::metadata()
{
    return m_metadata;
}

VectorPairString CDataInfo::getStringList() const
{
    VectorPairString infoList;
    infoList.push_back(std::make_pair(QObject::tr("File path").toStdString(), m_fileName));

    if(getMetadata().size() > 0)
        infoList.push_back(std::make_pair("Metadata", QObject::tr("General metadata").toStdString()));

    for(auto&& it : getMetadata())
        infoList.push_back(std::make_pair(it.first, it.second));

    return infoList;
}

