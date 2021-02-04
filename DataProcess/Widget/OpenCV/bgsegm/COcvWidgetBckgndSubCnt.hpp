#ifndef COCVWIDGETBCKGNDSUBCNT_HPP
#define COCVWIDGETBCKGNDSUBCNT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bgsegm/COcvBckgndSubCnt.hpp"

class COcvWidgetBckgndSubCnt : public CProtocolTaskWidget
{
    public:

        COcvWidgetBckgndSubCnt(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBckgndSubCnt(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBckgndSubCntParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBckgndSubCntParam>();

            auto pLabelMinPxStability = new QLabel(tr("Minimum pixel stability"));
            auto pSpinMinPxStability = new QSpinBox;
            pSpinMinPxStability->setSingleStep(1);
            pSpinMinPxStability->setRange(1, 1000);
            pSpinMinPxStability->setValue(m_pParam->m_minPixelStability);

            auto pLabelMaxPxStability = new QLabel(tr("Minimum pixel stability"));
            auto pSpinMaxPxStability = new QSpinBox;
            pSpinMaxPxStability->setSingleStep(1);
            pSpinMaxPxStability->setRange(1, 10000);
            pSpinMaxPxStability->setValue(m_pParam->m_maxPixelStability);

            auto pCheckHistory = new QCheckBox(tr("Use history"));
            pCheckHistory->setChecked(m_pParam->m_bUseHistory);

            auto pCheckParallel = new QCheckBox(tr("Parallel computing"));
            pCheckParallel->setChecked(m_pParam->m_bIsParallel);

            
            m_pLayout->addWidget(pLabelMinPxStability, 0, 0);
            m_pLayout->addWidget(pSpinMinPxStability, 0, 1);
            m_pLayout->addWidget(pLabelMaxPxStability, 1, 0);
            m_pLayout->addWidget(pSpinMaxPxStability, 1, 1);
            m_pLayout->addWidget(pCheckHistory, 2, 0, 1, 2);
            m_pLayout->addWidget(pCheckParallel, 3, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_minPixelStability = pSpinMinPxStability->value();
                m_pParam->m_maxPixelStability = pSpinMaxPxStability->value();
                m_pParam->m_bUseHistory = pCheckHistory->isChecked();
                m_pParam->m_bIsParallel = pCheckParallel->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBckgndSubCntParam> m_pParam = nullptr;
};

class COcvWidgetBckgndSubCntFactory : public CWidgetFactory
{
    public:

        COcvWidgetBckgndSubCntFactory()
        {
            m_name = QObject::tr("CNT background substractor").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBckgndSubCnt>(pParam);
        }
};

#endif // COCVWIDGETBCKGNDSUBCNT_HPP
