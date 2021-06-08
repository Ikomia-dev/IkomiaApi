/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CGRAPHICSTOBINARY_H
#define CGRAPHICSTOBINARY_H

#include "Core/CTaskFactory.hpp"
#include "Core/C2dImageTask.h"

//----------------------------------//
//----- CGraphicsToBinaryParam -----//
//----------------------------------//
class CGraphicsToBinaryParam: public CWorkflowTaskParam
{
    public:

        CGraphicsToBinaryParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        bool    m_bFitToContent = true;
        int     m_width = 1024;
        int     m_height = 1024;
};

//-----------------------------//
//----- CGraphicsToBinary -----//
//-----------------------------//
class CGraphicsToBinary : public C2dImageTask
{
    public:

        CGraphicsToBinary();
        CGraphicsToBinary(const std::string name, const std::shared_ptr<CGraphicsToBinaryParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;
};

class CGraphicsToBinaryFactory : public CTaskFactory
{
    public:

        CGraphicsToBinaryFactory()
        {
            m_info.m_name = QObject::tr("Graphics to binary").toStdString();
            m_info.m_description = QObject::tr("This process generates binary mask from graphics items.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob,graphics,mask,binary,connected,component";
        }
        ~CGraphicsToBinaryFactory() {}

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGraphicsToBinaryParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGraphicsToBinary>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CGraphicsToBinaryParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CGraphicsToBinary>(m_info.m_name, pDerivedParam);
        }
};

#endif // CGRAPHICSTOBINARY_H
