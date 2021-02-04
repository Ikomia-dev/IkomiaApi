#ifndef CGMICWIDGETBOOSTFADE_HPP
#define CGMICWIDGETBOOSTFADE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicBoostFade.hpp"

class CGmicWidgetBoostFade : public CProtocolTaskWidget
{
    public:

        CGmicWidgetBoostFade(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetBoostFade(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicBoostFadeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicBoostFadeParam>();

            auto pSpinAmplitude = addSpin(0, tr("Amplitude"), m_pParam->m_amplitude, 0, 10);
            auto pComboColorSpace = addCombo(1, tr("Color space"));
            pComboColorSpace->addItem(tr("YCbCr"), CGmicBoostFadeParam::YCBCR);
            pComboColorSpace->addItem(tr("Lab"), CGmicBoostFadeParam::LAB);

            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_amplitude = pSpinAmplitude->value();
                m_pParam->m_colorSpace = pComboColorSpace->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicBoostFadeParam> m_pParam = nullptr;
};

class CGmicWidgetBoostFadeFactory : public CWidgetFactory
{
    public:

        CGmicWidgetBoostFadeFactory()
        {
            m_name = QObject::tr("Boost fade").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetBoostFade>(pParam);
        }
};

#endif // CGMICWIDGETBOOSTFADE_HPP
