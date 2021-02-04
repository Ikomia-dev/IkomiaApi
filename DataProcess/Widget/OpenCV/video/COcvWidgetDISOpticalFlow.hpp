#ifndef COCVWIDGETDISOPTICALFLOW_HPP
#define COCVWIDGETDISOPTICALFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvDISOpticalFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDISOF : public CProtocolTaskWidget
{
    public:

        COcvWidgetDISOF(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDISOF(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDISOFParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDISOFParam>();

            auto pCombo = addCombo(0, tr("Preset mode"));
            pCombo->addItem("PRESET_MEDIUM", cv::DISOpticalFlow::PRESET_MEDIUM);
            pCombo->addItem("PRESET_FAST", cv::DISOpticalFlow::PRESET_FAST);
            pCombo->addItem("PRESET_ULTRAFAST", cv::DISOpticalFlow::PRESET_ULTRAFAST);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_preset));
            auto pCheck = addCheck(1, tr("Use OpenCL"), m_pParam->m_bUseOCL);
            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_preset = pCombo->currentData().toInt();
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
            
        }

    private:

        std::shared_ptr<COcvDISOFParam> m_pParam = nullptr;
        //QSpinBox*                           m_pSpin = nullptr;
};

class COcvWidgetDISOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetDISOFFactory()
        {
            m_name = QObject::tr("DIS Optical Flow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDISOF>(pParam);
        }
};
#endif // COCVWIDGETDISOPTICALFLOW_HPP
