#ifndef COCVWIDGETNEGATIVE_HPP
#define COCVWIDGETNEGATIVE_HPP

#include "Process/OpenCV/core/COcvNegative.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetNegative : public CWidgetApplyOnly
{
    public:

        COcvWidgetNegative(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetNegative(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetNegativeFactory : public CWidgetFactory
{
    public:

        COcvWidgetNegativeFactory()
        {
            m_name = QObject::tr("Negative").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetNegative>(pParam);
        }
};

#endif // COCVWIDGETNEGATIVE_HPP
