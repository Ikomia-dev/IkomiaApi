#include "CWidgetRgbHlsThreshold.h"

//----------------------------------//
//----- CWidgetRgbHlsThreshold -----//
//----------------------------------//
CWidgetRgbHlsThreshold::CWidgetRgbHlsThreshold(QWidget *parent) : CProtocolTaskWidget(parent)
{
    init();
}

CWidgetRgbHlsThreshold::CWidgetRgbHlsThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent) : CProtocolTaskWidget(parent)
{
    m_pParam = std::dynamic_pointer_cast<CRgbHlsThresholdParam>(pParam);
    init();
}

void CWidgetRgbHlsThreshold::init()
{
    if(m_pParam == nullptr)
        m_pParam = std::make_shared<CRgbHlsThresholdParam>();

    m_pClearBtn = new QPushButton(tr("Clear segmentation"));
    m_pClearBtn->setDefault(false);
    m_pApplyBtn->setFocus();

    connect(m_pApplyBtn, &QPushButton::clicked, [&]{ emit doApplyProcess(m_pParam); } );
    connect(m_pClearBtn, &QPushButton::clicked, [&]{ emit doSendProcessAction(CRgbHlsThresholdParam::ActionFlags::CLEAR_SEGMENTATION); });

    
    m_pLayout->addWidget(m_pClearBtn, 0, 0);
    
}

//-----------------------------------------//
//----- CWidgetRgbHlsThresholdFactory -----//
//-----------------------------------------//
CWidgetRgbHlsThresholdFactory::CWidgetRgbHlsThresholdFactory()
{
    m_name = QObject::tr("RGB-HLS Segmentation").toStdString();
}

ProtocolTaskWidgetPtr CWidgetRgbHlsThresholdFactory::create(std::shared_ptr<CProtocolTaskParam> pParam)
{
    return std::make_shared<CWidgetRgbHlsThreshold>(pParam);
}
