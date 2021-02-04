#ifndef COCVWIDGETSIMPLEBLOBDETECTOR_HPP
#define COCVWIDGETSIMPLEBLOBDETECTOR_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvSimpleBlobDetector.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSimpleBlobDetector : public CProtocolTaskWidget
{
    public:

        COcvWidgetSimpleBlobDetector(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSimpleBlobDetector(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSimpleBlobDetectorParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSimpleBlobDetectorParam>();

            int count = 0;
            auto pSpinMinThresh = addDoubleSpin(count++, tr("Min threshold"), m_pParam->m_blobParam.minThreshold);
            pSpinMinThresh->setMaximum(INT_MAX);
            pSpinMinThresh->setValue(m_pParam->m_blobParam.minThreshold);
            auto pSpinMaxThresh = addDoubleSpin(count++, tr("Max threshold"), m_pParam->m_blobParam.maxThreshold);
            pSpinMaxThresh->setMaximum(INT_MAX);
            pSpinMaxThresh->setValue(m_pParam->m_blobParam.maxThreshold);
            auto pSpinMinDist = addDoubleSpin(count++, tr("Min distance between blobs"), m_pParam->m_blobParam.minDistBetweenBlobs);
            pSpinMinDist->setMaximum(INT_MAX);
            pSpinMinDist->setValue(m_pParam->m_blobParam.minDistBetweenBlobs);

            bool isChecked = m_pParam->m_blobParam.blobColor == 0 ? true : false;
            auto pRadioDark = addRadio(count++, tr("Dark blobs"), isChecked);
            addRadio(count++, tr("Bright objects"), !isChecked);

            /* AREA */
            auto pGroupArea = new QGroupBox(tr("Area"));
            pGroupArea->setCheckable(true);
            pGroupArea->setChecked(m_pParam->m_blobParam.filterByArea);

            auto pLayoutArea = new QGridLayout;
            auto pLabelAreaMin = new QLabel(tr("Min area"));
            auto pSpinAreaMin = new QDoubleSpinBox;
            pSpinAreaMin->setMaximum(INT_MAX);
            pSpinAreaMin->setValue(m_pParam->m_blobParam.minArea);
            auto pLabelAreaMax = new QLabel(tr("Max area"));
            auto pSpinAreaMax = new QDoubleSpinBox;
            pSpinAreaMax->setMaximum(INT_MAX);
            pSpinAreaMax->setValue(m_pParam->m_blobParam.maxArea);
            pLayoutArea->addWidget(pLabelAreaMin, 0, 0);
            pLayoutArea->addWidget(pSpinAreaMin, 0, 1);
            pLayoutArea->addWidget(pLabelAreaMax, 1, 0);
            pLayoutArea->addWidget(pSpinAreaMax, 1, 1);

            pGroupArea->setLayout(pLayoutArea);

            /* CIRCULARITY */
            auto pGroupCircle = new QGroupBox(tr("Circularity"));
            pGroupCircle->setCheckable(true);
            pGroupCircle->setChecked(m_pParam->m_blobParam.filterByCircularity);

            auto pLayoutCircle = new QGridLayout;
            auto pLabelCircleMin = new QLabel(tr("Min circularity"));
            auto pSpinCircleMin = new QDoubleSpinBox;
            pSpinCircleMin->setMaximum(INT_MAX);
            pSpinCircleMin->setValue(m_pParam->m_blobParam.minCircularity);
            auto pLabelCircleMax = new QLabel(tr("Max circularity"));
            auto pSpinCircleMax = new QDoubleSpinBox;
            pSpinCircleMax->setMaximum(INT_MAX);
            pSpinCircleMax->setValue(m_pParam->m_blobParam.maxCircularity);
            pLayoutCircle->addWidget(pLabelCircleMin, 0, 0);
            pLayoutCircle->addWidget(pSpinCircleMin, 0, 1);
            pLayoutCircle->addWidget(pLabelCircleMax, 1, 0);
            pLayoutCircle->addWidget(pSpinCircleMax, 1, 1);

            pGroupCircle->setLayout(pLayoutCircle);

            /* CONVEXITY */
            auto pGroupConvex = new QGroupBox(tr("Convexity"));
            pGroupConvex->setCheckable(true);
            pGroupConvex->setChecked(m_pParam->m_blobParam.filterByConvexity);

            auto pLayoutConvex = new QGridLayout;
            auto pLabelConvexMin = new QLabel(tr("Min convexity"));
            auto pSpinConvexMin = new QDoubleSpinBox;
            pSpinConvexMin->setMaximum(INT_MAX);
            pSpinConvexMin->setValue(m_pParam->m_blobParam.minConvexity);
            auto pLabelConvexMax = new QLabel(tr("Max convexity"));
            auto pSpinConvexMax = new QDoubleSpinBox;
            pSpinConvexMax->setMaximum(INT_MAX);
            pSpinConvexMax->setValue(m_pParam->m_blobParam.maxConvexity);
            pLayoutConvex->addWidget(pLabelConvexMin, 0, 0);
            pLayoutConvex->addWidget(pSpinConvexMin, 0, 1);
            pLayoutConvex->addWidget(pLabelConvexMax, 1, 0);
            pLayoutConvex->addWidget(pSpinConvexMax, 1, 1);

            pGroupConvex->setLayout(pLayoutConvex);

            /* INERTIA */
            auto pGroupInertia = new QGroupBox(tr("Inertia ratio"));
            pGroupInertia->setCheckable(true);
            pGroupInertia->setChecked(m_pParam->m_blobParam.filterByInertia);

            auto pLayoutInertia = new QGridLayout;
            auto pLabelInertiaMin = new QLabel(tr("Min inertia ratio"));
            auto pSpinInertiaMin = new QDoubleSpinBox;
            pSpinInertiaMin->setMaximum(INT_MAX);
            pSpinInertiaMin->setValue(m_pParam->m_blobParam.minInertiaRatio);
            auto pLabelInertiaMax = new QLabel(tr("Max inertia ratio"));
            auto pSpinInertiaMax = new QDoubleSpinBox;
            pSpinInertiaMax->setMaximum(INT_MAX);
            pSpinInertiaMax->setValue(m_pParam->m_blobParam.maxInertiaRatio);
            pLayoutInertia->addWidget(pLabelInertiaMin, 0, 0);
            pLayoutInertia->addWidget(pSpinInertiaMin, 0, 1);
            pLayoutInertia->addWidget(pLabelInertiaMax, 1, 0);
            pLayoutInertia->addWidget(pSpinInertiaMax, 1, 1);

            pGroupInertia->setLayout(pLayoutInertia);

            m_pLayout->addWidget(pGroupArea, count++, 0, 1, 2);
            m_pLayout->addWidget(pGroupCircle, count++, 0, 1, 2);
            m_pLayout->addWidget(pGroupConvex, count++, 0, 1, 2);
            m_pLayout->addWidget(pGroupInertia, count++, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_blobParam.minThreshold = pSpinMinThresh->value();
                m_pParam->m_blobParam.maxThreshold = pSpinMaxThresh->value();
                m_pParam->m_blobParam.minDistBetweenBlobs = pSpinMinDist->value();
                m_pParam->m_blobParam.blobColor = pRadioDark->isChecked() ? 0 : 255;

                m_pParam->m_blobParam.filterByArea = pGroupArea->isChecked();
                m_pParam->m_blobParam.minArea = pSpinAreaMin->value();
                m_pParam->m_blobParam.maxArea = pSpinAreaMax->value();

                m_pParam->m_blobParam.filterByConvexity = pGroupConvex->isChecked();
                m_pParam->m_blobParam.minConvexity = pSpinConvexMin->value();
                m_pParam->m_blobParam.maxConvexity = pSpinConvexMax->value();

                m_pParam->m_blobParam.filterByCircularity = pGroupCircle->isChecked();
                m_pParam->m_blobParam.minCircularity = pSpinCircleMin->value();
                m_pParam->m_blobParam.maxCircularity = pSpinCircleMax->value();

                m_pParam->m_blobParam.filterByInertia = pGroupInertia->isChecked();
                m_pParam->m_blobParam.minInertiaRatio = pSpinInertiaMin->value();
                m_pParam->m_blobParam.maxInertiaRatio = pSpinInertiaMax->value();

                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvSimpleBlobDetectorParam> m_pParam = nullptr;
};

class COcvWidgetSimpleBlobDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetSimpleBlobDetectorFactory()
        {
            m_name = QObject::tr("SimpleBlobDetector").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSimpleBlobDetector>(pParam);
        }
};
#endif // COCVWIDGETSIMPLEBLOBDETECTOR_HPP
