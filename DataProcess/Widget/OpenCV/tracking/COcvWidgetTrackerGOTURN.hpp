#ifndef COCVWIDGETTRACKERGOTURN_HPP
#define COCVWIDGETTRACKERGOTURN_HPP
#include "Process/OpenCV/tracking/COcvTrackerGOTURN.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetTrackerGOTURN : public CWidgetApplyOnly
{
    public:

        COcvWidgetTrackerGOTURN(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetTrackerGOTURN(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvTrackerGOTURNParam> m_pParam = nullptr;
};

class COcvWidgetTrackerGOTURNFactory : public CWidgetFactory
{
    public:

        COcvWidgetTrackerGOTURNFactory()
        {
            m_name = QObject::tr("TrackerGOTURN").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTrackerGOTURN>(pParam);
        }
};
#endif // COCVWIDGETTRACKERGOTURN_HPP
