#ifndef COCVWIDGETROTATE_HPP
#define COCVWIDGETROTATE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvRotate.hpp"
#include <QComboBox>

class COcvWidgetRotate : public CProtocolTaskWidget
{
    public:

        COcvWidgetRotate(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetRotate(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRotateParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRotateParam>();

            auto pLabelAngle = new QLabel(tr("Angle"));
            m_pComboAngle = new QComboBox;
            m_pComboAngle->addItem(tr("90°"), cv::ROTATE_90_CLOCKWISE);
            m_pComboAngle->addItem(tr("180°"), cv::ROTATE_180);
            m_pComboAngle->addItem(tr("270°"), cv::ROTATE_90_COUNTERCLOCKWISE);
            m_pComboAngle->setCurrentIndex(m_pComboAngle->findData(m_pParam->m_rotateCode));

            
            m_pLayout->addWidget(pLabelAngle, 0, 0);
            m_pLayout->addWidget(m_pComboAngle, 0, 1);
            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                m_pParam->m_rotateCode = m_pComboAngle->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvRotateParam>    m_pParam = nullptr;
        QComboBox*                          m_pComboAngle = nullptr;
};

class COcvWidgetRotateFactory : public CWidgetFactory
{
    public:

        COcvWidgetRotateFactory()
        {
            m_name = QObject::tr("Rotate").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRotate>(pParam);
        }
};

#endif // COCVWIDGETROTATE_HPP
