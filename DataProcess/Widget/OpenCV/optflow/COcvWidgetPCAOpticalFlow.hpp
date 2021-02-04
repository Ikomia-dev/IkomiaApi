#ifndef COCVWIDGETPCAOPTICALFLOW_HPP
#define COCVWIDGETPCAOPTICALFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvPCAOpticalFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetPCAOF : public CProtocolTaskWidget
{
    public:

        COcvWidgetPCAOF(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetPCAOF(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvPCAOFParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvPCAOFParam>();

            auto pSpinSizeW = addSpin(0, tr("Basis width"), m_pParam->m_basisSize.width);
            auto pSpinSizeH = addSpin(1, tr("Basis height"), m_pParam->m_basisSize.height);
            auto pSpinRate = addDoubleSpin(2, tr("Sparse rate"), m_pParam->m_sparseRate, 0, 1, 0.001, 3);
            auto pSpinCorners = addDoubleSpin(3, tr("Corners fraction"), m_pParam->m_retainedCornersFraction, 0, 1, 0.1, 1);
            auto pSpinOccTh = addDoubleSpin(4, tr("Occlusions threshold"), m_pParam->m_occlusionsThreshold, 0, 1, 0.0001, 4);
            auto pSpinDamp = addDoubleSpin(5, tr("Damping factor"), m_pParam->m_dampingFactor, 0, 1, 0.00001, 5);
            auto pSpinClahe = addSpin(6, tr("Clahe clip"), m_pParam->m_claheClip);
            auto pCheck = addCheck(7, tr("Use OpenCL"), m_pParam->m_bUseOCL);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_basisSize = cv::Size(pSpinSizeW->value(), pSpinSizeH->value());
                m_pParam->m_sparseRate = pSpinRate->value();
                m_pParam->m_retainedCornersFraction = pSpinCorners->value();
                m_pParam->m_occlusionsThreshold = pSpinOccTh->value();
                m_pParam->m_dampingFactor = pSpinDamp->value();
                m_pParam->m_claheClip = pSpinClahe->value();
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvPCAOFParam> m_pParam = nullptr;
};

class COcvWidgetPCAOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetPCAOFFactory()
        {
            m_name = QObject::tr("PCA Optical Flow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetPCAOF>(pParam);
        }
};
#endif // COCVWIDGETPCAOPTICALFLOW_HPP
