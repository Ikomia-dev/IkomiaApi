#ifndef COCVWIDGETFASTNLMEANS_HPP
#define COCVWIDGETFASTNLMEANS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/photo/COcvFastNlMeans.hpp"

class COcvWidgetFastNlMeans : public CProtocolTaskWidget
{
    public:

        COcvWidgetFastNlMeans(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetFastNlMeans(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFastNlMeansParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFastNlMeansParam>();

            QLabel* pLabelH = new QLabel(QObject::tr("Filter strength"));
            m_pSpinH = new QDoubleSpinBox;
            m_pSpinH->setValue(m_pParam->m_h);
            m_pSpinH->setSingleStep(0.5);
            m_pSpinH->setRange(0, INT_MAX - 1);

            QLabel* pLabelBlockSize = new QLabel(QObject::tr("Block size"));
            m_pSpinBlockSize = new QDoubleSpinBox;
            m_pSpinBlockSize->setValue(m_pParam->m_blockSize);
            m_pSpinBlockSize->setSingleStep(2);
            m_pSpinBlockSize->setRange(1, INT_MAX - 1);

            QLabel* pLabelSearchSize = new QLabel(QObject::tr("Search size"));
            m_pSpinSearchSize = new QDoubleSpinBox;
            m_pSpinSearchSize->setValue(m_pParam->m_searchSize);
            m_pSpinSearchSize->setSingleStep(2);
            m_pSpinSearchSize->setRange(1, INT_MAX - 1);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_h = m_pSpinH->value();
                m_pParam->m_blockSize = m_pSpinBlockSize->value();
                m_pParam->m_searchSize = m_pSpinSearchSize->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelH, 0, 0);
            m_pLayout->addWidget(m_pSpinH, 0, 1);
            m_pLayout->addWidget(pLabelBlockSize, 1, 0);
            m_pLayout->addWidget(m_pSpinBlockSize, 1, 1);
            m_pLayout->addWidget(pLabelSearchSize, 2, 0);
            m_pLayout->addWidget(m_pSpinSearchSize, 2, 1);
            
        }

    private:

        std::shared_ptr<COcvFastNlMeansParam>   m_pParam = nullptr;
        QDoubleSpinBox*                         m_pSpinH = nullptr;
        QDoubleSpinBox*                         m_pSpinBlockSize = nullptr;
        QDoubleSpinBox*                         m_pSpinSearchSize = nullptr;
};

class COcvWidgetFastNlMeansFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastNlMeansFactory()
        {
            m_name = QObject::tr("Non Local Means Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastNlMeans>(pParam);
        }
};

#endif // COCVWIDGETFASTNLMEANS_HPP
