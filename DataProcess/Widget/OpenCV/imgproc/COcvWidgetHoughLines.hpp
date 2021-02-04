#ifndef COCVWIDGETHOUGHLINES_HPP
#define COCVWIDGETHOUGHLINES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvHoughLines.hpp"

class COcvWidgetHoughLines : public CProtocolTaskWidget
{
    public:

        COcvWidgetHoughLines(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetHoughLines(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHoughLinesParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHoughLinesParam>();

            auto pCheckProba = addCheck(0, tr("Probabilistic method"), m_pParam->m_bProbabilistic);
            auto pSpinRho = addDoubleSpin(1, "Rho", m_pParam->m_rho, 1.0, DBL_MAX, 1.0, 1);
            auto pSpinTheta = addDoubleSpin(2, "Theta", m_pParam->m_theta, 0.0, 2*CV_PI/180.0, 1.0, 5);
            auto pSpinThreshold = addSpin(3, tr("Accumulator threshold"), m_pParam->m_threshold, 0, INT_MAX, 1);
            auto pSpinMinLength = addDoubleSpin(4, tr("Minimum length"), m_pParam->m_minLength, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMaxGap = addDoubleSpin(5, tr("Maximum gap"), m_pParam->m_maxGap, 0.0, DBL_MAX, 1.0, 1);
            

            pSpinMinLength->setEnabled(m_pParam->m_bProbabilistic);
            pSpinMaxGap->setEnabled(m_pParam->m_bProbabilistic);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bProbabilistic = pCheckProba->isChecked();
                m_pParam->m_rho = pSpinRho->value();
                m_pParam->m_theta = pSpinTheta->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_minLength = pSpinMinLength->value();
                m_pParam->m_maxGap = pSpinMaxGap->value();
                emit doApplyProcess(m_pParam);
            });

            connect(pCheckProba, &QCheckBox::clicked, [=](bool bChecked)
            {
                pSpinMinLength->setEnabled(bChecked);
                pSpinMaxGap->setEnabled(bChecked);
            });
        }

    private:

        std::shared_ptr<COcvHoughLinesParam> m_pParam = nullptr;
};

class COcvWidgetHoughLinesFactory : public CWidgetFactory
{
    public:

        COcvWidgetHoughLinesFactory()
        {
            m_name = QObject::tr("Hough lines").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetHoughLines>(pParam);
        }
};

#endif // COCVWIDGETHOUGHLINES_HPP
