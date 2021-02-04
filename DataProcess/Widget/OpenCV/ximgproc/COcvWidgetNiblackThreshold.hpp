#ifndef COCVWIDGETNIBLACKTHRESHOLD_HPP
#define COCVWIDGETNIBLACKTHRESHOLD_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvNiblackThreshold.hpp"

class COcvWidgetNiblackThreshold : public CProtocolTaskWidget
{
    public:

        COcvWidgetNiblackThreshold(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetNiblackThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvNiblackThresholdParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvNiblackThresholdParam>();

            auto pLabelMethod = new QLabel(QObject::tr("Binarization method"));
            auto pComboMethod = new QComboBox;
            pComboMethod->addItem(tr("Niblack"), cv::ximgproc::BINARIZATION_NIBLACK);
            pComboMethod->addItem(tr("Sauvola"), cv::ximgproc::BINARIZATION_SAUVOLA);
            pComboMethod->addItem(tr("Wolf"), cv::ximgproc::BINARIZATION_WOLF);
            pComboMethod->addItem(tr("Nick"), cv::ximgproc::BINARIZATION_NICK);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_binaryMethod));

            auto pLabelThreshType = new QLabel(QObject::tr("Threshold type"));
            auto pComboThreshType = new QComboBox;
            pComboThreshType->addItem(tr("Binary"), cv::THRESH_BINARY);
            pComboThreshType->addItem(tr("Inverse binary"), cv::THRESH_BINARY_INV);
            pComboThreshType->setCurrentIndex(pComboMethod->findData(m_pParam->m_thresholdType));

            auto pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            auto pSpinBlockSize = new QDoubleSpinBox;
            pSpinBlockSize->setSingleStep(2);
            pSpinBlockSize->setRange(1, INT_MAX - 1);
            pSpinBlockSize->setValue(m_pParam->m_blockSize);

            auto pLabelK = new QLabel(QObject::tr("K"));
            auto pSpinK = new QDoubleSpinBox;
            pSpinK->setValue(m_pParam->m_k);
            pSpinK->setRange(-1.0, 1.0);
            pSpinK->setSingleStep(0.1);

            
            m_pLayout->addWidget(pLabelMethod, 0, 0);
            m_pLayout->addWidget(pComboMethod, 0, 1);
            m_pLayout->addWidget(pLabelThreshType, 1, 0);
            m_pLayout->addWidget(pComboThreshType, 1, 1);
            m_pLayout->addWidget(pLabelBlockSize, 2, 0);
            m_pLayout->addWidget(pSpinBlockSize, 2, 1);
            m_pLayout->addWidget(pLabelK, 3, 0);
            m_pLayout->addWidget(pSpinK, 3, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_binaryMethod = pComboMethod->currentData().toInt();
                m_pParam->m_thresholdType = pComboThreshType->currentData().toInt();
                m_pParam->m_blockSize = pSpinBlockSize->value();
                m_pParam->m_k = pSpinK->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvNiblackThresholdParam> m_pParam = nullptr;
};

class COcvWidgetNiblackThresholdFactory : public CWidgetFactory
{
    public:

        COcvWidgetNiblackThresholdFactory()
        {
            m_name = QObject::tr("Niblack Threshold").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetNiblackThreshold>(pParam);
        }
};

#endif // COCVWIDGETNIBLACKTHRESHOLD_HPP
