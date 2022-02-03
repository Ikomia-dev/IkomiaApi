#ifndef CIKOMIAREGISTRYWRAP_H
#define CIKOMIAREGISTRYWRAP_H

#include "PyDataProcessGlobal.h"
#include "Core/CIkomiaRegistry.h"


class CIkomiaRegistryWrap: public CIkomiaRegistry, public wrapper<CIkomiaRegistry>
{
    public:

        CIkomiaRegistryWrap();
        ~CIkomiaRegistryWrap() = default;

        CIkomiaRegistryWrap(const CIkomiaRegistry& reg);

        CTaskInfo getAlgorithmInfo(const std::string& name) const;

};

#endif // CIKOMIAREGISTRYWRAP_H
