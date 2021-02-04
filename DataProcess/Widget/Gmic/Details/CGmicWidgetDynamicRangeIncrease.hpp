#ifndef CGMICWIDGETDYNAMICRANGEINCREASE_HPP
#define CGMICWIDGETDYNAMICRANGEINCREASE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicDynamicRangeIncrease.hpp"

class CGmicWidgetDynamicRangeIncrease : public CProtocolTaskWidget
{
    public:

        CGmicWidgetDynamicRangeIncrease(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDynamicRangeIncrease(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDynamicRangeIncreaseParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDynamicRangeIncreaseParam>();

            auto pSpinMapTones = addDoubleSpin(0, tr("Map tones"), m_pParam->m_mapTones, 0, 1);
            auto pSpinShadows = addDoubleSpin(1, tr("Recover shadows"), m_pParam->m_recoverShadows, 0, 1);
            auto pSpinHighlights = addDoubleSpin(2, tr("Recover highlights"), m_pParam->m_recoverHighligths, 0, 1);
            auto pSpinDetails = addDoubleSpin(3, tr("Enhance details"), m_pParam->m_enhanceDetails, 0, 5);
            auto pSpinDetailStrength = addDoubleSpin(4, tr("Detail strength"), m_pParam->m_detailStrength, 0, 1);
            auto pCheckMapTones = addCheck(5, tr("Map tones"), m_pParam->m_bMapTones);
            auto pCheckDetails = addCheck(6, tr("Enhance details"), m_pParam->m_bEnhanceDetails);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_mapTones = pSpinMapTones->value();
                m_pParam->m_recoverShadows = pSpinShadows->value();
                m_pParam->m_recoverHighligths = pSpinHighlights->value();
                m_pParam->m_enhanceDetails = pSpinDetails->value();
                m_pParam->m_detailStrength = pSpinDetailStrength->value();
                m_pParam->m_bMapTones = pCheckMapTones->isChecked();
                m_pParam->m_bEnhanceDetails = pCheckDetails->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicDynamicRangeIncreaseParam> m_pParam = nullptr;
};

class CGmicWidgetDynamicRangeIncreaseFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDynamicRangeIncreaseFactory()
        {
            m_name = QObject::tr("Dynamic range increase").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDynamicRangeIncrease>(pParam);
        }
};

#endif // CGMICWIDGETDYNAMICRANGEINCREASE_HPP
