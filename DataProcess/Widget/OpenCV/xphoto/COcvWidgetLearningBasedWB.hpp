#ifndef COCVWIDGETLEARNINGBASEDWB_HPP
#define COCVWIDGETLEARNINGBASEDWB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/xphoto/COcvLearningBasedWB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetLearningBasedWB : public CProtocolTaskWidget
{
    public:

        COcvWidgetLearningBasedWB(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetLearningBasedWB(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvLearningBasedWBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvLearningBasedWBParam>();

            auto pSpinBin = addSpin(0, tr("Hist bin number"), m_pParam->m_histBinNum);
            auto pSpinMax = addSpin(1, tr("Max value"), m_pParam->m_rangeMaxVal);
            auto pSpinThresh = addDoubleSpin(2, tr("Saturation threshold"), m_pParam->m_satThreshold, 0, 1, 0.1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_histBinNum = pSpinBin->value();
                m_pParam->m_rangeMaxVal = pSpinMax->value();
                m_pParam->m_satThreshold = pSpinThresh->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvLearningBasedWBParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pDblSpinSat = nullptr;
};

class COcvWidgetLearningBasedWBFactory : public CWidgetFactory
{
    public:

        COcvWidgetLearningBasedWBFactory()
        {
            m_name = QObject::tr("LearningBasedWB").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLearningBasedWB>(pParam);
        }
};
#endif // COCVWIDGETLEARNINGBASEDWB_HPP
