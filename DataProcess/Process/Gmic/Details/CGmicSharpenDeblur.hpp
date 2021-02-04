#ifndef CGMICDEBLUR_HPP
#define CGMICDEBLUR_HPP

#include "Core/CImageProcess2d.h"
#include "Process/Gmic/CGmicTaskParam.hpp"
#include "Process/Gmic/CGmicImageConverter.h"

//----------------------------//
//----- CGmicDeblurParam -----//
//----------------------------//
class CGmicSharpenDeblurParam: public CGmicTaskParam
{
    public:

        enum Regularization { TIKHONOV, MEAN_CURVATURE, TV };
        enum ParallelMode { AUTO, ONE_THREAD, TWO_THREADS, FOUR_THREADS, EIGHT_THREADS, SIXTEEN_THREADS };

        CGmicSharpenDeblurParam() : CGmicTaskParam(){}

        void        setParamMap(const UMapString& paramMap) override
        {
            m_radius = std::stod(paramMap.at("radius"));
            m_iteration = std::stoi(paramMap.at("iteration"));
            m_timeStep = std::stod(paramMap.at("timeStep"));
            m_smoothness = std::stod(paramMap.at("smoothness"));
            m_regularization = std::stoi(paramMap.at("regularization"));
            m_channel = std::stoi(paramMap.at("channel"));
        }

        UMapString  getParamMap() const override
        {
            UMapString map;
            map.insert(std::make_pair("radius", std::to_string(m_radius)));
            map.insert(std::make_pair("iteration", std::to_string(m_iteration)));
            map.insert(std::make_pair("timeStep", std::to_string(m_timeStep)));
            map.insert(std::make_pair("smoothness", std::to_string(m_smoothness)));
            map.insert(std::make_pair("regularization", std::to_string(m_regularization)));
            map.insert(std::make_pair("channel", std::to_string(m_channel)));
            return map;
        }

        std::string getParamString() const override
        {
            //Build string of parameters, take care of order.
            return std::to_string(m_radius) + "," + std::to_string(m_iteration) + "," + std::to_string(m_timeStep) + "," +
                    std::to_string(m_smoothness) + "," + std::to_string(m_regularization) + "," + std::to_string(m_channel) + "," +
                    std::to_string(m_parallelMode) + "," + std::to_string(m_spatialOverlap);
        }

    public:

        const std::vector<std::string> m_channels =
        {
            "All","RGBA [All]","RGB [All]","RGB [Red]","RGB [Green]","RGB [Blue]","RGBA [Alpha]",
            "Linear RGB [All]","Linear RGB [Red]","Linear RGB [Green]","Linear RGB [Blue]","YCbCr [Luminance]",
            "YCbCr [Blue-Red Chrominances]","YCbCr [Blue Chrominance]","YCbCr [Red Chrominance]",
            "YCbCr [Green Chrominance]","Lab [Lightness]","Lab [ab-Chrominances]","Lab [a-Chrominance]",
            "Lab [b-Chrominance]","Lch [ch-Chrominances]","Lch [c-Chrominance]","Lch [h-Chrominance]","HSV [Hue]",
            "HSV [Saturation]","HSV [Value]","HSI [Intensity]","HSL [Lightness]","CMYK [Cyan]","CMYK [Magenta]",
            "CMYK [Yellow]","CMYK [Key]","YIQ [Luma]","YIQ [Chromas]","RYB [All]","RYB [Red]","RYB [Yellow]","RYB [Blue]"
        };

        float   m_radius = 2;
        int     m_iteration = 10;
        float   m_timeStep = 20;
        float   m_smoothness = 0.1;
        int     m_regularization = MEAN_CURVATURE;
        int     m_channel = 11;
        int     m_parallelMode = AUTO;
        int     m_spatialOverlap = 24;
};

//-----------------------//
//----- CGmicDeblur -----//
//-----------------------//
class CGmicSharpenDeblur : public CImageProcess2d
{
    public:

        CGmicSharpenDeblur() : CImageProcess2d()
        {
        }
        CGmicSharpenDeblur(const std::string name, const std::shared_ptr<CGmicSharpenDeblurParam>& pParam) : CImageProcess2d(name)
        {
            m_pParam = std::make_shared<CGmicSharpenDeblurParam>(*pParam);
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
            auto pParam = std::dynamic_pointer_cast<CGmicSharpenDeblurParam>(m_pParam);

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
                std::string command = "fx_deblur[0] " + pParam->getParamString();
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

class CGmicSharpenDeblurFactory : public CProcessFactory
{
    public:

        CGmicSharpenDeblurFactory()
        {
            m_info.m_name = QObject::tr("Sharpen [Deblur]").toStdString();
            m_info.m_description = QObject::tr("This process performs deblurring on images.").toStdString();
            m_info.m_path = QObject::tr("Gmic/Details").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/gmic.png").toStdString();
            m_info.m_keywords = "enhance,contrast,deconvolution";
            m_info.m_authors = "David Tschumperlé";
            m_info.m_year = 2010;
        }

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGmicSharpenDeblurParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGmicSharpenDeblur>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pParam = std::make_shared<CGmicSharpenDeblurParam>();
            assert(pParam != nullptr);
            return std::make_shared<CGmicSharpenDeblur>(m_info.m_name, pParam);
        }
};

#endif // CGMICDEBLUR_HPP
