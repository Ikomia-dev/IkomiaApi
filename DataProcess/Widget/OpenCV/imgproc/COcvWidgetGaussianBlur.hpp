#ifndef COCVWIDGETGAUSSIANBLUR_HPP
#define COCVWIDGETGAUSSIANBLUR_HPP

#include <QComboBox>
#include <QDoubleSpinBox>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvGaussianBlur.hpp"

class COcvWidgetGaussianBlur : public CProtocolTaskWidget
{
    public:

        COcvWidgetGaussianBlur(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGaussianBlur(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGaussianBlurParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGaussianBlurParam>();

            auto pLabelSize = new QLabel(QObject::tr("Kernel size"));
            auto pSpinSize = new QSpinBox;
            pSpinSize->setValue(m_pParam->m_size.width);
            pSpinSize->setSingleStep(2);
            pSpinSize->setRange(1, INT_MAX - 1);

            auto pLabelSigmaX = new QLabel(QObject::tr("Sigma X"));
            auto pSpinSigmaX = new QDoubleSpinBox;
            pSpinSigmaX->setValue(m_pParam->m_sigmaX);
            pSpinSigmaX->setSingleStep(0.1);
            pSpinSigmaX->setRange(0, 255);

            auto pLabelSigmaY = new QLabel(QObject::tr("Sigma Y"));
            auto pSpinSigmaY = new QDoubleSpinBox;
            pSpinSigmaY->setValue(m_pParam->m_sigmaY);
            pSpinSigmaY->setSingleStep(0.1);
            pSpinSigmaY->setRange(0, 255);

            
            m_pLayout->addWidget(pLabelSize, 0, 0);
            m_pLayout->addWidget(pSpinSize, 0, 1);
            m_pLayout->addWidget(pLabelSigmaX, 1, 0);
            m_pLayout->addWidget(pSpinSigmaX, 1, 1);
            m_pLayout->addWidget(pLabelSigmaY, 2, 0);
            m_pLayout->addWidget(pSpinSigmaY, 2, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_size.width = m_pParam->m_size.height = pSpinSize->value();
                m_pParam->m_sigmaX = pSpinSigmaX->value();
                m_pParam->m_sigmaY = pSpinSigmaY->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvGaussianBlurParam> m_pParam = nullptr;
};

class COcvWidgetGaussianBlurFactory : public CWidgetFactory
{
    public:

        COcvWidgetGaussianBlurFactory()
        {
            m_name = QObject::tr("Gaussian Blur").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGaussianBlur>(pParam);
        }
};

#endif // COCVWIDGETGAUSSIANBLUR_HPP
