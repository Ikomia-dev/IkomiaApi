#ifndef CBLOBMEASURE_HPP
#define CBLOBMEASURE_HPP

#include "Core/CProcessFactory.hpp"
#include "Core/CImageProcess2d.h"
#include "IO/CVideoProcessIO.h"
#include "IO/CMeasureProcessIO.h"

//-----------------------------//
//----- CBlobMeasureParam -----//
//-----------------------------//
class CBlobMeasureParam: public CProtocolTaskParam
{
    public:

        CBlobMeasureParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        std::vector<CMeasure>   m_measures;
};

//------------------------//
//----- CBlobMeasure -----//
//------------------------//
class CBlobMeasure : public CImageProcess2d
{
    public:

        CBlobMeasure();
        CBlobMeasure(const std::string name, const std::shared_ptr<CBlobMeasureParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;

        void    updateStaticOutputs() override;

    private:

        void            compute(CMat imgSrc);
        void            computeFromLabel(CMat imgSrc);
        void            computeGraphics(const CColor &penColor, const CColor &brushColor, const int lineSize);
        void            computeMeasures(const std::string &label);
        ObjectMeasures  computeBlobMeasures(const std::vector<cv::Point>& blob, const std::vector<std::vector<cv::Point>>& holes, size_t graphicsId, const std::string &label);
        void            computeCustomMeasure(ObjectMeasures& blobMeasures, const std::string& formula, size_t graphicsId);

    private:

        std::vector<cv::Vec4i>              m_hierarchy;
        std::vector<std::vector<cv::Point>> m_blobs;
};

class CBlobMeasureFactory : public CProcessFactory
{
    public:

        CBlobMeasureFactory()
        {
            m_info.m_name = QObject::tr("Objects measurements").toStdString();
            m_info.m_description = QObject::tr("This process extract connected components from binary image and calculate selected measures for each.").toStdString();
            m_info.m_path = QObject::tr("Core/Measurements").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob, quantification, measure, connected component";
        }
        ~CBlobMeasureFactory() {}

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CBlobMeasureParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CBlobMeasure>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CBlobMeasureParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CBlobMeasure>(m_info.m_name, pDerivedParam);
        } 
};

#endif // CBLOBMEASURE_HPP
