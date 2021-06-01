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

#ifndef COCVSUPERRESBTVL1_H
#define COCVSUPERRESBTVL1_H

#include "Core/CVideoProcess.h"
#include "CSuperResFrameSrc.h"

//----------------------------------//
//----- COcvSuperResBTVL1Param -----//
//----------------------------------//
class COcvSuperResBTVL1Param : public CProtocolTaskParam
{
    public:

        enum OpticalFlowType
        {
            BROX,
            DUAL_TVL1,
            FARNEBACK,
            PYR_LK
        };

        COcvSuperResBTVL1Param();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        int     m_scale = 2;                    //Scale factor.
        int     m_iterations = 5;//180;             //Iteration count.
        double  m_tau = 1.3;                    //Asymptotic value of steepest descent method.
        double  m_lambda = 0.03;                //Weight parameter to balance data term and smoothness term.
        double  m_alpha = 0.7;                  //Parameter of spacial distribution in Bilateral-TV.
        int     m_btvKernelSize = 7;            //Kernel size of Bilateral-TV filter.
        int     m_blurKernelSize = 5;           //Gaussian blur kernel size.
        double  m_blurSigma = 1.0;              //Gaussian blur sigma.
        int     m_temporalAreaRadius = 4;       //Radius of the temporal search area.
        int     m_opticalFlowType = FARNEBACK;  //Dense optical flow algorithm.
};

//-----------------------------//
//----- COcvSuperResBTVL1 -----//
//-----------------------------//
class COcvSuperResBTVL1 : public CVideoProcess
{
    public:

        COcvSuperResBTVL1();
        COcvSuperResBTVL1(const std::string name, const std::shared_ptr<COcvSuperResBTVL1Param>& pParam);

        size_t     getProgressSteps() override;

        void    notifyVideoStart(int frameCount) override;

        void    run() override;

    private:

        cv::Ptr<cv::superres::DenseOpticalFlowExt>  createOpticalFlow();

    protected:

        bool                                    m_bInit = false;
        bool                                    m_bVideoStart = false;
        bool                                    m_bCuda = false;
        cv::Ptr<cv::superres::SuperResolution>  m_superResPtr;
        cv::Ptr<CSuperResFrameSrc>              m_frameSrcPtr;
};

//------------------------------------//
//----- COcvSuperResBTVL1Factory -----//
//------------------------------------//
class COcvSuperResBTVL1Factory : public CProcessFactory
{
    public:

        COcvSuperResBTVL1Factory();

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override;
        virtual ProtocolTaskPtr create() override;
};

#endif // COCVSUPERRESBTVL1_H
