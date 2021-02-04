#ifndef CGMICWIDGETDEBLUR_HPP
#define CGMICWIDGETDEBLUR_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Details/CGmicSharpenDeblur.hpp"

class CGmicWidgetSharpenDeblur : public CProtocolTaskWidget
{
    public:

        CGmicWidgetSharpenDeblur(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSharpenDeblur(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSharpenDeblurParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSharpenDeblurParam>();

            auto pSpinRadius = addDoubleSpin(0, tr("Radius"), m_pParam->m_radius, 0, 20);
            auto pSpinIteration = addSpin(1, tr("Iterations"), m_pParam->m_iteration, 0, 100);
            auto pSpinTimeStep = addDoubleSpin(2, tr("Time step"), m_pParam->m_timeStep, 0, 50);
            auto pSpinSmoothness = addDoubleSpin(3, tr("Smoothness"), m_pParam->m_smoothness, 0, 10);
            auto pSpinOverlap = addSpin(1, tr("Iterations"), m_pParam->m_spatialOverlap, 0, 256);

            auto pComboRegularization = addCombo(4, tr("Regularization"));
            pComboRegularization->addItem(tr("Tikhonov"), CGmicSharpenDeblurParam::TIKHONOV);
            pComboRegularization->addItem(tr("Mean curvature"), CGmicSharpenDeblurParam::MEAN_CURVATURE);
            pComboRegularization->addItem("Total variation", CGmicSharpenDeblurParam::TV);
            pComboRegularization->setCurrentIndex(pComboRegularization->findData(m_pParam->m_regularization));

            auto pComboChannels = addCombo(5, tr("Channels"));
            for(size_t i=0; i<m_pParam->m_channels.size(); ++i)
                pComboChannels->addItem(QString::fromStdString(m_pParam->m_channels[i]));

            pComboChannels->setCurrentIndex(m_pParam->m_channel);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_radius = pSpinRadius->value();
                m_pParam->m_iteration = pSpinIteration->value();
                m_pParam->m_timeStep = pSpinTimeStep->value();
                m_pParam->m_smoothness = pSpinSmoothness->value();
                m_pParam->m_spatialOverlap = pSpinOverlap->value();
                m_pParam->m_regularization = pComboRegularization->currentData().toInt();
                m_pParam->m_channel = pComboChannels->currentIndex();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSharpenDeblurParam> m_pParam = nullptr;
};

class CGmicWidgetSharpenDeblurFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSharpenDeblurFactory()
        {
            m_name = QObject::tr("Sharpen [Deblur]").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSharpenDeblur>(pParam);
        }
};

#endif // CGMICWIDGETDEBLUR_HPP
