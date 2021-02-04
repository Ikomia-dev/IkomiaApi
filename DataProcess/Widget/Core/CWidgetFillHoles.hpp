#ifndef CWIDGETFILLHOLES_HPP
#define CWIDGETFILLHOLES_HPP

#include "Process/Core/CFillHoles.hpp"
#include "Widget/Core/CWidgetApplyOnly.hpp"

class CWidgetFillHoles : public CWidgetApplyOnly
{
    public:

        CWidgetFillHoles(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        CWidgetFillHoles(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }
};

class CWidgetFillHolesFactory : public CWidgetFactory
{
    public:

        CWidgetFillHolesFactory()
        {
            m_name = QObject::tr("Fill Holes").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetFillHoles>(pParam);
        }
};

#endif // CWIDGETFILLHOLES_HPP
