#ifndef CPROTOCOLTASKWIDGETWRAP_H
#define CPROTOCOLTASKWIDGETWRAP_H

#include "PyCoreGlobal.h"
#include "Protocol/CProtocolTaskWidget.h"

class CProtocolTaskWidgetWrap : public CProtocolTaskWidget, public wrapper<CProtocolTaskWidget>
{
    public:

        CProtocolTaskWidgetWrap();
        CProtocolTaskWidgetWrap(QWidget *parent);
        virtual ~CProtocolTaskWidgetWrap() = default;

        void            init();

        void            setLayout(long long layoutPtr);

        void            emitApply(const ProtocolTaskParamPtr &paramPtr);
        void            emitSendProcessAction(int flags);
        void            emitSetGraphicsTool(GraphicsShape tool);
        void            emitSetGraphicsCategory(const std::string& category);

        virtual void    onApply();
        void            default_onApply();
};

#endif // CPROTOCOLTASKWIDGETWRAP_H
