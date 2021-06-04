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

#ifndef C2DFEATUREMATCHER_H
#define C2DFEATUREMATCHER_H

#include "C2dImageTask.h"

class C2dFeatureMatcherTask : public C2dImageTask
{
    public:

        using Matches = std::vector<cv::DMatch>;
        using Keypoints = std::vector<cv::KeyPoint>;

        C2dFeatureMatcherTask();
        C2dFeatureMatcherTask(const std::string& name);

        virtual ~C2dFeatureMatcherTask();

    protected:

        void manageInputs();
        void manageOutputs(const CMat& img1, const Keypoints& kp1, const CMat& img2, const Keypoints& kp2, const Matches& matches);

        CMat drawMatchingWithOpenCv(const CMat& img1, const C2dFeatureMatcherTask::Keypoints& kp1,
                                    const CMat& img2, const C2dFeatureMatcherTask::Keypoints& kp2,
                                    const C2dFeatureMatcherTask::Matches& matches);

    protected:

        cv::Ptr<cv::DescriptorMatcher> m_pMatcher;
};

#endif // C2DFEATUREMATCHER_H
