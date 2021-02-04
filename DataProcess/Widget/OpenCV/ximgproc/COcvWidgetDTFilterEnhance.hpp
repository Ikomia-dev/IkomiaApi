#ifndef COCVWIDGETDTFILTERENHANCE_HPP
#define COCVWIDGETDTFILTERENHANCE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvDTFilterEnhance.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDTFilterEnhance : public CProtocolTaskWidget
{
    public:

        COcvWidgetDTFilterEnhance(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDTFilterEnhance(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDTFilterEnhanceParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDTFilterEnhanceParam>();

            auto pSpinColor = addDoubleSpin(0, tr("Sigma color"), m_pParam->m_sigmaColor, 0, DBL_MAX, 0.1);
            auto pSpinSpatial = addDoubleSpin(1, tr("Sigma spatial"), m_pParam->m_sigmaSpatial);
            auto pSpinContrast = addSpin(2, tr("Contrast"), m_pParam->m_contrastBase);
            auto pSpinDetails = addSpin(3, tr("Sigmoid coefficient"), m_pParam->m_detailsLevel);
            auto pSpinLayers = addSpin(4, tr("Layers"), m_pParam->m_numLayer, 1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigmaColor = pSpinColor->value();
                m_pParam->m_sigmaSpatial = pSpinSpatial->value();
                m_pParam->m_contrastBase = pSpinContrast->value();
                m_pParam->m_detailsLevel = pSpinDetails->value();
                m_pParam->m_numLayer = pSpinLayers->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvDTFilterEnhanceParam> m_pParam = nullptr;
};

class COcvWidgetDTFilterEnhanceFactory : public CWidgetFactory
{
    public:

        COcvWidgetDTFilterEnhanceFactory()
        {
            m_name = QObject::tr("DTFilterEnhance").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDTFilterEnhance>(pParam);
        }
};
#endif // COCVWIDGETDTFILTERENHANCE_HPP
