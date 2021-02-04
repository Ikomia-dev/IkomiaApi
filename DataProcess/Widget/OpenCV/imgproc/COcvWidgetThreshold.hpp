#ifndef COCVWIDGETTHRESHOLD_HPP
#define COCVWIDGETTHRESHOLD_HPP

#include <QComboBox>
#include <QDoubleSpinBox>
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvThreshold.hpp"

class COcvWidgetThreshold : public CProtocolTaskWidget
{
    public:

        COcvWidgetThreshold(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetThreshold(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvThresholdParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvThresholdParam>();

            auto pComboMethod = addCombo(0, tr("Threshold method"));
            pComboMethod->addItem(tr("Fixed value"), 0);
            pComboMethod->addItem(tr("Otsu"), cv::THRESH_OTSU);
            pComboMethod->addItem(tr("Triangle"), cv::THRESH_TRIANGLE);

            if(m_pParam->m_thresholdType & cv::THRESH_OTSU)
                pComboMethod->setCurrentIndex(1);
            else if(m_pParam->m_thresholdType & cv::THRESH_TRIANGLE)
                pComboMethod->setCurrentIndex(2);
            else
                pComboMethod->setCurrentIndex(0);

            auto pComboType = addCombo(1, tr("Threshold type"));
            pComboType->addItem(tr("Binary"), cv::THRESH_BINARY);
            pComboType->addItem(tr("Inverse binary"), cv::THRESH_BINARY_INV);
            pComboType->addItem(tr("Truncate"), cv::THRESH_TRUNC);
            pComboType->addItem(tr("To zero"), cv::THRESH_TOZERO);
            pComboType->addItem(tr("To zero inverse"), cv::THRESH_TOZERO_INV);

            if(m_pParam->m_thresholdType == cv::THRESH_BINARY || m_pParam->m_thresholdType == cv::THRESH_OTSU || m_pParam->m_thresholdType == cv::THRESH_TRIANGLE)
                pComboType->setCurrentIndex(0);
            else if(m_pParam->m_thresholdType & cv::THRESH_BINARY_INV)
                pComboType->setCurrentIndex(1);
            else if(m_pParam->m_thresholdType & cv::THRESH_TRUNC)
                pComboType->setCurrentIndex(2);
            else if(m_pParam->m_thresholdType & cv::THRESH_TOZERO)
                pComboType->setCurrentIndex(3);
            else
                pComboType->setCurrentIndex(4);

            auto pSpinValue = addDoubleSpin(2, tr("Threshold value"), m_pParam->m_threshold);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_thresholdType = pComboMethod->currentData().toInt() | pComboType->currentData().toInt();
                m_pParam->m_threshold = pSpinValue->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvThresholdParam> m_pParam = nullptr;
};

class COcvWidgetThresholdFactory : public CWidgetFactory
{
    public:

        COcvWidgetThresholdFactory()
        {
            m_name = QObject::tr("Threshold").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetThreshold>(pParam);
        }
};

#endif // COCVWIDGETTHRESHOLD_HPP
