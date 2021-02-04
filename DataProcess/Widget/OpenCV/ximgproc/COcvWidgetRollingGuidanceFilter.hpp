#ifndef COCVWIDGETROLLINGGUIDANCEFILTER_HPP
#define COCVWIDGETROLLINGGUIDANCEFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvRollingGuidanceFilter.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetRollingGuidanceFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetRollingGuidanceFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetRollingGuidanceFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRollingGuidanceFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRollingGuidanceFilterParam>();

            QLabel* pLabelSpin = new QLabel(QObject::tr("Filter size d"));
            m_pSpin = new QSpinBox;
            m_pSpin->setRange(-1, 100);
            m_pSpin->setValue(m_pParam->m_d);

            QLabel* pLabelSpinIter = new QLabel(QObject::tr("Iterations"));
            m_pDblSpinIter = new QDoubleSpinBox;
            m_pDblSpinIter->setValue(m_pParam->m_numOfIter);
            m_pDblSpinIter->setRange(0, 1000);

            QLabel* pLabelSpinColor = new QLabel(QObject::tr("Sigma Color"));
            m_pDblSpinColor = new QDoubleSpinBox;
            m_pDblSpinColor->setRange(0, 100);
            m_pDblSpinColor->setValue(m_pParam->m_sigmaColor);

            QLabel* pLabelSpinSpace = new QLabel(QObject::tr("Sigma Space"));
            m_pDblSpinSpace = new QDoubleSpinBox;
            m_pDblSpinSpace->setRange(0, 100);
            m_pDblSpinSpace->setValue(m_pParam->m_sigmaSpace);

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_d = m_pSpin->value();
                m_pParam->m_numOfIter = m_pDblSpinIter->value();
                m_pParam->m_sigmaColor = m_pDblSpinColor->value();
                m_pParam->m_sigmaSpace = m_pDblSpinSpace->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);

            m_pLayout->addWidget(pLabelSpinIter, 1, 0);
            m_pLayout->addWidget(m_pDblSpinIter, 1, 1);

            m_pLayout->addWidget(pLabelSpinColor, 2, 0);
            m_pLayout->addWidget(m_pDblSpinColor, 2, 1);

            m_pLayout->addWidget(pLabelSpinSpace, 3, 0);
            m_pLayout->addWidget(m_pDblSpinSpace, 3, 1);

            
        }

    private:

        std::shared_ptr<COcvRollingGuidanceFilterParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
        QDoubleSpinBox*                     m_pDblSpinIter = nullptr;
        QDoubleSpinBox*                     m_pDblSpinColor = nullptr;
        QDoubleSpinBox*                     m_pDblSpinSpace = nullptr;
};

class COcvWidgetRollingGuidanceFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetRollingGuidanceFilterFactory()
        {
            m_name = QObject::tr("Rolling guidance filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRollingGuidanceFilter>(pParam);
        }
};

#endif // COCVWIDGETROLLINGGUIDANCEFILTER_HPP
