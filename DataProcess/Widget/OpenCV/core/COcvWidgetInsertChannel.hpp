#ifndef COCVWIDGETINSERTCHANNEL_HPP
#define COCVWIDGETINSERTCHANNEL_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvInsertChannel.hpp"

class COcvWidgetInsertChannel : public CProtocolTaskWidget
{
    public:

        COcvWidgetInsertChannel(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetInsertChannel(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInsertChannelParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInsertChannelParam>();

            auto pSpinIndex = addSpin(0, tr("Channel index (zero-based)"), m_pParam->m_index, 0, 4, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_index = pSpinIndex->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvInsertChannelParam> m_pParam = nullptr;
};

class COcvWidgetInsertChannelFactory : public CWidgetFactory
{
    public:

        COcvWidgetInsertChannelFactory()
        {
            m_name = QObject::tr("Insert channel").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInsertChannel>(pParam);
        }
};

#endif // COCVWIDGETINSERTCHANNEL_HPP
