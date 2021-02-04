#ifndef COCVWIDGETSIMPLEWB_HPP
#define COCVWIDGETSIMPLEWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvSimpleWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetSimpleWB : public CProtocolTaskWidget
{
    public:

        COcvWidgetSimpleWB(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetSimpleWB(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSimpleWBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSimpleWBParam>();

            auto pSpinInMin = addDoubleSpin(0, tr("Input min"), m_pParam->m_inputMin);
            auto pSpinInMax = addDoubleSpin(1, tr("Input max"), m_pParam->m_inputMax);
            auto pSpinOutMin = addDoubleSpin(2, tr("Output min"), m_pParam->m_outputMin);
            auto pSpinOutMax = addDoubleSpin(3, tr("Output max"), m_pParam->m_outputMax);
            auto pSpinP = addDoubleSpin(4, tr("% of top/bottom to ignore"), m_pParam->m_P);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_inputMin = pSpinInMin->value();
                m_pParam->m_inputMax = pSpinInMax->value();
                m_pParam->m_outputMin = pSpinOutMin->value();
                m_pParam->m_outputMax = pSpinOutMax->value();
                m_pParam->m_P = pSpinP->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSimpleWBParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pDblSpinSat = nullptr;
};

class COcvWidgetSimpleWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetSimpleWBFactory()
        {
            m_name = QObject::tr("SimpleWB").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSimpleWB>(pParam);
        }
};
#endif // COCVWIDGETSIMPLEWB_HPP
