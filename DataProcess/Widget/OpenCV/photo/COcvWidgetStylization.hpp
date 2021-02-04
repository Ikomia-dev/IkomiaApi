#ifndef COCVWIDGETSTYLIZATION_HPP
#define COCVWIDGETSTYLIZATION_HPP
#include "../../../Process/OpenCV/photo/COcvStylization.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetStylization : public CProtocolTaskWidget
{
    public:

        COcvWidgetStylization(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetStylization(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvStylizationParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvStylizationParam>();

            auto pLabelSigmaS = new QLabel(tr("Sigma spatial"));
            auto pLabelSigmaR = new QLabel(tr("Sigma range"));

            auto pSpinSigmaS = new QDoubleSpinBox;
            auto pSpinSigmaR = new QDoubleSpinBox;

            pSpinSigmaS->setValue(m_pParam->m_sigmaS);
            pSpinSigmaS->setRange(0, 100);
            pSpinSigmaS->setSingleStep(1);

            pSpinSigmaR->setValue(m_pParam->m_sigmaR);
            pSpinSigmaR->setRange(0, 1);
            pSpinSigmaR->setSingleStep(0.01);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_sigmaS = pSpinSigmaS->value();
                m_pParam->m_sigmaR = pSpinSigmaR->value();
                emit doApplyProcess(m_pParam); } );

            
            m_pLayout->addWidget(pLabelSigmaS, 0, 0);
            m_pLayout->addWidget(pSpinSigmaS, 0, 1);

            m_pLayout->addWidget(pLabelSigmaR, 1, 0);
            m_pLayout->addWidget(pSpinSigmaR, 1, 1);

            
        }

    private:

        std::shared_ptr<COcvStylizationParam> m_pParam = nullptr;
};

class COcvWidgetStylizationFactory : public CWidgetFactory
{
    public:

        COcvWidgetStylizationFactory()
        {
            m_name = QObject::tr("Stylization").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetStylization>(pParam);
        }
};
#endif // COCVWIDGETSTYLIZATION_HPP
