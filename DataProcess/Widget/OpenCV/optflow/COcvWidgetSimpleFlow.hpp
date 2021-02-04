#ifndef COCVWIDGETSIMPLEFLOW_HPP
#define COCVWIDGETSIMPLEFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvSimpleFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSimpleFlow : public CProtocolTaskWidget
{
    public:

        COcvWidgetSimpleFlow(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSimpleFlow(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSimpleFlowParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSimpleFlowParam>();

            auto pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSimpleFlowParam> m_pParam = nullptr;
};

class COcvWidgetSimpleFlowFactory : public CWidgetFactory
{
    public:

        COcvWidgetSimpleFlowFactory()
        {
            m_name = QObject::tr("Simple Flow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSimpleFlow>(pParam);
        }
};
#endif // COCVWIDGETSIMPLEFLOW_HPP
