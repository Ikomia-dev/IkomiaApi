#ifndef CGMICWIDGETDISTANCETRANSFORM_HPP
#define CGMICWIDGETDISTANCETRANSFORM_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicDistanceTransform.hpp"

class CGmicWidgetDistanceTransform : public CProtocolTaskWidget
{
    public:

        CGmicWidgetDistanceTransform(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetDistanceTransform(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicDistanceTransformParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicDistanceTransformParam>();

            auto pSpinValue = addSpin(0, tr("Value"), m_pParam->m_value, 0, 5, 0.1);

            auto pComboMetric = addCombo(1, tr("Metric"));
            pComboMetric->addItem(tr("Chebyshev"), CGmicDistanceTransformParam::CHEBYSHEV);
            pComboMetric->addItem(tr("Manhattan"), CGmicDistanceTransformParam::MANHATTAN);
            pComboMetric->addItem(tr("Euclidean"), CGmicDistanceTransformParam::EUCLIDEAN);
            pComboMetric->addItem(tr("Square-euclidean"), CGmicDistanceTransformParam::SQUARE_EUCLIDEAN);
            pComboMetric->setCurrentIndex(pComboMetric->findData(m_pParam->m_metric));

            auto pComboNorm = addCombo(2, tr("Normalization"));
            pComboNorm->addItem(tr("Cut"), CGmicDistanceTransformParam::CUT);
            pComboNorm->addItem(tr("Normalize"), CGmicDistanceTransformParam::NORMALIZE);
            pComboNorm->addItem(tr("Modulo"), CGmicDistanceTransformParam::MODULO);
            pComboMetric->setCurrentIndex(pComboMetric->findData(m_pParam->m_normalization));

            auto pSpinModulo = addSpin(0, tr("Modulo"), m_pParam->m_modulo, 1, 255);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_value = pSpinValue->value();
                m_pParam->m_metric = pComboMetric->currentData().toInt();
                m_pParam->m_normalization = pComboNorm->currentData().toInt();
                m_pParam->m_modulo = pSpinModulo->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicDistanceTransformParam> m_pParam = nullptr;
};

class CGmicWidgetDistanceTransformFactory : public CWidgetFactory
{
    public:

        CGmicWidgetDistanceTransformFactory()
        {
            m_name = QObject::tr("Distance transform").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetDistanceTransform>(pParam);
        }
};

#endif // CGMICWIDGETDISTANCETRANSFORM_HPP
