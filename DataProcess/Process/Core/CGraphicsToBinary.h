#ifndef CGRAPHICSTOBINARY_H
#define CGRAPHICSTOBINARY_H

#include "Core/CProcessFactory.hpp"
#include "Core/CImageProcess2d.h"

//----------------------------------//
//----- CGraphicsToBinaryParam -----//
//----------------------------------//
class CGraphicsToBinaryParam: public CProtocolTaskParam
{
    public:

        CGraphicsToBinaryParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        bool    m_bFitToContent = true;
        int     m_width = 1024;
        int     m_height = 1024;
};

//-----------------------------//
//----- CGraphicsToBinary -----//
//-----------------------------//
class CGraphicsToBinary : public CImageProcess2d
{
    public:

        CGraphicsToBinary();
        CGraphicsToBinary(const std::string name, const std::shared_ptr<CGraphicsToBinaryParam>& pParam);

        size_t  getProgressSteps() override;

        void    run() override;
};

class CGraphicsToBinaryFactory : public CProcessFactory
{
    public:

        CGraphicsToBinaryFactory()
        {
            m_info.m_name = QObject::tr("Graphics to binary").toStdString();
            m_info.m_description = QObject::tr("This process generates binary mask from graphics items.").toStdString();
            m_info.m_path = QObject::tr("Core/Utils").toStdString();
            m_info.m_iconPath = QObject::tr(":/Images/default-process.png").toStdString();
            m_info.m_keywords = "Blob,graphics,mask,binary,connected,component";
        }
        ~CGraphicsToBinaryFactory() {}

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override
        {
            auto pDerivedParam = std::dynamic_pointer_cast<CGraphicsToBinaryParam>(pParam);
            if(pDerivedParam != nullptr)
                return std::make_shared<CGraphicsToBinary>(m_info.m_name, pDerivedParam);
            else
                return create();
        }
        virtual ProtocolTaskPtr create() override
        {
            auto pDerivedParam = std::make_shared<CGraphicsToBinaryParam>();
            assert(pDerivedParam != nullptr);
            return std::make_shared<CGraphicsToBinary>(m_info.m_name, pDerivedParam);
        }
};

#endif // CGRAPHICSTOBINARY_H
