#ifndef COCVWIDGETABSDIFF_HPP
#define COCVWIDGETABSDIFF_HPP
#include "Process/OpenCV/core/COcvAbsdiff.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetAbsdiff : public CWidgetApplyOnly
{
    public:

        COcvWidgetAbsdiff(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetAbsdiff(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetAbsdiffFactory : public CWidgetFactory
{
    public:

        COcvWidgetAbsdiffFactory()
        {
            m_name = QObject::tr("Abs diff").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAbsdiff>(pParam);
        }
};
#endif // COCVWIDGETABSDIFF_HPP
