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
