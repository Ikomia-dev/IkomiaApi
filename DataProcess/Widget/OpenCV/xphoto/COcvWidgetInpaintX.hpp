#ifndef COCVWIDGETINPAINTX_HPP
#define COCVWIDGETINPAINTX_HPP

#include "Process/OpenCV/xphoto/COcvInpaintX.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetInpaintX : public CWidgetApplyOnly
{
    public:

        COcvWidgetInpaintX(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetInpaintX(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetInpaintXFactory : public CWidgetFactory
{
    public:

        COcvWidgetInpaintXFactory()
        {
            m_name = QObject::tr("XPhoto Inpaint").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInpaintX>(pParam);
        }
};

#endif // COCVWIDGETINPAINTX_HPP
