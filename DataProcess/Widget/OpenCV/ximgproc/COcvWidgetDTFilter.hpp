#ifndef COCVWIDGETDTFILTER_HPP
#define COCVWIDGETDTFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvDTFilter.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDTFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetDTFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDTFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDTFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDTFilterParam>();

            auto pSpinColor = addDoubleSpin(0, tr("Sigma color"), m_pParam->m_sigmaColor, 0, DBL_MAX, 0.1);
            auto pSpinSpatial = addDoubleSpin(1, tr("Sigma spatial"), m_pParam->m_sigmaSpatial);
            auto pSpinIter = addSpin(2, tr("Iterations"), m_pParam->m_numIters);
            auto pCombo = addCombo(3, tr("Mode"));
            pCombo->addItem("DTF_NC", cv::ximgproc::DTF_NC);
            pCombo->addItem("DTF_IC", cv::ximgproc::DTF_IC);
            pCombo->addItem("DTF_RF", cv::ximgproc::DTF_RF);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_mode));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigmaColor = pSpinColor->value();
                m_pParam->m_sigmaSpatial = pSpinSpatial->value();
                m_pParam->m_numIters = pSpinIter->value();
                m_pParam->m_mode = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvDTFilterParam> m_pParam = nullptr;
};

class COcvWidgetDTFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetDTFilterFactory()
        {
            m_name = QObject::tr("DTFilter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDTFilter>(pParam);
        }
};
#endif // COCVWIDGETDTFILTER_HPP
