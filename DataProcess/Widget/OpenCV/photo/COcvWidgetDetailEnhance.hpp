#ifndef COCVWIDGETDETAILENHANCE_HPP
#define COCVWIDGETDETAILENHANCE_HPP

#include "Process/OpenCV/photo/COcvDetailEnhance.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetDetailEnhance : public CProtocolTaskWidget
{
    public:

        COcvWidgetDetailEnhance(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDetailEnhance(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDetailEnhanceParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDetailEnhanceParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma range"));
            auto pSpinSigmaS = new QDoubleSpinBox;
            auto pSpinSigmaR = new QDoubleSpinBox;

            pSpinSigmaS->setSingleStep(1);
            pSpinSigmaR->setSingleStep(0.1);

            pSpinSigmaS->setRange(0, 200);
            pSpinSigmaR->setRange(0, 1);

            pSpinSigmaS->setValue(m_pParam->m_sigma_s);
            pSpinSigmaR->setValue(m_pParam->m_sigma_r);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_sigma_s = pSpinSigmaS->value();
                m_pParam->m_sigma_r = pSpinSigmaR->value();
                emit doApplyProcess(m_pParam); } );

            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(pSpinSigmaS, 0, 1);

            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(pSpinSigmaR, 1, 1);

            
        }

    private:

        std::shared_ptr<COcvDetailEnhanceParam> m_pParam = nullptr;
};

class COcvWidgetDetailEnhanceFactory : public CWidgetFactory
{
    public:

        COcvWidgetDetailEnhanceFactory()
        {
            m_name = QObject::tr("Detail Enhance Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDetailEnhance>(pParam);
        }
};

#endif // COCVWIDGETDETAILENHANCE_HPP
