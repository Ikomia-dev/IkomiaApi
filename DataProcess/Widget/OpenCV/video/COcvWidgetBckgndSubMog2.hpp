#ifndef COCVWIDGETBCKGNDSUBMOG2_HPP
#define COCVWIDGETBCKGNDSUBMOG2_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvBckgndSubMog2.hpp"

class COcvWidgetBckgndSubMog2 : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubMog2(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubMog2(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubMog2Param>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubMog2Param>();

            auto pLabelHistory = new QLabel(tr("History"));
            auto pSpinHistory = new QSpinBox;
            pSpinHistory->setSingleStep(1);
            pSpinHistory->setRange(1, 10000);
            pSpinHistory->setValue(m_pParam->m_history);

            auto pLabelDistThresh = new QLabel(tr("Variance threshold"));
            auto pSpinVarThresh = new QDoubleSpinBox;
            pSpinVarThresh->setSingleStep(1.0);
            pSpinVarThresh->setRange(1, 255);
            pSpinVarThresh->setValue(m_pParam->m_varThreshold);

            auto pCheckDetectShadow = new QCheckBox(tr("Detect shadow"));
            pCheckDetectShadow->setChecked(m_pParam->m_bDetectShadow);

            
            m_pLayout->addWidget(pLabelHistory, 0, 0);
            m_pLayout->addWidget(pSpinHistory, 0, 1);
            m_pLayout->addWidget(pLabelDistThresh, 1, 0);
            m_pLayout->addWidget(pSpinVarThresh, 1, 1);
            m_pLayout->addWidget(pCheckDetectShadow, 2, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_history = pSpinHistory->value();
                m_pParam->m_varThreshold = pSpinVarThresh->value();
                m_pParam->m_bDetectShadow = pCheckDetectShadow->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubMog2Param> m_pParam = nullptr;
};

class COcvWidgetBckgndSubMog2Factory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubMog2Factory()
        {
            m_name = QObject::tr("MOG2 background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubMog2>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBMOG2_HPP
