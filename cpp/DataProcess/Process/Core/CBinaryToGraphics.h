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

#ifndef CBINARYTOGRAPHICS_H
#define CBINARYTOGRAPHICS_H

#include "Core/CTaskFactory.hpp"
#include "Core/C2dImageTask.h"

//----------------------------------//
//----- CBinaryToGraphicsParam -----//
//----------------------------------//
class CBinaryToGraphicsParam: public CWorkflowTaskParam
{
    public:

        CBinaryToGraphicsParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;
};

//-----------------------------//
//----- CBinaryToGraphics -----//
//-----------------------------//
class CBinaryToGraphics : public C2dImageTask
{
    public:

        CBinaryToGraphics();
        CBinaryToGraphics(const std::string name, const std::shared_ptr<CBinaryToGraphicsParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;

    private:

        void    computeBinary(const CMat& src);
        void    computeLabel(const CMat& src);
        void    computeGraphics(const CColor &penColor, const CColor &brushColor, const int lineSize);

    private:

        std::vector<cv::Vec4i>              m_hierarchy;
        std::vector<std::vector<cv::Point>> m_blobs;
};

class CBinaryToGraphicsFactory : public CTaskFactory
{
    public:

        CBinaryToGraphicsFactory()
        {
            m_info.m_name = "ik_binary_to_graphics";
            m_info.m_description = QObject::tr("This process extract connected components from binary image and generates the corresponding graphics items.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob,graphics,connected,component";
        }
        ~CBinaryToGraphicsFactory() {}

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CBinaryToGraphicsParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CBinaryToGraphics>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CBinaryToGraphicsParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CBinaryToGraphics>(m_info.m_name, pDerivedParam);
        }
};

#endif // CBINARYTOGRAPHICS_H
