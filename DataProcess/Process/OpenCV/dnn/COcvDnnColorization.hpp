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

#ifndef COCVDNNCOLORIZATION_HPP
#define COCVDNNCOLORIZATION_HPP

#include "COcvDnnProcess.h"

//For colorization network
// -> the 313 ab cluster centers from pts_in_hull.npy (already transposed)
static float hullPts[] = {
    -90., -90., -90., -90., -90., -80., -80., -80., -80., -80., -80., -80., -80., -70., -70., -70., -70., -70., -70., -70., -70.,
    -70., -70., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -50., -50., -50., -50., -50., -50., -50., -50.,
    -50., -50., -50., -50., -50., -50., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -30.,
    -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -20., -20., -20., -20., -20., -20., -20.,
    -20., -20., -20., -20., -20., -20., -20., -20., -20., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10.,
    -10., -10., -10., -10., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 10., 10., 10., 10., 10., 10., 10.,
    10., 10., 10., 10., 10., 10., 10., 10., 10., 10., 10., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20.,
    20., 20., 20., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 40., 40., 40., 40.,
    40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 50., 50., 50., 50., 50., 50., 50., 50., 50., 50.,
    50., 50., 50., 50., 50., 50., 50., 50., 50., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60.,
    60., 60., 60., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 80., 80., 80.,
    80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 90., 90., 90., 90., 90., 90., 90., 90., 90., 90.,
    90., 90., 90., 90., 90., 90., 90., 90., 90., 100., 100., 100., 100., 100., 100., 100., 100., 100., 100., 50., 60., 70., 80., 90.,
    20., 30., 40., 50., 60., 70., 80., 90., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -20., -10., 0., 10., 20., 30., 40., 50.,
    60., 70., 80., 90., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -40., -30., -20., -10., 0., 10., 20.,
    30., 40., 50., 60., 70., 80., 90., 100., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -50.,
    -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -60., -50., -40., -30., -20., -10., 0., 10., 20.,
    30., 40., 50., 60., 70., 80., 90., 100., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90.,
    100., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -80., -70., -60., -50.,
    -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -90., -80., -70., -60., -50., -40., -30., -20., -10.,
    0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30.,
    40., 50., 60., 70., 80., 90., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70.,
    80., -110., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., -110., -100.,
    -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., -110., -100., -90., -80., -70.,
    -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., -110., -100., -90., -80., -70., -60., -50., -40., -30.,
    -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0.
};

//-------------------------------//
//----- COcvDnnColorization -----//
//-------------------------------//
class COcvDnnColorization: public COcvDnnProcess
{
    public:

        COcvDnnColorization() : COcvDnnProcess()
        {
        }
        COcvDnnColorization(const std::string& name, const std::shared_ptr<COcvDnnProcessParam>& pParam) : COcvDnnProcess(name)
        {
            m_pParam = std::make_shared<COcvDnnProcessParam>(*pParam);
        }

        size_t      getProgressSteps() override
        {
            return 3;
        }
        int         getNetworkInputSize() const override
        {
            return 224;
        }
        cv::Scalar  getNetworkInputMean() const override
        {
            cv::Scalar mean = 50;
            return mean;
        }

        void        run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgOrigin = pInput->getImage();
            cv::Mat dnnOutput;
            CMat imgSrc;

            if(imgOrigin.channels() == 1)
                cv::cvtColor(imgOrigin, imgSrc, cv::COLOR_GRAY2RGB);
            else
                imgSrc = imgOrigin;

            //Convert to LAB
            CMat imgLab;
            imgSrc.convertTo(imgLab, CV_32F, 1.0/255.0);
            cv::cvtColor(imgLab, imgLab, cv::COLOR_RGB2Lab);
            cv::extractChannel(imgLab, m_imgL, 0);
            imgSrc = m_imgL;
            emit m_signalHandler->doProgress();

            try
            {
                if(m_net.empty() || pParam->m_bUpdate)
                {
                    m_net = readDnn();
                    if(m_net.empty())
                        throw CException(CoreExCode::INVALID_PARAMETER, "Failed to load network", __func__, __FILE__, __LINE__);

                    pParam->m_bUpdate = false;
                }
                int size = getNetworkInputSize();
                double scaleFactor = getNetworkInputScaleFactor();
                cv::Scalar mean = getNetworkInputMean();
                setupColorizationLayer();

                auto inputBlob = cv::dnn::blobFromImage(imgSrc, scaleFactor, cv::Size(size,size), mean, false, false);
                m_net.setInput(inputBlob);
                dnnOutput = m_net.forward(m_outputLayerName);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            readClassNames();
            endTaskRun();
            emit m_signalHandler->doProgress();
            manageOutput(dnnOutput);
            emit m_signalHandler->doProgress();
        }

        void        setupColorizationLayer()
        {
            int sz[] = {2, 313, 1, 1};
            const cv::Mat ptsInHull(4, sz, CV_32F, hullPts);
            cv::Ptr<cv::dnn::Layer> class8_ab = m_net.getLayer("class8_ab");
            class8_ab->blobs.push_back(ptsInHull);
            cv::Ptr<cv::dnn::Layer> conv8_313_rh = m_net.getLayer("conv8_313_rh");
            conv8_313_rh->blobs.push_back(cv::Mat(1, 313, CV_32F, cv::Scalar(2.606)));
            m_outputLayerName = "class8_ab";
        }

        void        manageOutput(cv::Mat &dnnOutput)
        {
            forwardInputImage();

            //Retrieve the calculated a,b channels from the network output
            cv::Size size(dnnOutput.size[2], dnnOutput.size[3]);
            cv::Mat a = cv::Mat(size, CV_32F, dnnOutput.ptr(0, 0));
            cv::Mat b = cv::Mat(size, CV_32F, dnnOutput.ptr(0, 1));
            cv::resize(a, a, m_imgL.size());
            cv::resize(b, b, m_imgL.size());

            //Merge and convert back to BGR
            CMat imgOut;
            cv::Mat imgLab;
            cv::Mat channels[] = {m_imgL, a, b};
            cv::merge(channels, 3, imgLab);
            cv::cvtColor(imgLab, imgOut, cv::COLOR_Lab2RGB);

            auto pImgOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            pImgOutput->setImage(imgOut);
        }

    private:

        cv::Mat m_imgL;
};

//--------------------------------------//
//----- COcvDnnColorizationFactory -----//
//--------------------------------------//
class COcvDnnColorizationFactory : public CProcessFactory
{
    public:

        COcvDnnColorizationFactory()
        {
            m_info.m_name = QObject::tr("DNN colorization").toStdString();
            m_info.m_description = QObject::tr("This process gives the possibility to launch inference from already trained networks for colorization purpose (CAFFE, TENSORFLOW, DARKNET and TORCH)).").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Deep neural network").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "cnn,dnn,deep,neural,network,colorization";
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/d6/d0f/group__dnn.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<COcvDnnProcessParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<COcvDnnColorization>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<COcvDnnProcessParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<COcvDnnColorization>(m_info.m_name, pDerivedParam);
        }
};

#endif // COCVDNNCOLORIZATION_HPP
