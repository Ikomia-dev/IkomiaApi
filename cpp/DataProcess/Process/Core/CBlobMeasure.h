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

#ifndef CBLOBMEASURE_HPP
#define CBLOBMEASURE_HPP

#include "Core/CTaskFactory.hpp"
#include "Core/C2dImageTask.h"
#include "IO/CVideoIO.h"
#include "IO/CBlobMeasureIO.h"

//-----------------------------//
//----- CBlobMeasureParam -----//
//-----------------------------//
class CBlobMeasureParam: public CWorkflowTaskParam
{
    public:

        CBlobMeasureParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        std::vector<CMeasure>   m_measures;
};

//------------------------//
//----- CBlobMeasure -----//
//------------------------//
class CBlobMeasure : public C2dImageTask
{
    public:

        CBlobMeasure();
        CBlobMeasure(const std::string name, const std::shared_ptr<CBlobMeasureParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;

        void    updateStaticOutputs() override;

    private:

        void            compute(CMat imgSrc);
        void            computeFromLabel(CMat imgSrc);
        void            computeGraphics(const CColor &penColor, const CColor &brushColor, const int lineSize);
        void            computeMeasures(const std::string &label);
        ObjectMeasures  computeBlobMeasures(const std::vector<cv::Point>& blob, const std::vector<std::vector<cv::Point>>& holes, size_t graphicsId, const std::string &label);
        void            computeCustomMeasure(ObjectMeasures& blobMeasures, const std::string& formula, size_t graphicsId);

    private:

        std::vector<cv::Vec4i>              m_hierarchy;
        std::vector<std::vector<cv::Point>> m_blobs;
};

class CBlobMeasureFactory : public CTaskFactory
{
    public:

        CBlobMeasureFactory()
        {
            m_info.m_name = "ik_blob_measurement";
            m_info.m_description = QObject::tr("This process extract connected components from binary image and calculate selected measures for each.").toStdString();
            m_info.m_path = QObject::tr("Core/Measurements").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob, quantification, measure, connected component";
        }
        ~CBlobMeasureFactory() {}

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CBlobMeasureParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CBlobMeasure>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CBlobMeasureParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CBlobMeasure>(m_info.m_name, pDerivedParam);
        } 
};

#endif // CBLOBMEASURE_HPP
