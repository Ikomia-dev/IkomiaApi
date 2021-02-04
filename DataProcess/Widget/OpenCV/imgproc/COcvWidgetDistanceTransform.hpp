#ifndef COCVWIDGETDISTANCETRANSFORM_HPP
#define COCVWIDGETDISTANCETRANSFORM_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvDistanceTransform.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDistanceTransform : public CProtocolTaskWidget
{
    public:

        COcvWidgetDistanceTransform(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDistanceTransform(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDistanceTransformParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDistanceTransformParam>();

            auto pCheckVoronoi = new QCheckBox(tr("Voronoï output"));
            pCheckVoronoi->setCheckable(true);
            pCheckVoronoi->setChecked(m_pParam->m_bVoronoi);

            auto pLabelType = new QLabel(tr("Distance type"));
            auto pComboType = new QComboBox;
            pComboType->addItem("C", cv::DIST_C);
            pComboType->addItem("L1", cv::DIST_L1);
            pComboType->addItem("L2", cv::DIST_L2);
            pComboType->addItem("L12", cv::DIST_L12);
            pComboType->addItem("FAIR", cv::DIST_FAIR);
            pComboType->addItem("HUBER", cv::DIST_HUBER);
            pComboType->addItem("WELSCH", cv::DIST_WELSCH);
            pComboType->setCurrentIndex(pComboType->findData(m_pParam->m_type));

            auto pLabelMaskSize = new QLabel(tr("Mask size"));
            auto pComboMaskSize = new QComboBox;
            pComboMaskSize->addItem("3x3", cv::DIST_MASK_3);
            pComboMaskSize->addItem("5x5", cv::DIST_MASK_5);
            pComboMaskSize->addItem(tr("Precise"), cv::DIST_MASK_PRECISE);
            pComboMaskSize->setCurrentIndex(pComboMaskSize->findData(m_pParam->m_maskSize));

            auto pLabelLabelType = new QLabel(tr("Label type"));
            auto pComboLabelType = new QComboBox;
            pComboLabelType->addItem("Nearest connected component", cv::DIST_LABEL_CCOMP);
            pComboLabelType->addItem("Nearest zero pixel", cv::DIST_LABEL_PIXEL);
            pComboLabelType->setCurrentIndex(pComboLabelType->findData(m_pParam->m_labelType));
            pLabelLabelType->setEnabled(m_pParam->m_bVoronoi);
            pComboLabelType->setEnabled(m_pParam->m_bVoronoi);

            
            m_pLayout->addWidget(pCheckVoronoi, 0, 0, 1, 2);
            m_pLayout->addWidget(pLabelType, 1, 0);
            m_pLayout->addWidget(pComboType, 1, 1);
            m_pLayout->addWidget(pLabelMaskSize, 2, 0);
            m_pLayout->addWidget(pComboMaskSize, 2, 1);
            m_pLayout->addWidget(pLabelLabelType, 3, 0);
            m_pLayout->addWidget(pComboLabelType, 3, 1);
            

            connect(pCheckVoronoi, &QCheckBox::toggled, [=](bool bChecked)
            {
                m_pParam->m_bVoronoi = bChecked;
                pLabelLabelType->setEnabled(bChecked);
                pComboLabelType->setEnabled(bChecked);
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_type = pComboType->currentData().toInt();
                m_pParam->m_maskSize = pComboMaskSize->currentData().toInt();
                m_pParam->m_labelType = pComboLabelType->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvDistanceTransformParam> m_pParam = nullptr;
};

class COcvWidgetDistanceTransformFactory : public CWidgetFactory
{
    public:

        COcvWidgetDistanceTransformFactory()
        {
            m_name = QObject::tr("Distance Transform").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetDistanceTransform>(pParam);
        }
};

#endif // COCVWIDGETDISTANCETRANSFORM_HPP
