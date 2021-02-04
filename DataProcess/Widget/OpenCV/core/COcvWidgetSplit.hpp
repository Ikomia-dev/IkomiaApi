#ifndef COCVWIDGETSPLIT_HPP
#define COCVWIDGETSPLIT_HPP

#include "Process/OpenCV/core/COcvSplit.hpp"

class COcvWidgetSplit : public CProtocolTaskWidget
{
    public:

        COcvWidgetSplit(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetSplit(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSplitParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSplitParam>();

            auto pLabel = new QLabel(tr("Number of output images"));
            auto pSpin = new QSpinBox;

            pSpin->setMinimum(1);
            pSpin->setValue(m_pParam->m_outputCount);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_outputCount = pSpin->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pSpin, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvSplitParam> m_pParam = nullptr;
};

class COcvWidgetSplitFactory : public CWidgetFactory
{
    public:

        COcvWidgetSplitFactory()
        {
            m_name = QObject::tr("Split Operator").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSplit>(pParam);
        }
};

#endif // COCVWIDGETSPLIT_HPP
