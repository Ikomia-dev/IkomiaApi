#ifndef CGMICWIDGETAUTOBALANCE_HPP
#define CGMICWIDGETAUTOBALANCE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicAutoBalance.hpp"

class CGmicWidgetAutoBalance : public CProtocolTaskWidget
{
    public:

        CGmicWidgetAutoBalance(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetAutoBalance(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicAutoBalanceParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicAutoBalanceParam>();

            auto pSpinArea = addSpin(0, tr("Area"), m_pParam->m_area);
            auto pSpinSmooth = addDoubleSpin(1, tr("Smooth"), m_pParam->m_smooth);

            auto pComboColorSpace = addCombo(2, tr("Color space"));
            pComboColorSpace->addItem("HSI", CGmicAutoBalanceParam::HSI);
            pComboColorSpace->addItem("HSV", CGmicAutoBalanceParam::HSV);
            pComboColorSpace->addItem("Lab", CGmicAutoBalanceParam::LAB);
            pComboColorSpace->addItem("Linear RGB", CGmicAutoBalanceParam::LINEAR_RGB);
            pComboColorSpace->addItem("RGB", CGmicAutoBalanceParam::RGB);
            pComboColorSpace->addItem("YCrCb", CGmicAutoBalanceParam::YCBCR);
            pComboColorSpace->setCurrentIndex(pComboColorSpace->findData(m_pParam->m_colorSpace));

            auto pCheckBalance = addCheck(3, "sRGB balance", m_pParam->m_bBalance);
            auto pCheckReduceRAM = addCheck(4, tr("Reduce RAM"), m_pParam->m_bReduceRAM);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_area = pSpinArea->value();
                m_pParam->m_smooth = pSpinSmooth->value();
                m_pParam->m_colorSpace = pComboColorSpace->currentData().toInt();
                m_pParam->m_bBalance = pCheckBalance->isChecked();
                m_pParam->m_bReduceRAM = pCheckReduceRAM->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicAutoBalanceParam> m_pParam = nullptr;
};

class CGmicWidgetAutoBalanceFactory : public CWidgetFactory
{
    public:

        CGmicWidgetAutoBalanceFactory()
        {
            m_name = QObject::tr("Auto balance").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetAutoBalance>(pParam);
        }
};

#endif // CGMICWIDGETAUTOBALANCE_HPP
