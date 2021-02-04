#ifndef COCVWIDGETGRAYWORLDWB_HPP
#define COCVWIDGETGRAYWORLDWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvGrayworldWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetGrayworldWB : public CProtocolTaskWidget
{
    public:

        COcvWidgetGrayworldWB(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGrayworldWB(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGrayworldWBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGrayworldWBParam>();

            QLabel* pLabelSpinSat = new QLabel(QObject::tr("Saturation threshold"));
            m_pDblSpinSat = new QDoubleSpinBox;
            m_pDblSpinSat->setValue(m_pParam->m_satThreshold);
            m_pDblSpinSat->setSingleStep(0.1f);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_satThreshold = m_pDblSpinSat->value();
                emit doApplyProcess(m_pParam);
            });


            m_pLayout->addWidget(pLabelSpinSat, 0, 0);
            m_pLayout->addWidget(m_pDblSpinSat, 0, 1);

        }

    private:

        std::shared_ptr<COcvGrayworldWBParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pDblSpinSat = nullptr;
};

class COcvWidgetGrayworldWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetGrayworldWBFactory()
        {
            m_name = QObject::tr("GrayworldWB").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGrayworldWB>(pParam);
        }
};
#endif // COCVWIDGETGRAYWORLDWB_HPP
