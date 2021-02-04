#ifndef COCVWIDGETL0SMOOTH_HPP
#define COCVWIDGETL0SMOOTH_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvL0Smooth.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetL0Smooth : public CProtocolTaskWidget
{
    public:

        COcvWidgetL0Smooth(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetL0Smooth(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvL0SmoothParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvL0SmoothParam>();


            QLabel* pLabelSpinLambda = new QLabel(QObject::tr("lambda"));
            m_pDblSpinLambda = new QDoubleSpinBox;
            m_pDblSpinLambda->setRange(0, 1);
            m_pDblSpinLambda->setSingleStep(0.01);
            m_pDblSpinLambda->setValue(m_pParam->m_lambda);

            QLabel* pLabelSpinKappa = new QLabel(QObject::tr("Kappa"));
            m_pDblSpinKappa = new QDoubleSpinBox;
            m_pDblSpinKappa->setRange(0, 100);
            m_pDblSpinKappa->setValue(m_pParam->m_kappa);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_lambda = m_pDblSpinLambda->value();
                m_pParam->m_kappa = m_pDblSpinKappa->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelSpinLambda, 0, 0);
            m_pLayout->addWidget(m_pDblSpinLambda, 0, 1);

            m_pLayout->addWidget(pLabelSpinKappa, 1, 0);
            m_pLayout->addWidget(m_pDblSpinKappa, 1, 1);

            
        }

    private:

        std::shared_ptr<COcvL0SmoothParam> m_pParam = nullptr;
        QDoubleSpinBox*                     m_pDblSpinLambda = nullptr;
        QDoubleSpinBox*                     m_pDblSpinKappa = nullptr;
};

class COcvWidgetL0SmoothFactory : public CWidgetFactory
{
    public:

        COcvWidgetL0SmoothFactory()
        {
            m_name = QObject::tr("L0 smooth filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetL0Smooth>(pParam);
        }
};
#endif // COCVWIDGETL0SMOOTH_HPP
