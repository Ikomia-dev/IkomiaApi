#ifndef COCVWIDGETCOLORMAP_HPP
#define COCVWIDGETCOLORMAP_HPP

#include "Process/OpenCV/imgproc/COcvColorMap.hpp"
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetColorMap : public CProtocolTaskWidget
{
    public:

        COcvWidgetColorMap(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetColorMap(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvColorMapParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvColorMapParam>();

            auto pLabel = new QLabel(tr("Colormap type"));
            auto pCombo = new QComboBox;
            pCombo->addItem("Autumn", cv::COLORMAP_AUTUMN);
            pCombo->addItem("Bone", cv::COLORMAP_BONE);
            pCombo->addItem("Jet", cv::COLORMAP_JET);
            pCombo->addItem("Winter", cv::COLORMAP_WINTER);
            pCombo->addItem("Rainbow", cv::COLORMAP_RAINBOW);
            pCombo->addItem("Ocean", cv::COLORMAP_OCEAN);
            pCombo->addItem("Summer", cv::COLORMAP_SUMMER);
            pCombo->addItem("Spring", cv::COLORMAP_SPRING);
            pCombo->addItem("Cool", cv::COLORMAP_COOL);
            pCombo->addItem("Hsv", cv::COLORMAP_HSV);
            pCombo->addItem("Pink", cv::COLORMAP_PINK);
            pCombo->addItem("Hot", cv::COLORMAP_HOT);
            pCombo->addItem("Parula", cv::COLORMAP_PARULA);
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

        std::shared_ptr<COcvColorMapParam> m_pParam = nullptr;
};

class COcvWidgetColorMapFactory : public CWidgetFactory
{
    public:

        COcvWidgetColorMapFactory()
        {
            m_name = QObject::tr("Colormap").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetColorMap>(pParam);
        }
};

#endif // COCVWIDGETCOLORMAP_HPP
