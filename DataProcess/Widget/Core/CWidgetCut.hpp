#ifndef CWIDGETCUT_HPP
#define CWIDGETCUT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CCut.hpp"

class CWidgetCut : public CProtocolTaskWidget
{
    public:

        CWidgetCut(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CWidgetCut(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CCutParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CCutParam>();

            auto pSpinSize = addSpin(0, tr("Size"), m_pParam->m_size);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_size = pSpinSize->value();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<CCutParam> m_pParam = nullptr;
};

class CWidgetCutFactory : public CWidgetFactory
{
    public:

        CWidgetCutFactory()
        {
            m_name = QObject::tr("Cut").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetCut>(pParam);
        }
};

#endif // CWIDGETCUT_HPP
