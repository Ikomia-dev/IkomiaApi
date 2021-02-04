#ifndef COCVWIDGETBCKGNDSUBLSBP_HPP
#define COCVWIDGETBCKGNDSUBLSBP_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubLsbp.hpp"

class COcvWidgetBckgndSubLsbp : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubLsbp(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubLsbp(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubLsbpParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubLsbpParam>();

            auto pComboMotion = addCombo(0, tr("Motion compensation"));
            pComboMotion->addItem(tr("None"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE);
            pComboMotion->addItem(tr("LK"), cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_LK);
            pComboMotion->setCurrentIndex(pComboMotion->findData(m_pParam->m_motionComp));

            auto pSpinSamples = addSpin(1, tr("Samples count"), m_pParam->m_samples);
            auto pSpinRadius = addSpin(2, tr("LSBP radius"), m_pParam->m_lsbpRadius);
            auto pSpinTLower = addDoubleSpin(3, tr("T lower bound"), m_pParam->m_TLower);
            auto pSpinTUpper = addDoubleSpin(4, tr("T upper bound"), m_pParam->m_TUpper);
            auto pSpinTInc = addDoubleSpin(5, tr("T increase step"), m_pParam->m_TInc);
            auto pSpinTDec = addDoubleSpin(6, tr("T decrease step"), m_pParam->m_TDec, 0.0, 10.0, 0.01, 2);
            auto pSpinRscale = addDoubleSpin(7, tr("Scale factor for threshold"), m_pParam->m_RScale);
            auto pSpinRIncDec = addDoubleSpin(8, tr("Increase/decrease step for threshold"), m_pParam->m_RIncDec, 0.0, 10.0, 0.001, 3);
            auto pSpinNoiseRemoveBG = addDoubleSpin(9, tr("Strength of the noise removal for background points"), m_pParam->m_noiseRemovalThresholdFacBG, 0.0, 1.0, 0.0001, 4);
            auto pSpinNoiseRemoveFG = addDoubleSpin(10, tr("Strength of the noise removal for foreground points"), m_pParam->m_noiseRemovalThresholdFacFG, 0.0, 1.0, 0.0001, 4);
            auto pSpinThreshold = addSpin(11, tr("LSBP threshold"), m_pParam->m_lsbpThreshold);
            auto pSpinMintCount = addSpin(12, tr("Minimal number of matches"), m_pParam->m_minCount);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_motionComp = pComboMotion->currentData().toInt();
                m_pParam->m_samples = pSpinSamples->value();
                m_pParam->m_lsbpRadius = pSpinRadius->value();
                m_pParam->m_TLower = pSpinTLower->value();
                m_pParam->m_TUpper = pSpinTUpper->value();
                m_pParam->m_TInc = pSpinTInc->value();
                m_pParam->m_TDec = pSpinTDec->value();
                m_pParam->m_RScale = pSpinRscale->value();
                m_pParam->m_RIncDec = pSpinRIncDec->value();
                m_pParam->m_noiseRemovalThresholdFacBG = pSpinNoiseRemoveBG->value();
                m_pParam->m_noiseRemovalThresholdFacFG = pSpinNoiseRemoveFG->value();
                m_pParam->m_lsbpThreshold = pSpinThreshold->value();
                m_pParam->m_minCount = pSpinMintCount->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubLsbpParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubLsbpFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubLsbpFactory()
        {
            m_name = QObject::tr("LSBP background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubLsbp>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBLSBP_HPP
