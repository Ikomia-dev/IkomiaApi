#include "CProtocolTaskParam.h"
#include <QHash>

CProtocolTaskParam::CProtocolTaskParam()
{
}

void CProtocolTaskParam::setParamMap(const UMapString &paramMap)
{
    Q_UNUSED(paramMap);
}

UMapString CProtocolTaskParam::getParamMap() const
{
    return UMapString();
}

uint CProtocolTaskParam::getHashValue() const
{
    std::vector<QString> values;
    auto paramMap = getParamMap();

    for(auto it=paramMap.begin(); it!=paramMap.end(); ++it)
        values.push_back(QString::fromStdString(it->second));

    return qHashRange(values.begin(), values.end());
}
