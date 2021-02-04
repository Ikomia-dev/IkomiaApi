#ifndef COCVOBJECTNESSBING_HPP
#define COCVOBJECTNESSBING_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include <opencv2/saliency.hpp>

//------------------------------//
//----- COcvObjectnessBINGParam -----//
//------------------------------//
class COcvObjectnessBINGParam: public CProtocolTaskParam
{
    public:

        COcvObjectnessBINGParam() : CProtocolTaskParam(){}

        void setParamMap(const UMapString& paramMap) override
        {
            Q_UNUSED(paramMap)
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            return map;
        }

    public:

        int m_nbBBox = 7;
};

//-------------------------//
//----- COcvObjectnessBING -----//
//-------------------------//
class COcvObjectnessBING : public CImageProcess2d
{
    public:

        COcvObjectnessBING() : CImageProcess2d()
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
        }
        COcvObjectnessBING(const std::string name, const std::shared_ptr<COcvObjectnessBINGParam>& pParam) : CImageProcess2d(name)
        {
            addOutput(std::make_shared<CGraphicsProcessOutput>());
            m_pParam = std::make_shared<COcvObjectnessBINGParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void manageGraphicOuputs(const std::vector<cv::Vec4i>& saliencyMap)
        {
            auto pOutput = std::dynamic_pointer_cast<CGraphicsProcessOutput>(getOutput(getOutputCount() - 1));
            if(pOutput == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Invalid graphics output", __func__, __FILE__, __LINE__);

            auto pParam = std::dynamic_pointer_cast<COcvObjectnessBINGParam>(m_pParam);
            if(pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            pOutput->setNewLayer(getName());
            pOutput->setImageIndex(0);

            GraphicsRectProperty prop = m_graphicsContextPtr->getRectProperty();
            int ndet = int(saliencyMap.size());
            // The result are sorted by objectness. We only use the first maxd boxes here.
            int maxd = pParam->m_nbBBox, step = 255 / maxd;

            for (int i = 0; i < std::min(maxd, ndet); i++)
            {
                cv::Vec4i bb = saliencyMap[i];
                // Set different color for each bb
                prop.m_penColor = {((i*step)%255), 50, 255-((i*step)%255), 255};
                //Create rectangle graphics of bbox
                pOutput->addRectangle(bb[0], bb[1], bb[2]-bb[0], bb[3]-bb[1], prop);
            }
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pParam = std::dynamic_pointer_cast<COcvObjectnessBINGParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            std::vector<cv::Vec4i> saliencyMap;
            CMat imgSrc = pInput->getImage();
            emit m_signalHandler->doProgress();

            try
            {
                if(m_pSaliency.get() == nullptr)
                    m_pSaliency = cv::saliency::ObjectnessBING::create();

                std::string trainingPath = Utils::IkomiaApp::getAppFolder() + "/models/ObjectnessTrainedModel";
                m_pSaliency->setTrainingPath(trainingPath);
                m_pSaliency->setBBResDir( "Results" );

                if( m_pSaliency->computeSaliency( imgSrc, saliencyMap ) )
                    manageGraphicOuputs(saliencyMap);
                else
                    throw CException(CoreExCode::INVALID_PARAMETER, "Compute saliency has failed", __func__, __FILE__, __LINE__);
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            forwardInputImage();

            emit m_signalHandler->doProgress();
        }

    private:
        cv::Ptr<cv::saliency::ObjectnessBING> m_pSaliency;
        int m_width = 0;
        int m_height = 0;
};

class COcvObjectnessBINGFactory : public CProcessFactory
{
    public:

        COcvObjectnessBINGFactory()
        {
            m_info.m_name = QObject::tr("ObjectnessBING").toStdString();
            m_info.m_description = QObject::tr("This process finds all object window in a given image according to the BING method.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Extra modules/Saliency API").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "Saliency,Objectness,Features,Detection";
            m_info.m_authors = "Ming-Ming Cheng, Ziming Zhang, Wen-Yan Lin, and Philip Torr";
            m_info.m_article = "Bing: Binarized normed gradients for objectness estimation at 300fps";
            m_info.m_journal = "CVPR, IEEE";
            m_info.m_year = 2014;
            m_info.m_docLink = "https://docs.opencv.org/3.4.3/db/d63/classcv_1_1saliency_1_1ObjectnessBING.html";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pObjectnessBINGParam = std::dynamic_pointer_cast<COcvObjectnessBINGParam>(pParam);
            if(pObjectnessBINGParam != nullptr)
                return std::make_shared<COcvObjectnessBING>(m_info.m_name, pObjectnessBINGParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pObjectnessBINGParam = std::make_shared<COcvObjectnessBINGParam>();
            assert(pObjectnessBINGParam != nullptr);
            return std::make_shared<COcvObjectnessBING>(m_info.m_name, pObjectnessBINGParam);
        }
};

#endif // COCVOBJECTNESSBING_HPP
