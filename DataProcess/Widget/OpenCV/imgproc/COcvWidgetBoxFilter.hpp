#ifndef COCVWIDGETBOXFILTER_HPP
#define COCVWIDGETBOXFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvBoxFilter.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetBoxFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetBoxFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBoxFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBoxFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBoxFilterParam>();

            m_pSpin = new QSpinBox;
            m_pSpin->setValue(m_pParam->m_ksize.width);
            QLabel* pLabelSpin = new QLabel(tr("Filter size d"));

            auto pCheckNormalize = new QCheckBox("Normalize");
            pCheckNormalize->setChecked(m_pParam->m_bNormalize);

            auto pLabelBorder = new QLabel(tr("Border type"));
            auto pComboBorder = new QComboBox;
            pComboBorder->addItem("Default", cv::BORDER_DEFAULT);
            pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_borderType));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_ksize = cv::Size(m_pSpin->value(), m_pSpin->value());
                m_pParam->m_bNormalize = pCheckNormalize->isChecked();
                m_pParam->m_borderType = pComboBorder->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });


            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);
            m_pLayout->addWidget(pCheckNormalize, 1, 0);
            m_pLayout->addWidget(pLabelBorder, 2, 0);
            m_pLayout->addWidget(pComboBorder, 2, 1);

        }

    private:

        std::shared_ptr<COcvBoxFilterParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
};

class COcvWidgetBoxFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetBoxFilterFactory()
        {
            m_name = QObject::tr("Box Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBoxFilter>(pParam);
        }
};

#endif // COCVWIDGETBOXFILTER_HPP
