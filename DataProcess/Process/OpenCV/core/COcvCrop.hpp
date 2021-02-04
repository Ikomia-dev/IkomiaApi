#ifndef COCVCROP_HPP
#define COCVCROP_HPP

#include "Core/CImageProcess2d.h"
#include "IO/CImageProcessIO.h"
#include "Graphics/CGraphicsRectangle.h"

//-------------------------//
//----- COcvCropParam -----//
//-------------------------//
class COcvCropParam: public CProtocolTaskParam
{
    public:

        COcvCropParam() : CProtocolTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_cropRect.x = std::stoi(paramMap.at("left"));
            m_cropRect.y = std::stoi(paramMap.at("top"));
            m_cropRect.width = std::stoi(paramMap.at("width"));
            m_cropRect.height = std::stoi(paramMap.at("height"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("left", std::to_string(m_cropRect.x)));
            map.insert(std::make_pair("top", std::to_string(m_cropRect.y)));
            map.insert(std::make_pair("width", std::to_string(m_cropRect.width)));
            map.insert(std::make_pair("height", std::to_string(m_cropRect.height)));
            return map;
        }

    public:

        cv::Rect    m_cropRect;
};

//--------------------//
//----- COcvCrop -----//
//--------------------//
class COcvCrop : public CImageProcess2d
{
    public:

        COcvCrop() : CImageProcess2d()
        {
        }
        COcvCrop(const std::string name, const std::shared_ptr<COcvCropParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<COcvCropParam>(*pParam);
        }

        size_t  getProgressSteps() override
        {
            return 3;
        }

        void run() override
        {
            beginTaskRun();
            auto pInput = std::dynamic_pointer_cast<CImageProcessIO>(getInput(0));
            auto pGraphicsInput = std::dynamic_pointer_cast<CGraphicsProcessInput>(getInput(1));
            auto pParam = std::dynamic_pointer_cast<COcvCropParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid parameters", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgDst;
            emit m_signalHandler->doProgress();

            try
            {
                cv::Rect cropRoi;
                auto imgSrc = pInput->getImage();

                if(pGraphicsInput->isDataAvailable() == true)
                {
                    cropRoi = getGraphicsRoi(pGraphicsInput);
                    if(cropRoi.empty() == true)
                        cropRoi = cv::Rect(0, 0, imgSrc.cols, imgSrc.rows);
                }
                else if(pParam->m_cropRect.width != 0 && pParam->m_cropRect.height != 0)
                    cropRoi = pParam->m_cropRect;
                else
                    cropRoi = cv::Rect(0, 0, imgSrc.cols, imgSrc.rows);

                CMat cropImg = CMat(imgSrc, cropRoi);
                imgDst = cropImg.clone();
            }
            catch(cv::Exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e, __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }

    private:

        cv::Rect    getGraphicsRoi(const GraphicsProcessInputPtr& pInput)
        {
            auto items = pInput->getItems();
            for(size_t i=0; i<items.size(); ++i)
            {
                auto pItem = std::dynamic_pointer_cast<CProxyGraphicsRect>(items[i]);
                if(pItem)
                    return cv::Rect(pItem->m_x, pItem->m_y, pItem->m_width, pItem->m_height);
            }
            return cv::Rect();
        }
};

class COcvCropFactory : public CProcessFactory
{
    public:

        COcvCropFactory()
        {
            m_info.m_name = QObject::tr("Crop").toStdString();
            m_info.m_description = QObject::tr("This process crops your image.").toStdString();
            m_info.m_path = QObject::tr("OpenCV/Main modules/Core functionality/Operations on arrays").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/opencv.png").toStdString();
            m_info.m_keywords = "crop";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pCropParam = std::dynamic_pointer_cast<COcvCropParam>(pParam);
            if(pCropParam != nullptr)
                return std::make_shared<COcvCrop>(m_info.m_name, pCropParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pCropParam = std::make_shared<COcvCropParam>();
            assert(pCropParam != nullptr);
            return std::make_shared<COcvCrop>(m_info.m_name, pCropParam);
        }
};

#endif // COCVCROP_HPP
