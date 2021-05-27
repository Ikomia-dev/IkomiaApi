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

#include "CFeatureMatcher2d.h"

CFeatureMatcher2d::CFeatureMatcher2d() : CImageProcess2d()
{
    clearInputs();
    addInput(std::make_shared<CImageProcessIO>()); // Image originale 1
    addInput(std::make_shared<CImageProcessIO>(IODataType::DESCRIPTORS)); // Descriptor 1
    addInput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>()); // Keypoints 1

    addInput(std::make_shared<CImageProcessIO>()); // Image originale 2
    addInput(std::make_shared<CImageProcessIO>(IODataType::DESCRIPTORS)); // Descriptor 2
    addInput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>()); // Keypoints 2

    addInput(std::make_shared<CGraphicsProcessInput>());
}

CFeatureMatcher2d::CFeatureMatcher2d(const std::string& name) : CImageProcess2d(name)
{
    clearInputs();
    addInput(std::make_shared<CImageProcessIO>()); // Image originale 1
    addInput(std::make_shared<CImageProcessIO>(IODataType::DESCRIPTORS)); // Descriptor 1
    addInput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>()); // Keypoints 1

    addInput(std::make_shared<CImageProcessIO>()); // Image originale 2
    addInput(std::make_shared<CImageProcessIO>(IODataType::DESCRIPTORS)); // Descriptor 2
    addInput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>()); // Keypoints 2

    addInput(std::make_shared<CGraphicsProcessInput>());
}

CFeatureMatcher2d::~CFeatureMatcher2d()
{

}

void CFeatureMatcher2d::manageInputs()
{

}

void CFeatureMatcher2d::manageOutputs(const CMat& img1, const CFeatureMatcher2d::Keypoints& kp1, const CMat& img2, const CFeatureMatcher2d::Keypoints& kp2, const CFeatureMatcher2d::Matches& matches)
{
    auto img_matches = drawMatchingWithOpenCv(img1, kp1, img2, kp2, matches);

    auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
    if(pOutput)
        pOutput->setImage(img_matches);
}

CMat CFeatureMatcher2d::drawMatchingWithOpenCv(const CMat& img1, const CFeatureMatcher2d::Keypoints& kp1, const CMat& img2, const CFeatureMatcher2d::Keypoints& kp2, const CFeatureMatcher2d::Matches& matches)
{
    double max_dist = 0; double min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( size_t i = 0; i < matches.size(); i++ )
    { double dist = matches[i].distance;
      if( dist < min_dist ) min_dist = dist;
      if( dist > max_dist ) max_dist = dist;
    }

    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
    //-- small)
    //-- PS.- radiusMatch can also be used here.
    std::vector< cv::DMatch > good_matches;
    for( size_t i = 0; i < matches.size(); i++ )
    { if( matches[i].distance <= std::max(2*min_dist, 0.02) )
      { good_matches.push_back( matches[i]); }
    }
    //-- Draw only "good" matches
    CMat img_matches;
    cv::drawMatches( img1, kp1, img2, kp2,
                 good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    return img_matches;
}
