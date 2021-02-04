#ifndef COCVWIDGETWATERSHED_HPP
#define COCVWIDGETWATERSHED_HPP

#include "Process/OpenCV/imgproc/COcvWatershed.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetWatershed : public CWidgetApplyOnly
{
    public:

        COcvWidgetWatershed(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetWatershed(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetWatershedFactory : public CWidgetFactory
{
    public:

        COcvWidgetWatershedFactory()
        {
            m_name = QObject::tr("Watershed").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetWatershed>(pParam);
        }
};

#endif // COCVWIDGETWATERSHED_HPP
