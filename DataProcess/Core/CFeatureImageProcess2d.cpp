#include "CFeatureImageProcess2d.h"

CFeatureImageProcess2d::CFeatureImageProcess2d() : CImageProcess2d()
{
}

CFeatureImageProcess2d::CFeatureImageProcess2d(const std::string& name) : CImageProcess2d(name)
{
}

CFeatureImageProcess2d::~CFeatureImageProcess2d()
{

}

void CFeatureImageProcess2d::addKeypointInput()
{
    addInput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>());
}

void CFeatureImageProcess2d::addDescriptorAndKeypointOuputs()
{
    addOutput(std::make_shared<CImageProcessIO>(IODataType::DESCRIPTORS));
    addOutput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

void CFeatureImageProcess2d::addKeypointOutput()
{
    addOutput(std::make_shared<CFeatureProcessIO<cv::KeyPoint>>());
    addOutput(std::make_shared<CGraphicsProcessOutput>());
}

void CFeatureImageProcess2d::manageInputs()
{
    if(getInputCount()<3)
        return;

    auto pInput = std::dynamic_pointer_cast<CFeatureProcessIO<cv::KeyPoint>>(getInput(2));
    if(pInput)
        if(pInput->getAllValues().size()>0)
            m_keypoints = pInput->getValueList(0);
}

void CFeatureImageProcess2d::makeFeatures(const CMat& imgSrc, bool bDetect, bool bCompute)
{
    CMat mask = isMaskAvailable(0) ? getGraphicsMask(0) : CMat();
    if(bDetect && bCompute)
        m_pFeatures->detectAndCompute(imgSrc, mask, m_keypoints, m_descriptors);
    else if(bDetect && !bCompute)
        m_pFeatures->detect(imgSrc, m_keypoints, mask);
    else
        m_pFeatures->compute(imgSrc, m_keypoints, m_descriptors);
}

void CFeatureImageProcess2d::manageOutputs()
{
    forwardInputImage(0, 0);

    int ind = 1;
    if(getOutputCount() > 3)
    {
        auto pOutput1 = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(ind));
        if(pOutput1)
            pOutput1->setImage(m_descriptors);
        ind++;
    }

    auto pOutput2 = std::dynamic_pointer_cast<CFeatureProcessIO<cv::KeyPoint>>(getOutput(ind));
    if(pOutput2)
    {
        pOutput2->clearData();
        pOutput2->addValueList(m_keypoints);
        ind++;
    }
    manageGraphicOutputs();
}

void CFeatureImageProcess2d::manageGraphicOutputs()
{
    auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(getOutputCount() - 1));
    if(pOutput == nullptr)
        throw CException(CoreExCode::NULL_POINTER, "Invalid graphics output", __func__, __FILE__, __LINE__);

    pOutput->setNewLayer(getName());
    pOutput->setImageIndex(0);

    for(size_t i=0; i<m_keypoints.size(); ++i)
    {
        auto keyPt = m_keypoints[i];
        pOutput->addEllipse(keyPt.pt.x, keyPt.pt.y, 10, 10);
    }
}
