#ifndef COCVWIDGETDTFILTERSTYLIZE_HPP
#define COCVWIDGETDTFILTERSTYLIZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvDTFilterStylize.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDTFilterStylize : public CProtocolTaskWidget
{
    public:

        COcvWidgetDTFilterStylize(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDTFilterStylize(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDTFilterStylizeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDTFilterStylizeParam>();

            auto pSpinColor = addDoubleSpin(0, tr("Sigma color"), m_pParam->m_sigmaColor, 0, DBL_MAX, 0.1);
            auto pSpinSpatial = addDoubleSpin(1, tr("Sigma spatial"), m_pParam->m_sigmaSpatial);
            auto pSpinEdges = addSpin(2, tr("Edges gamma"), m_pParam->m_edgesGamma);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigmaColor = pSpinColor->value();
                m_pParam->m_sigmaSpatial = pSpinSpatial->value();
                m_pParam->m_edgesGamma = pSpinEdges->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvDTFilterStylizeParam> m_pParam = nullptr;
};

class COcvWidgetDTFilterStylizeFactory : public CWidgetFactory
{
    public:

        COcvWidgetDTFilterStylizeFactory()
        {
            m_name = QObject::tr("DTFilterStylize").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDTFilterStylize>(pParam);
        }
};
#endif // COCVWIDGETDTFILTERSTYLIZE_HPP
