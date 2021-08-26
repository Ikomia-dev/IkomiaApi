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

#include "C2dFeatureImageTask.h"

C2dFeatureImageTask::C2dFeatureImageTask() : C2dImageTask()
{
}

C2dFeatureImageTask::C2dFeatureImageTask(const std::string& name) : C2dImageTask(name)
{
}

C2dFeatureImageTask::~C2dFeatureImageTask()
{

}

void C2dFeatureImageTask::addKeypointInput()
{
    addInput(std::make_shared<CNumericIO<cv::KeyPoint>>());
}

void C2dFeatureImageTask::addDescriptorAndKeypointOuputs()
{
    addOutput(std::make_shared<CImageIO>(IODataType::DESCRIPTORS));
    addOutput(std::make_shared<CNumericIO<cv::KeyPoint>>());
    addOutput(std::make_shared<CGraphicsOutput>());
}

void C2dFeatureImageTask::addKeypointOutput()
{
    addOutput(std::make_shared<CNumericIO<cv::KeyPoint>>());
    addOutput(std::make_shared<CGraphicsOutput>());
}

void C2dFeatureImageTask::manageInputs()
{
    if(getInputCount()<3)
        return;

    auto pInput = std::dynamic_pointer_cast<CNumericIO<cv::KeyPoint>>(getInput(2));
    if(pInput)
        if(pInput->getAllValues().size()>0)
            m_keypoints = pInput->getValueList(0);
}

void C2dFeatureImageTask::makeFeatures(const CMat& imgSrc, bool bDetect, bool bCompute)
{
    CMat mask = isMaskAvailable(0) ? getGraphicsMask(0) : CMat();
    if(bDetect && bCompute)
        m_pFeatures->detectAndCompute(imgSrc, mask, m_keypoints, m_descriptors);
    else if(bDetect && !bCompute)
        m_pFeatures->detect(imgSrc, m_keypoints, mask);
    else
        m_pFeatures->compute(imgSrc, m_keypoints, m_descriptors);
}

void C2dFeatureImageTask::manageOutputs()
{
    forwardInputImage(0, 0);

    int ind = 1;
    if(getOutputCount() > 3)
    {
        auto pOutput1 = std::dynamic_pointer_cast<CImageIO>(getOutput(ind));
        if(pOutput1)
            pOutput1->setImage(m_descriptors);
        ind++;
    }

    auto pOutput2 = std::dynamic_pointer_cast<CNumericIO<cv::KeyPoint>>(getOutput(ind));
    if(pOutput2)
    {
        pOutput2->clearData();
        pOutput2->addValueList(m_keypoints);
        ind++;
    }
    manageGraphicOutputs();
}

void C2dFeatureImageTask::manageGraphicOutputs()
{
    auto pOutput = std::dynamic_pointer_cast<CGraphicsOutput>(getOutput(getOutputCount() - 1));
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
