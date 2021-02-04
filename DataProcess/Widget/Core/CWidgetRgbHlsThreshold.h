#ifndef CWIDGETRGBHLSTHRESHOLD_H
#define CWIDGETRGBHLSTHRESHOLD_H

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CRgbHlsThreshold.h"

//----------------------------------//
//----- CWidgetRgbHlsThreshold -----//
//----------------------------------//
class CWidgetRgbHlsThreshold : public CProtocolTaskWidget
{
    public:

        CWidgetRgbHlsThreshold(QWidget *parent = Q_NULLPTR);
        CWidgetRgbHlsThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        virtual void    init() override;

    private:

        QPushButton*                            m_pClearBtn = nullptr;
        std::shared_ptr<CRgbHlsThresholdParam>  m_pParam = nullptr;
};

//-----------------------------------------//
//----- CWidgetRgbHlsThresholdFactory -----//
//-----------------------------------------//
class CWidgetRgbHlsThresholdFactory : public CWidgetFactory
{
    public:

        CWidgetRgbHlsThresholdFactory();
        ~CWidgetRgbHlsThresholdFactory() {}

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam);
};

#endif // CWIDGETRGBHLSTHRESHOLD_H
