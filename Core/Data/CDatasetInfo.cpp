#include "CDatasetInfo.h"
#include <algorithm>
#include <cassert>

CDatasetInfo::CDatasetInfo()
{
}

CDatasetInfo::CDatasetInfo(const CDatasetInfo &info)
{
    m_name = info.m_name;
}

CDatasetInfo::CDatasetInfo(CDatasetInfo&& info)
{
    m_name = std::move(info.m_name);
}

CDatasetInfo &CDatasetInfo::operator=(const CDatasetInfo &info)
{
    m_name = info.m_name;
    return *this;
}

CDatasetInfo &CDatasetInfo::operator=(CDatasetInfo &&info)
{
    m_name = std::move(info.m_name);
    return *this;
}

bool CDatasetInfo::operator==(const CDatasetInfo &other) const
{
    return m_name == other.m_name;
}

std::string CDatasetInfo::getName() const
{
    return m_name;
}

void CDatasetInfo::setName(const std::string &name)
{
    m_name = name;
}

