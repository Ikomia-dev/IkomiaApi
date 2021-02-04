#ifndef COCVWIDGETNORMALIZE_HPP
#define COCVWIDGETNORMALIZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvNormalize.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetNormalize : public CProtocolTaskWidget
{
    public:

        COcvWidgetNormalize(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetNormalize(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvNormalizeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvNormalizeParam>();

            auto pSpinAlpha = addDoubleSpin(0, tr("Alpha"), m_pParam->m_alpha);
            auto pSpinBeta = addDoubleSpin(1, tr("Beta"), m_pParam->m_beta);
            auto pCombo = addCombo(2, tr("Norm type"));
            pCombo->addItem(tr("NORM_INF"), cv::NORM_INF);
            pCombo->addItem(tr("NORM_L1"), cv::NORM_L1);
            pCombo->addItem(tr("NORM_L2"), cv::NORM_L2);
            pCombo->addItem(tr("NORM_L2SQR"), cv::NORM_L2SQR);
            pCombo->addItem(tr("NORM_HAMMING"), cv::NORM_HAMMING);
            pCombo->addItem(tr("NORM_HAMMING2"), cv::NORM_HAMMING2);
            pCombo->addItem(tr("NORM_TYPE_MASK"), cv::NORM_TYPE_MASK);
            pCombo->addItem(tr("NORM_MINMAX"), cv::NORM_MINMAX);
            pCombo->addItem(tr("NORM_RELATIVE"), cv::NORM_RELATIVE);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_norm_type));

            auto pComboDepth = addCombo(3, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_beta = pSpinBeta->value();
                m_pParam->m_norm_type = pCombo->currentData().toInt();
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvNormalizeParam> m_pParam = nullptr;
};

class COcvWidgetNormalizeFactory : public CWidgetFactory
{
    public:

        COcvWidgetNormalizeFactory()
        {
            m_name = QObject::tr("Normalize").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetNormalize>(pParam);
        }
};
#endif // COCVWIDGETNORMALIZE_HPP
