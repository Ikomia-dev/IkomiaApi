#ifndef CGMICWIDGETSHARPENTONES_HPP
#define CGMICWIDGETSHARPENTONES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenTones.hpp"

class CGmicWidgetSharpenTones : public CProtocolTaskWidget
{
    public:

        CGmicWidgetSharpenTones(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenTones(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenTonesParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenTonesParam>();

            auto pSpinAmount = addDoubleSpin(0, tr("Amount"), m_pParam->m_amount, 0, 4, 0.1);
            auto pSpinCentre = addSpin(1, tr("Centre"), m_pParam->m_centre, 0, 255);

            auto pComboValues = addCombo(2, tr("Values"));
            pComboValues->addItem(tr("Cut"), CGmicSharpenTonesParam::CUT);
            pComboValues->addItem(tr("Normalize luma"), CGmicSharpenTonesParam::NORMALYZE);
            pComboValues->setCurrentIndex(pComboValues->findData(m_pParam->m_values));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_amount = pSpinAmount->value();
                m_pParam->m_centre = pSpinCentre->value();
                m_pParam->m_values = pComboValues->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSharpenTonesParam> m_pParam = nullptr;
};

class CGmicWidgetSharpenTonesFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenTonesFactory()
        {
            m_name = QObject::tr("Sharpen [Tones]").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenTones>(pParam);
        }
};

#endif // CGMICWIDGETSHARPENTONES_HPP
