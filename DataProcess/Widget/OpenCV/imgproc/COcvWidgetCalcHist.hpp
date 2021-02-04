#ifndef COCVWIDGETCALCHIST_HPP
#define COCVWIDGETCALCHIST_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvCalcHist.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetCalcHist : public CProtocolTaskWidget
{
    public:

        COcvWidgetCalcHist(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCalcHist(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCalcHistParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCalcHistParam>();


            m_pSpin = new QSpinBox;
            m_pSpin->setMaximum(INT_MAX);
            m_pSpin->setValue(m_pParam->m_histSize[0]);

            QLabel* pLabelSpin = new QLabel(tr("Number of bins"));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_histSize[0] = m_pSpin->value();

                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelSpin, 2, 0);
            m_pLayout->addWidget(m_pSpin, 2, 1);
            
        }

    private:

        std::shared_ptr<COcvCalcHistParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
};

class COcvWidgetCalcHistFactory : public CWidgetFactory
{
    public:

        COcvWidgetCalcHistFactory()
        {
            m_name = QObject::tr("CalcHist").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetCalcHist>(pParam);
        }
};
#endif // COCVWIDGETCALCHIST_HPP
