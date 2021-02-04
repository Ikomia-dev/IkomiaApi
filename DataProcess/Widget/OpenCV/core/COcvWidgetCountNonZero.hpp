#ifndef COCVWIDGETCOUNTNONZERO_HPP
#define COCVWIDGETCOUNTNONZERO_HPP

#include "Process/OpenCV/core/COcvPSNR.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetCountNonZero : public CWidgetApplyOnly
{
    public:

        COcvWidgetCountNonZero(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }
        COcvWidgetCountNonZero(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetCountNonZeroFactory : public CWidgetFactory
{
    public:

        COcvWidgetCountNonZeroFactory()
        {
            m_name = QObject::tr("CountNonZero").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCountNonZero>(pParam);
        }
};

#endif // COCVWIDGETCOUNTNONZERO_HPP
