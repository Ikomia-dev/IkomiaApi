#ifndef COCVWIDGETEXP_HPP
#define COCVWIDGETEXP_HPP

#include "Process/OpenCV/core/COcvExp.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetExp : public CWidgetApplyOnly
{
    public:

        COcvWidgetExp(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetExp(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetExpFactory : public CWidgetFactory
{
    public:

        COcvWidgetExpFactory()
        {
            m_name = QObject::tr("Exponent").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetExp>(pParam);
        }
};

#endif // COCVWIDGETEXP_HPP
