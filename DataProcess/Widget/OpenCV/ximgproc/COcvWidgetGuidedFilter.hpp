#ifndef COCVWIDGETGUIDEDFILTER_HPP
#define COCVWIDGETGUIDEDFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGuidedFilter.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>

class COcvWidgetGuidedFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetGuidedFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetGuidedFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGuidedFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGuidedFilterParam>();

            auto pSpinRadius = addSpin(0, tr("Radius"), m_pParam->m_radius);
            auto pSpinEps = addDoubleSpin(1, tr("Epsilon"), m_pParam->m_eps, 0, 1, 0.1);
            auto pComboDepth = addCombo(2, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_8S", CV_8S);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_16S", CV_16S);
            pComboDepth->addItem("CV_32S", CV_32S);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_ddepth));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_radius = pSpinRadius->value();
                m_pParam->m_eps = pSpinEps->value();
                m_pParam->m_ddepth = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvGuidedFilterParam> m_pParam = nullptr;
};

class COcvWidgetGuidedFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetGuidedFilterFactory()
        {
            m_name = QObject::tr("Guided Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGuidedFilter>(pParam);
        }
};
#endif // COCVWIDGETGUIDEDFILTER_HPP
