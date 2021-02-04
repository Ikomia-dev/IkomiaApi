#ifndef COCVWIDGETDEEPFLOW_HPP
#define COCVWIDGETDEEPFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvDeepFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDeepFlow : public CProtocolTaskWidget
{
    public:

        COcvWidgetDeepFlow(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDeepFlow(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDeepFlowParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDeepFlowParam>();

            auto pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvDeepFlowParam> m_pParam = nullptr;
};

class COcvWidgetDeepFlowFactory : public CWidgetFactory
{
    public:

        COcvWidgetDeepFlowFactory()
        {
            m_name = QObject::tr("DeepFlow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDeepFlow>(pParam);
        }
};
#endif // COCVWIDGETDEEPFLOW_HPP
