#ifndef COCVWIDGETPSNR_HPP
#define COCVWIDGETPSNR_HPP

#include "Process/OpenCV/core/COcvPSNR.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetPSNR : public CWidgetApplyOnly
{
    public:

        COcvWidgetPSNR(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetPSNR(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetPSNRFactory : public CWidgetFactory
{
    public:

        COcvWidgetPSNRFactory()
        {
            m_name = QObject::tr("PSNR").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPSNR>(pParam);
        }
};

#endif // COCVWIDGETPSNR_HPP
