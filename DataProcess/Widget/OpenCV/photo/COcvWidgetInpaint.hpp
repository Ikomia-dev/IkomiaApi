#ifndef COCVWIDGETINPAINT_HPP
#define COCVWIDGETINPAINT_HPP

#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "Process/OpenCV/photo/COcvInpaint.hpp"

class COcvWidgetInpaint : public CProtocolTaskWidget
{
    public:

        COcvWidgetInpaint(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetInpaint(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInpaintParam>(pParam);;
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInpaintParam>();

            QLabel* pLabelRadius = new QLabel(QObject::tr("Neighborhood diameter"));
            m_pSpinRadius = new QSpinBox;
            m_pSpinRadius->setMinimum(1);
            m_pSpinRadius->setMaximum(256);
            m_pSpinRadius->setValue(m_pParam->m_radius);

            QLabel* pLabelMethod = new QLabel(QObject::tr("Method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem("Navier-Stokes");
            m_pComboMethod->addItem("Telea");

            if(m_pParam->m_method == cv::INPAINT_NS)
                m_pComboMethod->setCurrentIndex(0);
            else
                m_pComboMethod->setCurrentIndex(0);

            
            m_pLayout->addWidget(pLabelRadius, 0, 0);
            m_pLayout->addWidget(m_pSpinRadius, 0, 1);
            m_pLayout->addWidget(pLabelMethod, 1, 0);
            m_pLayout->addWidget(m_pComboMethod, 1, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_radius = m_pSpinRadius->value();
                m_pParam->m_method = m_pComboMethod->currentIndex();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvInpaintParam>   m_pParam = nullptr;
        QSpinBox*                           m_pSpinRadius = nullptr;
        QComboBox*                          m_pComboMethod = nullptr;
};

class COcvWidgetInpaintFactory : public CWidgetFactory
{
    public:

        COcvWidgetInpaintFactory()
        {
            m_name = QObject::tr("Inpaint filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInpaint>(pParam);
        }
};

#endif // COCVWIDGETINPAINT_HPP
