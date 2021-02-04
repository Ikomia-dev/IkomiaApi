#ifndef CWIDGETAPPLYONLY_HPP
#define CWIDGETAPPLYONLY_HPP

#include "Protocol/CProtocolTaskWidget.h"

class CWidgetApplyOnly : public CProtocolTaskWidget
{
    public:

        CWidgetApplyOnly(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        CWidgetApplyOnly(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = pParam;
            init();
        }

    protected:

        void init() final
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CProtocolTaskParam>();

            // Remove scroll area widget from layout and delete it
            auto pLayout = dynamic_cast<QVBoxLayout*>(layout());
            if(pLayout != nullptr)
            {
                pLayout->removeWidget(m_pScrollArea);
                pLayout->insertStretch(0, 1);
                m_pScrollArea->deleteLater();
            }

            connect(m_pApplyBtn, &QPushButton::clicked, [&]{ emit doApplyProcess(m_pParam); } );
        }

    private:

        std::shared_ptr<CProtocolTaskParam> m_pParam = nullptr;
};

#endif // CWIDGETAPPLYONLY_HPP
