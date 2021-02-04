#ifndef COCVWIDGETDECOLOR_HPP
#define COCVWIDGETDECOLOR_HPP

#include "Process/OpenCV/photo/COcvDecolor.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class COcvWidgetDecolor : public CWidgetApplyOnly
{
    public:

        COcvWidgetDecolor(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        COcvWidgetDecolor(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class COcvWidgetDecolorFactory : public CWidgetFactory
{
    public:

        COcvWidgetDecolorFactory()
        {
            m_name = QObject::tr("Decolor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDecolor>(pParam);
        }
};

#endif // COCVWIDGETDECOLOR_HPP
