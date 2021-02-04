#ifndef COCVWIDGETSUPERRESBTVL1_HPP
#define COCVWIDGETSUPERRESBTVL1_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/superres/COcvSuperResBTVL1.h"

class COcvWidgetSuperResBTVL1 : public CProtocolTaskWidget
{
    public:

        COcvWidgetSuperResBTVL1(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperResBTVL1(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperResBTVL1Param>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperResBTVL1Param>();

            auto pSpinScale = addSpin(0, tr("Scale factor"), m_pParam->m_scale);
            auto pSpinIterations = addSpin(1, tr("Number of iterations"), m_pParam->m_iterations);
            auto pSpinTau = addDoubleSpin(2, tr("Tau"), m_pParam->m_tau, 0.0, DBL_MAX, 0.1, 1);
            auto pSpinLambda = addDoubleSpin(3, tr("Lambda"), m_pParam->m_lambda, 0.0, DBL_MAX, 0.01, 2);
            auto pSpinAlpha = addDoubleSpin(4, tr("Alpha"), m_pParam->m_alpha, 0.0, DBL_MAX, 0.1, 1);
            auto pSpinBTVKernelSize = addSpin(5, tr("Bilateral TV kernel size"), m_pParam->m_btvKernelSize, 1, INT_MAX, 2);
            auto pSpinBlurKernelSize = addDoubleSpin(6, tr("Gaussian kernel size"), m_pParam->m_blurKernelSize, 1, INT_MAX, 2);
            auto pSpinBlurSigma = addDoubleSpin(7, tr("Gaussian sigma"), m_pParam->m_blurSigma, 0.0, DBL_MAX, 0.1, 1);
            auto pSpinTemporalRadius = addSpin(8, tr("Radius of temporal search area"), m_pParam->m_temporalAreaRadius, 1, INT_MAX, 1);

            auto pComboOptFlow = addCombo(9, tr("Optical flow"));
            pComboOptFlow->addItem(tr("Brox"), COcvSuperResBTVL1Param::BROX);
            pComboOptFlow->addItem(tr("DualTVL1"), COcvSuperResBTVL1Param::DUAL_TVL1);
            pComboOptFlow->addItem(tr("Farneback"), COcvSuperResBTVL1Param::FARNEBACK);
            pComboOptFlow->addItem(tr("PyrLK"), COcvSuperResBTVL1Param::PYR_LK);
            pComboOptFlow->setCurrentIndex(pComboOptFlow->findData(m_pParam->m_opticalFlowType));

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_iterations = pSpinIterations->value();
                m_pParam->m_tau = pSpinTau->value();
                m_pParam->m_lambda = pSpinLambda->value();
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_btvKernelSize = pSpinBTVKernelSize->value();
                m_pParam->m_blurKernelSize = pSpinBlurKernelSize->value();
                m_pParam->m_blurSigma = pSpinBlurSigma->value();
                m_pParam->m_temporalAreaRadius = pSpinTemporalRadius->value();
                m_pParam->m_opticalFlowType = pComboOptFlow->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvSuperResBTVL1Param> m_pParam = nullptr;
};

class COcvWidgetSuperResBTVL1Factory : public CWidgetFactory
{
    public:

        COcvWidgetSuperResBTVL1Factory()
        {
            m_name = QObject::tr("Super Resolution BTVL1").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperResBTVL1>(pParam);
        }
};

#endif // COCVWIDGETSUPERRESBTVL1_HPP
