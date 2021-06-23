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
#include "CImageIO.h"
#include "CMeasureIO.h"
#include "CGraphicsInput.h"
#include "CGraphicsOutput.h"
#include "CFeatureIO.hpp"
#include "CVideoIO.h"
#include "CWidgetOutput.h"
#include "CPathIO.h"

CTaskIORegistration::CTaskIORegistration()
{
    registerCore();
}

CTaskIORegistration::~CTaskIORegistration()
{
    CPyEnsureGIL gil;
    m_factory.clear();
}

const CWorkflowTaskIOAbstractFactory &CTaskIORegistration::getFactory() const
{
    return m_factory;
}

void CTaskIORegistration::registerIO(const TaskIOFactoryPtr &pFactory)
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
    registerIO(std::make_shared<CImageIOFactory>());
    registerIO(std::make_shared<CMeasureIOFactory>());
    registerIO(std::make_shared<CGraphicsInputFactory>());
    registerIO(std::make_shared<CGraphicsOutputFactory>());
    registerIO(std::make_shared<CFeatureIOFactory>());
    registerIO(std::make_shared<CVideoIOFactory>());
    registerIO(std::make_shared<CWidgetOutputFactory>());
    registerIO(std::make_shared<CPathIOFactory>());
}
