#ifndef COCVWIDGETTHINNING_HPP
#define COCVWIDGETTHINNING_HPP

#include "Process/OpenCV/ximgproc/COcvThinning.hpp"
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetThinning : public CProtocolTaskWidget
{
    public:

        COcvWidgetThinning(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetThinning(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvThinningParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvThinningParam>();

            auto pLabel = new QLabel(tr("Type"));
            auto pCombo = new QComboBox;
            pCombo->addItem("Zhang-Suen", cv::ximgproc::THINNING_ZHANGSUEN);
            pCombo->addItem("Guo-Hall", cv::ximgproc::THINNING_GUOHALL);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_type));

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pCombo, 0, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_type = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvThinningParam> m_pParam = nullptr;
};

class COcvWidgetThinningFactory : public CWidgetFactory
{
    public:

        COcvWidgetThinningFactory()
        {
            m_name = QObject::tr("Thinning").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetThinning>(pParam);
        }
};

#endif // COCVWIDGETTHINNING_HPP
