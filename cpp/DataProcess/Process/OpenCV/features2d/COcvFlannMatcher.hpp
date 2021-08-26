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

#ifndef COCVFLANNMATCHER_HPP
#define COCVFLANNMATCHER_HPP

#include "Core/C2dFeatureMatcherTask.h"
#include "IO/CImageIO.h"

//--------------------------------//
//----- COcvFlannMatcherParam -----//
//--------------------------------//
class COcvFlannMatcherParam: public CWorkflowTaskParam
{
    public:

        COcvFlannMatcherParam() : CWorkflowTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap);
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }
};

//---------------------------//
//----- COcvFlannMatcher -----//
//---------------------------//
class COcvFlannMatcher : public C2dFeatureMatcherTask
{
    public:

        COcvFlannMatcher() : C2dFeatureMatcherTask()
        {

        }
        COcvFlannMatcher(const std::string name, const std::shared_ptr<COcvFlannMatcherParam>& pParam) : C2dFeatureMatcherTask(name)
        {
            m_pParam = std::make_shared<COcvFlannMatcherParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();

            if(getInputCount() < 3)
                throw CException(CoreExCode::INVALID_PARAMETER, "Not enough inputs", __func__, __FILE__, __LINE__);

            auto pImg1 = std::dynamic_pointer_cast<CImageIO>(getInput(0));
            auto pDesc1 = std::dynamic_pointer_cast<CImageIO>(getInput(1));
            auto pFeatures1 = std::dynamic_pointer_cast<CNumericIO<cv::KeyPoint>>(getInput(2));

            auto pImg2 = std::dynamic_pointer_cast<CImageIO>(getInput(3));
            auto pDesc2 = std::dynamic_pointer_cast<CImageIO>(getInput(4));
            auto pFeatures2 = std::dynamic_pointer_cast<CNumericIO<cv::KeyPoint>>(getInput(5));

            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsInput>(getInput(6));
            auto pParam = std::dynamic_pointer_cast<COcvFlannMatcherParam>(m_pParam);

            if(pDesc1 == nullptr || pImg1 == nullptr || pFeatures1 == nullptr ||
                    pDesc2 == nullptr || pImg2 == nullptr || pFeatures2 == nullptr ||
                    pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pDesc1->isDataAvailable() == false || pImg1->isDataAvailable() == false ||
                   pDesc2->isDataAvailable() == false || pImg2->isDataAvailable() == false )
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty images", __func__, __FILE__, __LINE__);

            emit m_signalHandler->doProgress();

            CMat descriptor1 = pDesc1->getImage();
            CMat descriptor2 = pDesc2->getImage();

            CMat img1 = pImg1->getImage();

            std::vector< cv::DMatch > matches;
            createGraphicsMask(img1.getNbCols(), img1.getNbRows(), pGraphicsInput);
            try
            {
                if(descriptor1.type()!=CV_32F) {
                    descriptor1.convertTo(descriptor1, CV_32F);
                }

                if(descriptor2.type()!=CV_32F) {
                    descriptor2.convertTo(descriptor2, CV_32F);
                }
                auto mask = getGraphicsMask(0);
                m_pMatcher = cv::FlannBasedMatcher::create();
                m_pMatcher->match(descriptor1, descriptor2, matches, mask);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();

            emit m_signalHandler->doProgress();

            CMat img2 = pImg2->getImage();

            auto key1 = pFeatures1->getValueList(0);
            auto key2 = pFeatures2->getValueList(0);

            manageOutputs(img1, key1, img2, key2, matches);

            emit m_signalHandler->doProgress();
        }
};

class COcvFlannMatcherFactory : public CTaskFactory
{
    public:

        COcvFlannMatcherFactory()
        {
            m_info.m_name = QObject::tr("Flann based matcher").toStdString();
            m_info.m_description = QObject::tr("Flann-based descriptor matcher.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/2D features framework/Descriptor matchers").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Features,Keypoints,Descriptors,Interest";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/dc/de2/classcv_1_1FlannBasedMatcher.html";
        }

        virtual WorkflowTaskPtr create(const WorkflowTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvFlannMatcherParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvFlannMatcher>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual WorkflowTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvFlannMatcherParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvFlannMatcher>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVFLANNMATCHER_HPP
