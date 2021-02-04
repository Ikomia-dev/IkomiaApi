#ifndef COCVWIDGETBCKGNDSUBKNN_HPP
#define COCVWIDGETBCKGNDSUBKNN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvBckgndSubKnn.hpp"

class COcvWidgetBckgndSubKnn : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubKnn(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubKnn(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubKnnParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubKnnParam>();

            auto pLabelHistory = new QLabel(tr("History"));
            auto pSpinHistory = new QSpinBox;
            pSpinHistory->setSingleStep(1);
            pSpinHistory->setRange(1, 10000);
            pSpinHistory->setValue(m_pParam->m_history);

            auto pLabelDistThresh = new QLabel(tr("Distance threshold"));
            auto pSpinDistThresh = new QDoubleSpinBox;
            pSpinDistThresh->setSingleStep(1.0);
            pSpinDistThresh->setRange(1, 1000);
            pSpinDistThresh->setValue(m_pParam->m_distanceThreshold);

            auto pCheckDetectShadow = new QCheckBox(tr("Detect shadow"));
            pCheckDetectShadow->setChecked(m_pParam->m_bDetectShadow);

            
            m_pLayout->addWidget(pLabelHistory, 0, 0);
            m_pLayout->addWidget(pSpinHistory, 0, 1);
            m_pLayout->addWidget(pLabelDistThresh, 1, 0);
            m_pLayout->addWidget(pSpinDistThresh, 1, 1);
            m_pLayout->addWidget(pCheckDetectShadow, 2, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_history = pSpinHistory->value();
                m_pParam->m_distanceThreshold = pSpinDistThresh->value();
                m_pParam->m_bDetectShadow = pCheckDetectShadow->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubKnnParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubKnnFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubKnnFactory()
        {
            m_name = QObject::tr("KNN background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubKnn>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBKNN_HPP
