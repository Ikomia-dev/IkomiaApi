#ifndef COCVWIDGETFLANNMATCHER_HPP
#define COCVWIDGETFLANNMATCHER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvFlannMatcher.hpp"
#include <QDoubleSpinBox>
#include <QLabel>
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetFlannMatcher : public CWidgetApplyOnly
{
    public:

        COcvWidgetFlannMatcher(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetFlannMatcher(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<COcvFlannMatcherParam> m_pParam = nullptr;
};

class COcvWidgetFlannMatcherFactory : public CWidgetFactory
{
    public:

        COcvWidgetFlannMatcherFactory()
        {
            m_name = QObject::tr("Flann based matcher").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFlannMatcher>(pParam);
        }
};
#endif // COCVWIDGETFLANNMATCHER_HPP
