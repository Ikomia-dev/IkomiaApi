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

#ifndef CVIDEOPROCESSOF_H
#define CVIDEOPROCESSOF_H

#include "CVideoProcess.h"
#include "DataProcessGlobal.hpp"

/**
 * @ingroup groupDataProcess
 * @brief The CVideoProcessOF class is derived from CVideoProcess and is dedicated to optical flow task based on OpenCV framework.
 * @details This class handles the persistent data required to compute classical optical flow algorithms.
 * It also offers a way to display the computed flow as a color map.
 */
class DATAPROCESSSHARED_EXPORT CVideoProcessOF :  public CVideoProcess
{
    public:

        /**
         * @brief Default constructor.
         * Same inputs and outputs as CImageProcess2d.
         */
        CVideoProcessOF();
        /**
         * @brief Constructs CVideoProcessOF with the given name.
         * Same inputs and outputs as CImageProcess2d.
         * @param name: name of the task.
         */
        CVideoProcessOF(const std::string& name);

        virtual ~CVideoProcessOF() = default;

        /**
         * @brief Adds optical flow vectors to optical flow map (image).
         * @param flow: source optical flow color map. CMat for C++, Numpy array for Python.
         * @param cflowmap: optical flow color map + vectors. CMat for C++, Numpy array for Python.
         * @param step: sampling step.
         */
        void            drawOptFlowMap(const CMat& flow, CMat& cflowmap, int step);
        /**
         * @brief Generates optical flow color map.
         * @param flow: computed optical flow. CMat for C++, Numpy array for Python.
         * @return CMat for C++, Numpy array for Python.
         */
        CMat            flowToDisplay(const CMat& flow);

    protected:

        /**
         * @brief Computes optical flow for the given video frame.
         * @param nextGray: monochrome frame. CMat for C++, Numpy array for Python.
         */
        void            makeOF(const CMat& nextGray);
        /**
         * @brief Prepares input image to be ready for processing.
         * @param imgSrc: source video frame. CMat for C++, Numpy array for Python.
         * @return monochrome frame ready to process. CMat for C++, Numpy array for Python.
         */
        CMat            manageInputs(const CMat& imgSrc);
        /**
         * @brief Copies the optical flow colormap to the task output.
         * It handles graphics input to apply optical flow algorithms on ROIs only.
         * @param imgSrc: input source image. CMat for C++, Numpy array for Python.
         */
        void            manageOuputs(const CMat& imgSrc);

    /** @cond INTERNAL */
    protected:

        CMat                            m_imgPrev;
        CMat                            m_imgFlow;
        cv::Ptr<cv::DenseOpticalFlow>   m_pOF;
        bool                            m_bUseOpenCL = false;
    /** @endcond */
};

#endif // CVIDEOPROCESSOF_H
