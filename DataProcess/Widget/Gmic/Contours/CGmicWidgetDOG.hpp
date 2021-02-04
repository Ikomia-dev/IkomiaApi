#ifndef CGMICWIDGETDOG_HPP
#define CGMICWIDGETDOG_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicDOG.hpp"

class CGmicWidgetDoG : public CProtocolTaskWidget
{
    public:

        CGmicWidgetDoG(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDoG(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDoGParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDoGParam>();

            auto pSpinVariance1 = addDoubleSpin(0, tr("1st variance"), m_pParam->m_variance1, 0, 5, 0.1);
            auto pSpinVariance2 = addDoubleSpin(1, tr("2nd variance"), m_pParam->m_variance2, 0, 5, 0.1);
            auto pSpinThreshold = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold, 0, 49, 0.5);
            auto pCheckNegative = addCheck(3, tr("Negative colors"), m_pParam->m_bNegativeColors);
            auto pCheckMonochrome = addCheck(4, tr("Monochrome"), m_pParam->m_bMonochrome);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_variance1 = pSpinVariance1->value();
                m_pParam->m_variance2 = pSpinVariance2->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                m_pParam->m_bNegativeColors = pCheckNegative->isChecked();
                m_pParam->m_bMonochrome = pCheckMonochrome->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicDoGParam> m_pParam = nullptr;
};

class CGmicWidgetDoGFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDoGFactory()
        {
            m_name = QObject::tr("Difference of Gaussians").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDoG>(pParam);
        }
};

#endif // CGMICWIDGETDOG_HPP
