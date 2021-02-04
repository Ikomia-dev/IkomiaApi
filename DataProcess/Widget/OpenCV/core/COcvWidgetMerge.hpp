#ifndef COCVWIDGETMERGE_HPP
#define COCVWIDGETMERGE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMerge.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetMerge : public CProtocolTaskWidget
{
    public:

        COcvWidgetMerge(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetMerge(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMergeParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMergeParam>();

            auto pLabel = new QLabel(tr("Number of input images"));
            auto pSpin = new QSpinBox;

            pSpin->setMinimum(1);
            pSpin->setValue(m_pParam->m_inputCount);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_inputCount = pSpin->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pSpin, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvMergeParam> m_pParam = nullptr;
};

class COcvWidgetMergeFactory : public CWidgetFactory
{
    public:

        COcvWidgetMergeFactory()
        {
            m_name = QObject::tr("Merge Operator").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetMerge>(pParam);
        }
};

#endif // COCVWIDGETMERGE_HPP
