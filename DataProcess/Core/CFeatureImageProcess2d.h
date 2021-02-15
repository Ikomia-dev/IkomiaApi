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

#ifndef CFEATUREIMAGEPROCESS2D_H
#define CFEATUREIMAGEPROCESS2D_H
#include "CImageProcess2d.h"
#include "Graphics/CGraphicsLayer.h"

class DATAPROCESSSHARED_EXPORT CFeatureImageProcess2d : public CImageProcess2d
{
    public:

        using Blobs = std::vector<std::vector<cv::Point>>;

        CFeatureImageProcess2d();
        CFeatureImageProcess2d(const std::string& name);

        virtual ~CFeatureImageProcess2d();

    protected:

        void    addKeypointInput();
        void    addDescriptorAndKeypointOuputs();
        void    addKeypointOutput();
        void    manageInputs();
        void    makeFeatures(const CMat& imgSrc, bool bDetect, bool bCompute);
        void    manageOutputs();
        void    manageGraphicOutputs();

    protected:

        cv::Ptr<cv::Feature2D>      m_pFeatures;
        std::vector<cv::KeyPoint>   m_keypoints;
        CMat                        m_descriptors;
        CMat                        m_imgWithKeypoints;
        bool                        m_bDetect = true;
        bool                        m_bCompute = false;
};

#endif // CFEATUREIMAGEPROCESS2D_H
