#ifndef COCVWIDGETADAPTIVETHRESHOLD_HPP
#define COCVWIDGETADAPTIVETHRESHOLD_HPP

#include <QComboBox>
#include <QDoubleSpinBox>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvAdaptiveThreshold.hpp"

class COcvWidgetAdaptiveThreshold : public CProtocolTaskWidget
{
    public:

        COcvWidgetAdaptiveThreshold(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetAdaptiveThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAdaptiveThresholdParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAdaptiveThresholdParam>();

            QLabel* pLabelMethod = new QLabel(QObject::tr("Adaptive method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem(tr("Mean"));
            m_pComboMethod->addItem(tr("Gaussian"));

            if(m_pParam->m_adaptiveMethod ==  cv::ADAPTIVE_THRESH_MEAN_C)
                m_pComboMethod->setCurrentIndex(0);
            else
                m_pComboMethod->setCurrentIndex(1);

            QLabel* pLabelThreshType = new QLabel(QObject::tr("Threshold type"));
            m_pComboThreshType = new QComboBox;
            m_pComboThreshType->addItem(tr("Binary"));
            m_pComboThreshType->addItem(tr("Inverse binary"));

            if(m_pParam->m_thresholdType ==  cv::THRESH_BINARY)
                m_pComboThreshType->setCurrentIndex(0);
            else
                m_pComboThreshType->setCurrentIndex(1);

            QLabel* pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            m_pSpinBlockSize = new QDoubleSpinBox;
            m_pSpinBlockSize->setSingleStep(2);
            m_pSpinBlockSize->setRange(1, INT_MAX - 1);
            m_pSpinBlockSize->setValue(m_pParam->m_blockSize);

            QLabel* pLabelOffset = new QLabel(QObject::tr("Offset"));
            m_pSpinOffset = new QDoubleSpinBox;
            m_pSpinOffset->setValue(m_pParam->m_offset);
            m_pSpinOffset->setRange(-255, 255);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_adaptiveMethod = m_pComboMethod->currentIndex() == 0 ? cv::ADAPTIVE_THRESH_MEAN_C : cv::ADAPTIVE_THRESH_GAUSSIAN_C;
                m_pParam->m_thresholdType = m_pComboThreshType->currentIndex() == 0 ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
                m_pParam->m_blockSize = m_pSpinBlockSize->value();
                m_pParam->m_offset = m_pSpinOffset->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelMethod, 0, 0);
            m_pLayout->addWidget(m_pComboMethod, 0, 1);
            m_pLayout->addWidget(pLabelThreshType, 1, 0);
            m_pLayout->addWidget(m_pComboThreshType, 1, 1);
            m_pLayout->addWidget(pLabelBlockSize, 2, 0);
            m_pLayout->addWidget(m_pSpinBlockSize, 2, 1);
            m_pLayout->addWidget(pLabelOffset, 3, 0);
            m_pLayout->addWidget(m_pSpinOffset, 3, 1);
            
        }

    private:

        std::shared_ptr<COcvAdaptiveThresholdParam> m_pParam = nullptr;
        QComboBox*      m_pComboMethod = nullptr;
        QComboBox*      m_pComboThreshType = nullptr;
        QDoubleSpinBox* m_pSpinBlockSize = nullptr;
        QDoubleSpinBox* m_pSpinOffset = nullptr;
};

class COcvWidgetAdaptiveThresholdFactory : public CWidgetFactory
{
    public:

        COcvWidgetAdaptiveThresholdFactory()
        {
            m_name = QObject::tr("Adaptive Threshold").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAdaptiveThreshold>(pParam);
        }
};

#endif // COCVWIDGETADAPTIVETHRESHOLD_HPP
