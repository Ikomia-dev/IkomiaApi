#ifndef COCVWIDGETMAGNITUDE_HPP
#define COCVWIDGETMAGNITUDE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMagnitude.hpp"
#include <QDoubleSpinBox>
#include <QLabel>
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetMagnitude : public CWidgetApplyOnly
{
    public:

        COcvWidgetMagnitude(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetMagnitude(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvMagnitudeParam> m_pParam = nullptr;
};

class COcvWidgetMagnitudeFactory : public CWidgetFactory
{
    public:

        COcvWidgetMagnitudeFactory()
        {
            m_name = QObject::tr("Magnitude").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMagnitude>(pParam);
        }
};
#endif // COCVWIDGETMAGNITUDE_HPP
