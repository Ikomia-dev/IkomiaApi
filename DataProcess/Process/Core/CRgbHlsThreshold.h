#ifndef CRGBHLSTHRESHOLD_H
#define CRGBHLSTHRESHOLD_H

#include "Core/CInteractiveImageProcess2d.h"

//---------------------------------//
//----- CRgbHlsThresholdParam -----//
//---------------------------------//
class CRgbHlsThresholdParam: public CProtocolTaskParam
{
    public:

        enum ActionFlags
        {
            CLEAR_SEGMENTATION = 0x00000001
        };

        CRgbHlsThresholdParam();

        void        setParamMap(const UMapString& paramMap) override;

        UMapString  getParamMap() const override;

    public:

        int m_minR = 0;
        int m_maxR = 0;
        int m_minG = 0;
        int m_maxG = 0;
        int m_minB = 0;
        int m_maxB = 0;
        int m_minH = 0;
        int m_maxH = 0;
        int m_minL = 0;
        int m_maxL = 0;
        int m_minS = 0;
        int m_maxS = 0;
};

//----------------------------//
//----- CRgbHlsThreshold -----//
//----------------------------//
class CRgbHlsThreshold : public CInteractiveImageProcess2d
{
    public:

        CRgbHlsThreshold();
        CRgbHlsThreshold(const std::string name, const std::shared_ptr<CRgbHlsThresholdParam>& pParam);

        size_t  getProgressSteps() override;

        void    graphicsChanged() override;

        void    run() override;

        void    executeActions(int flags) override;

    private:

        void    updateThresholds();
};

//-----------------------------------//
//----- CRgbHlsThresholdFactory -----//
//-----------------------------------//
class CRgbHlsThresholdFactory : public CProcessFactory
{
    public:

        CRgbHlsThresholdFactory();
        ~CRgbHlsThresholdFactory() {}

        virtual ProtocolTaskPtr create(const ProtocolTaskParamPtr& pParam) override;
        virtual ProtocolTaskPtr create() override;
};

#endif // CRGBHLSTHRESHOLD_H
