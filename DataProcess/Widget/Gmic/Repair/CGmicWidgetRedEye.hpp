#ifndef CGMICWIDGETREDEYE_HPP
#define CGMICWIDGETREDEYE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicRedEye.hpp"

class CGmicWidgetRedEye : public CProtocolTaskWidget
{
    public:

        CGmicWidgetRedEye(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetRedEye(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicRedEyeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicRedEyeParam>();

            auto pSpinThreshold = addSpin(0, tr("Threshold"), m_pParam->m_threshold, 0, 100, 1);
            auto pSpinSmoothness = addDoubleSpin(1, tr("Smoothness"), m_pParam->m_smoothness);
            auto pSpinAttenuation = addDoubleSpin(2, tr("Attenuation"), m_pParam->m_attenuation);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_smoothness = pSpinSmoothness->value();
                m_pParam->m_attenuation = pSpinAttenuation->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicRedEyeParam> m_pParam = nullptr;
};

class CGmicWidgetRedEyeFactory : public CWidgetFactory
{
    public:

        CGmicWidgetRedEyeFactory()
        {
            m_name = QObject::tr("Red-eye").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetRedEye>(pParam);
        }
};

#endif // CGMICWIDGETREDEYE_HPP
