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
