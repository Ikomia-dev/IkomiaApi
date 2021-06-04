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

#ifndef CRGBHLSTHRESHOLD_H
#define CRGBHLSTHRESHOLD_H

#include "Core/C2dImageInteractiveTask.h"

//---------------------------------//
//----- CRgbHlsThresholdParam -----//
//---------------------------------//
class CRgbHlsThresholdParam: public CWorkflowTaskParam
{
    public:

        enum ActionFlags
        {
            CLEAR_SEGMENTATION = 0x00000001
        };

        CRgbHlsThresholdParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        int m_minR = 0;
        int m_maxR = 0;
        int m_minG = 0;
        int m_maxG = 0;
        int m_minB = 0;
        int m_maxB = 0;
        int m_minH = 0;
        int m_maxH = 0;
        int m_minL = 0;
        int m_maxL = 0;
        int m_minS = 0;
        int m_maxS = 0;
};

//----------------------------//
//----- CRgbHlsThreshold -----//
//----------------------------//
class CRgbHlsThreshold : public C2dImageInteractiveTask
{
    public:

        CRgbHlsThreshold();
        CRgbHlsThreshold(const std::string name, const std::shared_ptr<CRgbHlsThresholdParam>& pParam);

        size_t  getProgressSteps() override;

        void    graphicsChanged() override;

        void    run() override;

        void    executeActions(int flags) override;

    private:

        void    updateThresholds();
};

//-----------------------------------//
//----- CRgbHlsThresholdFactory -----//
//-----------------------------------//
class CRgbHlsThresholdFactory : public CTaskFactory
{
    public:

        CRgbHlsThresholdFactory();
        ~CRgbHlsThresholdFactory() {}

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override;
        virtual WorkflowTaskPtr create() override;
};

#endif // CRGBHLSTHRESHOLD_H
