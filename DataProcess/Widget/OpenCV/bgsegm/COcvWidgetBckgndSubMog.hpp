#ifndef COCVWIDGETBCKGNDSUBMOG_HPP
#define COCVWIDGETBCKGNDSUBMOG_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubMog.hpp"

class COcvWidgetBckgndSubMog : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubMog(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubMog(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubMogParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubMogParam>();

            auto pSpinHistory = addSpin(0, tr("Length of history"), m_pParam->m_history);
            auto pSpinMixtures = addSpin(1, tr("Number of Gaussian mixtures"), m_pParam->m_mixturesCount);
            auto pSpinRatio = addDoubleSpin(2, tr("Background ratio"), m_pParam->m_bckRatio, 0.0, 10.0, 0.1, 1);
            auto pSpinSigma = addDoubleSpin(3, tr("Noise strength (standard deviation)"), m_pParam->m_noiseSigma, 0.0, 255.0, 1.0, 1);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_history = pSpinHistory->value();
                m_pParam->m_mixturesCount = pSpinMixtures->value();
                m_pParam->m_bckRatio = pSpinRatio->value();
                m_pParam->m_noiseSigma = pSpinSigma->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubMogParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubMogFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubMogFactory()
        {
            m_name = QObject::tr("MOG background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubMog>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBMOG_HPP
