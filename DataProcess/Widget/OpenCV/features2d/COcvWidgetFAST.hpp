#ifndef COCVWIDGETFAST_HPP
#define COCVWIDGETFAST_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvFAST.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetFAST : public CProtocolTaskWidget
{
    public:

        COcvWidgetFAST(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFAST(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFASTParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFASTParam>();

            auto pSpinThresh = addSpin(0, tr("Threshold"), m_pParam->m_threshold);
            pSpinThresh->setRange(0, INT_MAX);
            auto pCombo = addCombo(1, tr("Type"));
            pCombo->addItem("TYPE_5_8", cv::FastFeatureDetector::TYPE_5_8);
            pCombo->addItem("TYPE_7_12", cv::FastFeatureDetector::TYPE_7_12);
            pCombo->addItem("TYPE_9_16", cv::FastFeatureDetector::TYPE_9_16);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_type));
            auto pCheck = addCheck(2, tr("Non maximum suppression"), m_pParam->m_bNonmaxSuppression);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_threshold = pSpinThresh->value();
                m_pParam->m_type = pCombo->currentData().toInt();
                m_pParam->m_bNonmaxSuppression = pCheck->isChecked();

                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFASTParam> m_pParam = nullptr;
};

class COcvWidgetFASTFactory : public CWidgetFactory
{
    public:

        COcvWidgetFASTFactory()
        {
            m_name = QObject::tr("FAST").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFAST>(pParam);
        }
};
#endif // COCVWIDGETFAST_HPP
