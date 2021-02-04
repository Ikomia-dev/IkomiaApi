#ifndef COCVWIDGETMEANSHIFT_HPP
#define COCVWIDGETMEANSHIFT_HPP

#include "Process/OpenCV/video/COcvMeanShift.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetMeanShift : public CWidgetApplyOnly
{
    public:

        COcvWidgetMeanShift(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetMeanShift(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMeanShiftParam>(pParam);
        }

    private:

        std::shared_ptr<COcvMeanShiftParam> m_pParam = nullptr;
};

class COcvWidgetMeanShiftFactory : public CWidgetFactory
{
    public:

        COcvWidgetMeanShiftFactory()
        {
            m_name = QObject::tr("MeanShift").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMeanShift>(pParam);
        }
};

#endif // COCVWIDGETMEANSHIFT_HPP
