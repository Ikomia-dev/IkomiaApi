#ifndef COCVWIDGETEXTRACTCHANNEL_HPP
#define COCVWIDGETEXTRACTCHANNEL_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvExtractChannel.hpp"

class COcvWidgetExtractChannel : public CProtocolTaskWidget
{
    public:

        COcvWidgetExtractChannel(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetExtractChannel(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvExtractChannelParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvExtractChannelParam>();

            auto pSpinIndex = addSpin(0, tr("Channel index (zero-based)"), m_pParam->m_index, 0, 3, 1);
            
            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_index = pSpinIndex->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvExtractChannelParam> m_pParam = nullptr;
};

class COcvWidgetExtractChannelFactory : public CWidgetFactory
{
    public:

        COcvWidgetExtractChannelFactory()
        {
            m_name = QObject::tr("Extract channel").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetExtractChannel>(pParam);
        }
};

#endif // COCVWIDGETEXTRACTCHANNEL_HPP
