#ifndef CGMICWIDGETINPAINT_HPP
#define CGMICWIDGETINPAINT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicInpaint.hpp"

class CGmicWidgetInpaint : public CProtocolTaskWidget
{
    public:

        CGmicWidgetInpaint(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetInpaint(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicInpaintParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicInpaintParam>();

            auto pSpinPatchSize = addSpin(0, tr("Patch size"), m_pParam->m_patchSize);
            auto pSpinLookupSize = addSpin(1, tr("Lookup size"), m_pParam->m_lookupSize);
            auto pSpinLookupFactor = addDoubleSpin(2, tr("Lookup factor"), m_pParam->m_lookupFactor);
            auto pSpinLookupInc = addSpin(3, tr("Lookup increment"), m_pParam->m_lookupInc);
            auto pSpinBlendSize = addSpin(4, tr("Blend size"), m_pParam->m_blendSize);
            auto pSpinBlendThresh = addSpin(5, tr("Blend threshold"), m_pParam->m_blendThreshold);
            auto pSpinBlendDecay = addDoubleSpin(6, tr("Blend decay"), m_pParam->m_blendDecay);
            auto pSpinBlendScales = addSpin(7, tr("Blend scales"), m_pParam->m_blendScales);
            auto pCheckBlendOuter = addCheck(8, tr("Outer blending"), m_pParam->m_bBlendOuter);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_patchSize = pSpinPatchSize->value();
                m_pParam->m_lookupSize = pSpinLookupSize->value();
                m_pParam->m_lookupFactor = pSpinLookupFactor->value();
                m_pParam->m_lookupInc = pSpinLookupInc->value();
                m_pParam->m_blendSize = pSpinBlendSize->value();
                m_pParam->m_blendThreshold = pSpinBlendThresh->value();
                m_pParam->m_blendDecay = pSpinBlendDecay->value();
                m_pParam->m_blendScales = pSpinBlendScales->value();
                m_pParam->m_bBlendOuter = pCheckBlendOuter->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicInpaintParam> m_pParam = nullptr;
};

class CGmicWidgetInpaintFactory : public CWidgetFactory
{
    public:

        CGmicWidgetInpaintFactory()
        {
            m_name = QObject::tr("Patch-based Inpaint").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetInpaint>(pParam);
        }
};

#endif // CGMICWIDGETINPAINT_HPP
