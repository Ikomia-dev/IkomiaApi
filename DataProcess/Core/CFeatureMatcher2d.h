#ifndef CFEATUREMATCHER2D_H
#define CFEATUREMATCHER2D_H

#include "CImageProcess2d.h"

class CFeatureMatcher2d : public CImageProcess2d
{
    public:
        using Matches = std::vector<cv::DMatch>;
        using Keypoints = std::vector<cv::KeyPoint>;

        CFeatureMatcher2d();
        CFeatureMatcher2d(const std::string& name);

        virtual ~CFeatureMatcher2d();

    protected:
        void manageInputs();
        void manageOutputs(const CMat& img1, const Keypoints& kp1, const CMat& img2, const Keypoints& kp2, const Matches& matches);

        CMat drawMatchingWithOpenCv(const CMat& img1, const CFeatureMatcher2d::Keypoints& kp1, const CMat& img2, const CFeatureMatcher2d::Keypoints& kp2, const CFeatureMatcher2d::Matches& matches);

    protected:
        cv::Ptr<cv::DescriptorMatcher> m_pMatcher;
};

#endif // CFEATUREMATCHER2D_H
