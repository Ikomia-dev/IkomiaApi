#ifndef COCVWIDGETFLIP_HPP
#define COCVWIDGETFLIP_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvFlip.hpp"
#include <QRadioButton>

class COcvWidgetFlip : public CProtocolTaskWidget
{
    public:

        COcvWidgetFlip(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFlip(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFlipParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFlipParam>();

            m_pVerticalBtn = new QRadioButton(tr("Vertical"));
            m_pVerticalBtn->setChecked(true);
            m_pHorizontalBtn = new QRadioButton(tr("Horizontal"));
            m_pBothBtn = new QRadioButton(tr("Both"));

            
            m_pLayout->addWidget(m_pVerticalBtn, 0, 0, 1, 1);
            m_pLayout->addWidget(m_pHorizontalBtn, 1, 0, 1, 1);
            m_pLayout->addWidget(m_pBothBtn, 2, 0, 1, 1);
            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pVerticalBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = 0;
            });
            connect(m_pHorizontalBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = 1;
            });
            connect(m_pBothBtn, &QRadioButton::toggled, [&](bool checked)
            {
                if(checked == true)
                    m_pParam->m_flipCode = -1;
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFlipParam>  m_pParam = nullptr;
        QRadioButton*                   m_pVerticalBtn = nullptr;
        QRadioButton*                   m_pHorizontalBtn = nullptr;
        QRadioButton*                   m_pBothBtn = nullptr;
};

class COcvWidgetFlipFactory : public CWidgetFactory
{
    public:

        COcvWidgetFlipFactory()
        {
            m_name = QObject::tr("Flip").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFlip>(pParam);
        }
};

#endif // COCVWIDGETFLIP_HPP
