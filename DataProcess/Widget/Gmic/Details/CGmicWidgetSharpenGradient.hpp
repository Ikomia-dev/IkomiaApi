#ifndef CGMICWIDGETSHARPENGRADIENT_HPP
#define CGMICWIDGETSHARPENGRADIENT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenGradient.hpp"

class CGmicWidgetSharpenGradient : public CProtocolTaskWidget
{
    public:

        CGmicWidgetSharpenGradient(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenGradient(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenGradientParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenGradientParam>();

            auto pSpinAmount = addDoubleSpin(0, tr("Amount"), m_pParam->m_amount, 0, 2, 0.1);
            auto pSpinScale = addDoubleSpin(1, tr("Scale"), m_pParam->m_scale, 0.1, 2, 0.1);

            auto pComboValues = addCombo(2, tr("Values"));
            pComboValues->addItem(tr("Cut"), CGmicSharpenGradientParam::CUT);
            pComboValues->addItem(tr("Normalize luma"), CGmicSharpenGradientParam::NORMALYZE);
            pComboValues->setCurrentIndex(pComboValues->findData(m_pParam->m_values));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_amount = pSpinAmount->value();
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_values = pComboValues->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSharpenGradientParam> m_pParam = nullptr;
};

class CGmicWidgetSharpenGradientFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenGradientFactory()
        {
            m_name = QObject::tr("Sharpen [Gradient]").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenGradient>(pParam);
        }
};

#endif // CGMICWIDGETSHARPENGRADIENT_HPP
