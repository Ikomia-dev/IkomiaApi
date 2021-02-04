#ifndef COCVWIDGETEDGEPRESERVINGFILTER_HPP
#define COCVWIDGETEDGEPRESERVINGFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/photo/COcvEdgePreservingFilter.hpp"

class COcvWidgetEdgePreservingFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetEdgePreservingFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetEdgePreservingFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvEdgePreservingFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvEdgePreservingFilterParam>();

            auto pLabelFilterType = new QLabel(tr("Filter type"));
            auto pComboFilterType = new QComboBox;
            pComboFilterType->addItem("Recursive", cv::RECURS_FILTER);
            pComboFilterType->addItem("Normalized convolution", cv::NORMCONV_FILTER);
            pComboFilterType->setCurrentIndex(pComboFilterType->findData(m_pParam->m_flag));

            auto pLabelSigmaS = new QLabel(tr("Sigma S"));
            auto pSpinSigmaS = new QDoubleSpinBox;
            pSpinSigmaS->setSingleStep(1.0);
            pSpinSigmaS->setRange(0.0, 200.0);
            pSpinSigmaS->setValue(m_pParam->m_sigmaS);

            auto pLabelSigmaR = new QLabel(tr("Sigma R"));
            auto pSpinSigmaR = new QDoubleSpinBox;
            pSpinSigmaR->setSingleStep(0.1);
            pSpinSigmaR->setRange(0.0, 1.0);
            pSpinSigmaR->setValue(m_pParam->m_sigmaR);

            
            m_pLayout->addWidget(pLabelFilterType, 0, 0);
            m_pLayout->addWidget(pComboFilterType, 0, 1);
            m_pLayout->addWidget(pLabelSigmaS, 1, 0);
            m_pLayout->addWidget(pSpinSigmaS, 1, 1);
            m_pLayout->addWidget(pLabelSigmaR, 2, 0);
            m_pLayout->addWidget(pSpinSigmaR, 2, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_flag = pComboFilterType->currentData().toInt();
                m_pParam->m_sigmaS = pSpinSigmaS->value();
                m_pParam->m_sigmaR = pSpinSigmaR->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvEdgePreservingFilterParam> m_pParam = nullptr;
};

class COcvWidgetEdgePreservingFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetEdgePreservingFilterFactory()
        {
            m_name = QObject::tr("Edge Preserving Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetEdgePreservingFilter>(pParam);
        }
};

#endif // COCVWIDGETEDGEPRESERVINGFILTER_HPP
