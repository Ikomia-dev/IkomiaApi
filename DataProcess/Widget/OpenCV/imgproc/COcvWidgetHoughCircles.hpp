#ifndef COCVWIDGETHOUGHCIRCLES_HPP
#define COCVWIDGETHOUGHCIRCLES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvHoughCircles.hpp"

class COcvWidgetHoughCircles : public CProtocolTaskWidget
{
    public:

        COcvWidgetHoughCircles(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetHoughCircles(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHoughCirclesParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHoughCirclesParam>();

            auto pComboMethod = addCombo(tr("Method"));
            pComboMethod->addItem("Hough gradient", cv::HOUGH_GRADIENT);
            pComboMethod->addItem("Hough gradient variant", cv::HOUGH_GRADIENT_ALT);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_method));

            auto pSpinDp = addDoubleSpin(tr("Accumulator ratio"), m_pParam->m_dp, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMinDst = addDoubleSpin(tr("Min distance between centers"), m_pParam->m_minDist, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinParam1 = addDoubleSpin(tr("Canny high threshold"), m_pParam->m_param1, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinParam2 = addDoubleSpin(tr("Accumulator threshold"), m_pParam->m_param2, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMinRaduis = addSpin(tr("Minimum radius"), m_pParam->m_minRadius, 0, INT_MAX, 1);
            auto pSpinMaxRaduis = addSpin(tr("Maximum radius"), m_pParam->m_maxRadius, 0, INT_MAX, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_method = pComboMethod->currentData().toInt();
                m_pParam->m_dp = pSpinDp->value();
                m_pParam->m_minDist = pSpinMinDst->value();
                m_pParam->m_param1 = pSpinParam1->value();
                m_pParam->m_param2 = pSpinParam2->value();
                m_pParam->m_minRadius = pSpinMinRaduis->value();
                m_pParam->m_maxRadius = pSpinMaxRaduis->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvHoughCirclesParam> m_pParam = nullptr;
};

class COcvWidgetHoughCirclesFactory : public CWidgetFactory
{
    public:

        COcvWidgetHoughCirclesFactory()
        {
            m_name = QObject::tr("Hough circles").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetHoughCircles>(pParam);
        }
};

#endif // COCVWIDGETHOUGHCIRCLES_HPP
