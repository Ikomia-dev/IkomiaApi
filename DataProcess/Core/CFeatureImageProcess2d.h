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
