#include "CIkomiaRegistryWrap.h"

CIkomiaRegistryWrap::CIkomiaRegistryWrap(): CIkomiaRegistry()
{
}

CIkomiaRegistryWrap::CIkomiaRegistryWrap(const CIkomiaRegistry &reg): CIkomiaRegistry(reg)
{
}

CTaskInfo CIkomiaRegistryWrap::getAlgorithmInfo(const std::string &name) const
{
    CPyEnsureGIL gil;
    try
    {
        return CIkomiaRegistry::getAlgorithmInfo(name);
    }
    catch(std::exception &e)
    {
        Utils::print(e.what());
        return CTaskInfo();
    }
}
