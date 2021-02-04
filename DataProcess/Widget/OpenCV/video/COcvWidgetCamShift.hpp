#ifndef COCVWIDGETCAMSHIFT_HPP
#define COCVWIDGETCAMSHIFT_HPP

#include "Process/OpenCV/video/COcvCamShift.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetCamShift : public CWidgetApplyOnly
{
    public:

        COcvWidgetCamShift(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetCamShift(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCamShiftParam>(pParam);
        }

    private:

        std::shared_ptr<COcvCamShiftParam> m_pParam = nullptr;
};

class COcvWidgetCamShiftFactory : public CWidgetFactory
{
    public:

        COcvWidgetCamShiftFactory()
        {
            m_name = QObject::tr("CamShift").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCamShift>(pParam);
        }
};

#endif // COCVWIDGETCAMSHIFT_HPP
