#ifndef CGMICHOTPIXELS_HPP
#define CGMICHOTPIXELS_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//-------------------------------//
//----- CGmicHotPixelsParam -----//
//-------------------------------//
class CGmicHotPixelsParam: public CGmicTaskParam
{
    public:

        CGmicHotPixelsParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_maskSize = std::stoi(paramMap.at("maskSize"));
            m_threshold = std::stoi(paramMap.at("threshold"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("maskSize", std::to_string(m_maskSize)));
            map.insert(std::make_pair("threshold", std::to_string(m_threshold)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_maskSize) + "," + std::to_string(m_threshold);
        }

    public:

        int     m_maskSize = 3;
        int     m_threshold = 10;
};

//--------------------------//
//----- CGmicHotPixels -----//
//--------------------------//
class CGmicHotPixels : public CImageProcess2d
{
    public:

        CGmicHotPixels() : CImageProcess2d()
        {
        }
        CGmicHotPixels(const std::string name, const std::shared_ptr<CGmicHotPixelsParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicHotPixelsParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicHotPixelsParam>(m_pParam);

            if(pInput == nullptr || pParam == nullptr)
                throw CException(CoreExCode::INVALID_PARAMETER, "Invalid input", __func__, __FILE__, __LINE__);

            if(pInput->isDataAvailable() == false)
                throw CException(CoreExCode::INVALID_PARAMETER, "Empty image", __func__, __FILE__, __LINE__);

            CMat imgSrc = pInput->getImage();
            CMat imgDst(imgSrc.rows, imgSrc.cols, imgSrc.type());
            createGraphicsMask(imgSrc.getNbCols(), imgSrc.getNbRows(), pGraphicsInput);
            emit m_signalHandler->doProgress();

            try
            {
                //Put image inputs into a gmic_list object
                gmic_list<float> imageList;
                gmic_list<char> image_names;

                //Allocate list, parameter is the number of image inputs
                imageList.assign(1);
                //Conversion from CMat(cv::Mat) to gmic_image(CImg)
                CGmicImageConverter::convert(imgSrc, imageList[0]);
                //Build command for gmic interpreter
                std::string command = "remove_hotpixels[0] " + pParam->getParamString();
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

            applyGraphicsMask(imgSrc, imgDst, 0);

            auto pOutput = std::dynamic_pointer_cast<CImageProcessIO>(getOutput(0));
            if(pOutput)
                pOutput->setImage(imgDst);

            emit m_signalHandler->doProgress();
        }
};

class CGmicHotPixelsFactory : public CProcessFactory
{
    public:

        CGmicHotPixelsFactory()
        {
            m_info.m_name = QObject::tr("Hot pixels").toStdString();
            m_info.m_description = QObject::tr("This process automatically removes hot pixels in images.").toStdString();
            m_info.m_docLink = "https://gmic.eu/reference.shtml#remove_hotpixels";
            m_info.m_path = QObject::tr("Gmic/Repair").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "noise,single,point";
            m_info.m_authors = "Jérôme Boulanger";
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicHotPixelsParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicHotPixels>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicHotPixelsParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicHotPixels>(m_info.m_name, pParam);
        }
};

#endif // CGMICHOTPIXELS_HPP
