#ifndef COCVWIDGETAND_HPP
#define COCVWIDGETAND_HPP

#include "Process/OpenCV/core/COcvLogicalOp.hpp"
#include "Core/CWidgetFactory.hpp"
#include <QComboBox>

class COcvWidgetLogicalOp : public CProtocolTaskWidget
{
    public:

        COcvWidgetLogicalOp(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetLogicalOp(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvLogicalOpParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvLogicalOpParam>();

            auto pLabel = new QLabel(tr("Operation"));

            m_pOpComboBox = new QComboBox();
            m_pOpComboBox->addItem("And", COcvLogicalOpParam::AND);
            m_pOpComboBox->addItem("Or", COcvLogicalOpParam::OR);
            m_pOpComboBox->addItem("Not", COcvLogicalOpParam::NOT);
            m_pOpComboBox->addItem("Xor", COcvLogicalOpParam::XOR);
            m_pOpComboBox->setCurrentIndex(m_pOpComboBox->findData(m_pParam->m_op));

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(m_pOpComboBox, 0, 1);

            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_op = m_pOpComboBox->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        QComboBox*                          m_pOpComboBox = nullptr;
        std::shared_ptr<COcvLogicalOpParam> m_pParam = nullptr;
};

class COcvWidgetLogicalOpFactory : public CWidgetFactory
{
    public:

        COcvWidgetLogicalOpFactory()
        {
            m_name = QObject::tr("Logical operations").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLogicalOp>(pParam);
        }
};

#endif // COCVWIDGETAND_HPP
