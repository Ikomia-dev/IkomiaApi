#ifndef CGMICINPAINT_HPP
#define CGMICINPAINT_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//-----------------------------//
//----- CGmicInpaintParam -----//
//-----------------------------//
class CGmicInpaintParam: public CGmicTaskParam
{
    public:

        CGmicInpaintParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_patchSize = std::stoi(paramMap.at("pathSize"));
            m_lookupSize = std::stoi(paramMap.at("lookupSize"));
            m_lookupFactor = std::stod(paramMap.at("lookupFactor"));
            m_lookupInc = std::stoi(paramMap.at("lookupInc"));
            m_blendSize = std::stoi(paramMap.at("blendSize"));
            m_blendThreshold = std::stoi(paramMap.at("blendThreshold"));
            m_blendDecay = std::stod(paramMap.at("blendDecay"));
            m_blendScales = std::stoi(paramMap.at("blendScales"));
            m_bBlendOuter = std::stoi(paramMap.at("isBlendOuter"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("pathSize", std::to_string(m_patchSize)));
            map.insert(std::make_pair("lookupSize", std::to_string(m_lookupSize)));
            map.insert(std::make_pair("lookupFactor", std::to_string(m_lookupFactor)));
            map.insert(std::make_pair("lookupInc", std::to_string(m_lookupInc)));
            map.insert(std::make_pair("blendSize", std::to_string(m_blendSize)));
            map.insert(std::make_pair("blendThreshold", std::to_string(m_blendThreshold)));
            map.insert(std::make_pair("blendDecay", std::to_string(m_blendDecay)));
            map.insert(std::make_pair("blendScales", std::to_string(m_blendScales)));
            map.insert(std::make_pair("isBlendOuter", std::to_string(m_bBlendOuter)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_patchSize) + "," + std::to_string(m_lookupSize) + "," +  std::to_string(m_lookupFactor) + "," +
                    std::to_string(m_lookupInc) + "," + std::to_string(m_blendSize) + "," +  std::to_string(m_blendThreshold) + "," +
                    std::to_string(m_blendDecay) + "," + std::to_string(m_blendScales) + "," +  std::to_string(m_bBlendOuter);
        }

    public:

        int     m_patchSize = 7;
        int     m_lookupSize = 22;
        double  m_lookupFactor = 0.5;
        int     m_lookupInc = 1;
        int     m_blendSize = 1;
        int     m_blendThreshold = 0;
        double  m_blendDecay = 0.05;
        int     m_blendScales = 10;
        bool    m_bBlendOuter = true;
};

//------------------------//
//----- CGmicInpaint -----//
//------------------------//
class CGmicInpaint : public CImageProcess2d
{
    public:

        CGmicInpaint() : CImageProcess2d()
        {
        }
        CGmicInpaint(const std::string name, const std::shared_ptr<CGmicInpaintParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicInpaintParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicInpaintParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            auto mask = getGraphicsMask(0);
            if(mask.data == nullptr)
                throw CException(CoreExCode::NULL_POINTER, "Empty region of interest", __func__, __FILE__, __LINE__);

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(2);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                CGmicImageConverter::convert(mask, imageList[1]);
                //Build command for gmic interpreter
                std::string command = "inpaint[0] [1]," + pParam->getParamString();
                //Call interpreter
                gmic(command.c_str(), imageList, image_names);
                //Conversion from gmic_image to CMat
                CGmicImageConverter::convert(imageList[0], imgDst);
                //Free memory
                imageList.assign(0);
            }
            catch(gmic_exception& e)
            {
                throw CException(CoreExCode::INVALID_PARAMETER, e.what(), __func__, __FILE__, __LINE__);
            }

            endTaskRun();
            emit m_signalHandler->doProgress();

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicInpaintFactory : public CProcessFactory
{
    public:

        CGmicInpaintFactory()
        {
            m_info.m_name = QObject::tr("Patch-based Inpaint").toStdString();
            m_info.m_description = QObject::tr("This process performs Gmic patch-based inpainting from region of interest drawn in original image."
                                               "Inpaint selected images by specified mask."
                                               "If no patch size (or 0) is specified, inpainting is done using a fast average or median algorithm."
                                               "Otherwise, it used a patch-based reconstruction method, that can be very time consuming.").toStdString();
            m_info.m_docLink = "https://gmic.eu/reference.shtml#inpaint";
            m_info.m_path = QObject::tr("Gmic/Repair").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "inpainting,photo,deletion";
            m_info.m_authors = "David Tschumperlé, Maxime Daisy";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicInpaintParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicInpaint>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicInpaintParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicInpaint>(m_info.m_name, pParam);
        }
};

#endif // CGMICINPAINT_HPP
