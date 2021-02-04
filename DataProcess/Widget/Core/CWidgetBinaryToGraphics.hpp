#ifndef CWIDGETBINARYTOGRAPHICS_HPP
#define CWIDGETBINARYTOGRAPHICS_HPP

#include "Widget/Core/CWidgetApplyOnly.hpp"
#include "Process/Core/CBinaryToGraphics.h"

class CWidgetBinaryToGraphics : public CWidgetApplyOnly
{
    public:

        CWidgetBinaryToGraphics(QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(parent)
        {
        }

        CWidgetBinaryToGraphics(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWidgetApplyOnly(pParam, parent)
        {
        }

    private:

        std::shared_ptr<CBinaryToGraphicsParam> m_pParam = nullptr;
};

class CWidgetBinaryToGraphicsFactory : public CWidgetFactory
{
    public:

        CWidgetBinaryToGraphicsFactory()
        {
            m_name = QObject::tr("Binary to graphics").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetBinaryToGraphics>(pParam);
        }
};

#endif // CWIDGETBINARYTOGRAPHICS_HPP
