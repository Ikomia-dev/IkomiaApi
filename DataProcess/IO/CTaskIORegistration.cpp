#include "CTaskIORegistration.h"
#include "CImageProcessIO.h"
#include "CMeasureProcessIO.h"
#include "CGraphicsProcessInput.h"
#include "CGraphicsProcessOutput.h"
#include "CFeatureProcessIO.hpp"
#include "CVideoProcessIO.h"
#include "CWidgetOutput.h"
#include "CFolderInput.h"
#include "CPathIO.h"

CTaskIORegistration::CTaskIORegistration()
{
    registerCore();
}

const CProtocolTaskIOAbstractFactory &CTaskIORegistration::getFactory() const
{
    return m_factory;
}

void CTaskIORegistration::registerIO(const std::shared_ptr<CProtocolTaskIOFactory> &pFactory)
{
    m_factory.getList().push_back(pFactory);
    //Passage par lambda -> pFactory par valeur pour assurer la portée du pointeur
    auto pCreatorFunc = [pFactory](IODataType dataType){ return pFactory->create(dataType); };
    m_factory.registerCreator(pFactory->getName(), pCreatorFunc);
}

void CTaskIORegistration::reset()
{
    m_factory.getList().clear();
    registerCore();
}

void CTaskIORegistration::registerCore()
{
    registerIO(std::make_shared<CImageProcessIOFactory>());
    registerIO(std::make_shared<CMeasureProcessIOFactory>());
    registerIO(std::make_shared<CGraphicsProcessInputFactory>());
    registerIO(std::make_shared<CGraphicsProcessOutputFactory>());
    registerIO(std::make_shared<CFeatureProcessIOFactory>());
    registerIO(std::make_shared<CVideoProcessIOFactory>());
    registerIO(std::make_shared<CWidgetOutputFactory>());
    registerIO(std::make_shared<CFolderInputFactory>());
    registerIO(std::make_shared<CPathIOFactory>());
}
