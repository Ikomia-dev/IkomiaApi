#ifndef COCVWIDGETLOG_HPP
#define COCVWIDGETLOG_HPP

#include "Process/OpenCV/core/COcvLog.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetLog : public CWidgetApplyOnly
{
    public:

        COcvWidgetLog(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetLog(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetLogFactory : public CWidgetFactory
{
    public:

        COcvWidgetLogFactory()
        {
            m_name = QObject::tr("Log").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLog>(pParam);
        }
};

#endif // COCVWIDGETLOG_HPP
