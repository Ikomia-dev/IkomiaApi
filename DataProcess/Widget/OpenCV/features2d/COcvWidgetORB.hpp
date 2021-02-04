#ifndef COCVWIDGETORB_HPP
#define COCVWIDGETORB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvORB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetORB : public CProtocolTaskWidget
{
    public:

        COcvWidgetORB(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetORB(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvORBParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvORBParam>();

            auto pSpinFeatures = addSpin(0, tr("Features"), m_pParam->m_nfeatures);
            pSpinFeatures->setRange(0, INT_MAX);
            pSpinFeatures->setValue(m_pParam->m_nfeatures);
            auto pSpinScale = addDoubleSpin(1, tr("Scale factor"), m_pParam->m_scaleFactor);
            pSpinScale->setSingleStep(0.1);
            auto pSpinLevels = addSpin(2, tr("Levels"), m_pParam->m_nlevels);
            auto pSpinEdgeThresh = addSpin(3, tr("Edge threshold"), m_pParam->m_edgeThreshold);
            auto pSpinFirstLevel = addSpin(2, tr("First level"), m_pParam->m_firstLevel);
            auto pSpinWTA = addSpin(4, tr("WTA_K"), m_pParam->m_WTA_K);
            auto pCombo = addCombo(5, tr("Score Type"));
            pCombo->addItem("HARRIS_SCORE", cv::ORB::HARRIS_SCORE);
            pCombo->addItem("FAST_SCORE", cv::ORB::FAST_SCORE);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_scoreType));
            auto pSpinPatch = addSpin(6, tr("Patch size"), m_pParam->m_patchSize);
            auto pSpinFastThresh = addSpin(7, tr("Fast threshold"), m_pParam->m_fastThreshold);
            auto pCheck = addCheck(8, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            auto pCheckDetect = addCheck(9, tr("Detect"), m_pParam->m_bDetect);
            pCheckDetect->setEnabled(false);
            auto pCheckCompute = addCheck(10, tr("Compute"), m_pParam->m_bCompute);

            connect(pCheck, &QCheckBox::clicked, [=](bool checked){
                if(checked)
                {
                    pCheckDetect->setEnabled(true);
                }
                else
                {
                    pCheckDetect->setChecked(true);
                    pCheckDetect->setEnabled(false);
                }
            });
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_nfeatures = pSpinFeatures->value();
                m_pParam->m_scaleFactor = pSpinScale->value();
                m_pParam->m_nlevels = pSpinLevels->value();
                m_pParam->m_edgeThreshold = pSpinEdgeThresh->value();
                m_pParam->m_firstLevel = pSpinFirstLevel->value();
                m_pParam->m_WTA_K = pSpinWTA->value();
                m_pParam->m_scoreType = pCombo->currentData().toInt();
                m_pParam->m_patchSize = pSpinPatch->value();
                m_pParam->m_fastThreshold = pSpinFastThresh->value();
                m_pParam->m_bUseProvidedKeypoints = pCheck->isChecked();
                m_pParam->m_bDetect = pCheckDetect->isChecked();
                m_pParam->m_bCompute = pCheckCompute->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvORBParam> m_pParam = nullptr;
};

class COcvWidgetORBFactory : public CWidgetFactory
{
    public:

        COcvWidgetORBFactory()
        {
            m_name = QObject::tr("ORB").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetORB>(pParam);
        }
};
#endif // COCVWIDGETORB_HPP
