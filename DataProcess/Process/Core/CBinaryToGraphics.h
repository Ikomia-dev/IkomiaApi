#ifndef CBINARYTOGRAPHICS_H
#define CBINARYTOGRAPHICS_H

#include "Core/CProcessFactory.hpp"
#include "Core/CImageProcess2d.h"

//----------------------------------//
//----- CBinaryToGraphicsParam -----//
//----------------------------------//
class CBinaryToGraphicsParam: public CProtocolTaskParam
{
    public:

        CBinaryToGraphicsParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;
};

//-----------------------------//
//----- CBinaryToGraphics -----//
//-----------------------------//
class CBinaryToGraphics : public CImageProcess2d
{
    public:

        CBinaryToGraphics();
        CBinaryToGraphics(const std::string name, const std::shared_ptr<CBinaryToGraphicsParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;

    private:

        void    computeBinary(const CMat& src);
        void    computeLabel(const CMat& src);
        void    computeGraphics(const CColor &penColor, const CColor &brushColor, const int lineSize);

    private:

        std::vector<cv::Vec4i>              m_hierarchy;
        std::vector<std::vector<cv::Point>> m_blobs;
};

class CBinaryToGraphicsFactory : public CProcessFactory
{
    public:

        CBinaryToGraphicsFactory()
        {
            m_info.m_name = QObject::tr("Binary to graphics").toStdString();
            m_info.m_description = QObject::tr("This process extract connected components from binary image and generates the corresponding graphics items.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob,graphics,connected,component";
        }
        ~CBinaryToGraphicsFactory() {}

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CBinaryToGraphicsParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CBinaryToGraphics>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CBinaryToGraphicsParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CBinaryToGraphics>(m_info.m_name, pDerivedParam);
        }
};

#endif // CBINARYTOGRAPHICS_H
