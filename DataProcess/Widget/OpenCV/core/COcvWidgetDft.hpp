#ifndef COCVWIDGETDFT_HPP
#define COCVWIDGETDFT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvDft.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDft : public CProtocolTaskWidget
{
    public:

        COcvWidgetDft(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDft(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDftParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDftParam>();

            auto pLabelFlags = new QLabel(tr("Flags"));
            auto pComboFlags = new QComboBox;
            pComboFlags->addItem("Default", 0);
            pComboFlags->addItem("DFT_INVERSE", cv::DFT_INVERSE);
            pComboFlags->addItem("DFT_SCALE", cv::DFT_SCALE);
            pComboFlags->addItem("DFT_ROWS", cv::DFT_ROWS);
            pComboFlags->addItem("DFT_COMPLEX_OUTPUT", cv::DFT_COMPLEX_OUTPUT);
            pComboFlags->addItem("DFT_REAL_OUTPUT", cv::DFT_REAL_OUTPUT);
            pComboFlags->addItem("DFT_COMPLEX_INPUT", cv::DFT_COMPLEX_INPUT);

            pComboFlags->setCurrentIndex(pComboFlags->findData(m_pParam->m_flags));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelFlags, 0, 0);
            m_pLayout->addWidget(pComboFlags, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvDftParam> m_pParam = nullptr;
};

class COcvWidgetDftFactory : public CWidgetFactory
{
    public:

        COcvWidgetDftFactory()
        {
            m_name = QObject::tr("Discrete Fourier Transform").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetDft>(pParam);
        }
};

#endif // COCVWIDGETDFT_HPP
