// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef COCVWIDGETSIMPLEBLOBDETECTOR_HPP
#define COCVWIDGETSIMPLEBLOBDETECTOR_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvSimpleBlobDetector.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSimpleBlobDetector : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSimpleBlobDetector(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSimpleBlobDetector(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSimpleBlobDetectorParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSimpleBlobDetectorParam>();

            int count = 0;
            m_pSpinMinThresh = addDoubleSpin(count++, tr("Min threshold"), m_pParam->m_blobParam.minThreshold);
            m_pSpinMaxThresh = addDoubleSpin(count++, tr("Max threshold"), m_pParam->m_blobParam.maxThreshold);
            m_pSpinMinDist = addDoubleSpin(count++, tr("Min distance between blobs"), m_pParam->m_blobParam.minDistBetweenBlobs);

            bool isChecked = m_pParam->m_blobParam.blobColor == 0 ? true : false;
            m_pRadioDark = addRadio(count++, tr("Dark blobs"), isChecked);
            addRadio(count++, tr("Bright objects"), !isChecked);

            /* AREA */
            m_pGroupArea = new QGroupBox(tr("Area"));
            m_pGroupArea->setCheckable(true);
            m_pGroupArea->setChecked(m_pParam->m_blobParam.filterByArea);

            auto pLayoutArea = new QGridLayout;
            auto pLabelAreaMin = new QLabel(tr("Min area"));
            m_pSpinAreaMin = new QDoubleSpinBox;
            m_pSpinAreaMin->setMaximum(INT_MAX);
            m_pSpinAreaMin->setValue(m_pParam->m_blobParam.minArea);
            auto pLabelAreaMax = new QLabel(tr("Max area"));
            m_pSpinAreaMax = new QDoubleSpinBox;
            m_pSpinAreaMax->setMaximum(INT_MAX);
            m_pSpinAreaMax->setValue(m_pParam->m_blobParam.maxArea);
            pLayoutArea->addWidget(pLabelAreaMin, 0, 0);
            pLayoutArea->addWidget(m_pSpinAreaMin, 0, 1);
            pLayoutArea->addWidget(pLabelAreaMax, 1, 0);
            pLayoutArea->addWidget(m_pSpinAreaMax, 1, 1);

            m_pGroupArea->setLayout(pLayoutArea);

            /* CIRCULARITY */
            m_pGroupCircle = new QGroupBox(tr("Circularity"));
            m_pGroupCircle->setCheckable(true);
            m_pGroupCircle->setChecked(m_pParam->m_blobParam.filterByCircularity);

            auto pLayoutCircle = new QGridLayout;
            auto pLabelCircleMin = new QLabel(tr("Min circularity"));
            m_pSpinCircleMin = new QDoubleSpinBox;
            m_pSpinCircleMin->setMaximum(INT_MAX);
            m_pSpinCircleMin->setValue(m_pParam->m_blobParam.minCircularity);
            auto pLabelCircleMax = new QLabel(tr("Max circularity"));
            m_pSpinCircleMax = new QDoubleSpinBox;
            m_pSpinCircleMax->setMaximum(INT_MAX);
            m_pSpinCircleMax->setValue(m_pParam->m_blobParam.maxCircularity);
            pLayoutCircle->addWidget(pLabelCircleMin, 0, 0);
            pLayoutCircle->addWidget(m_pSpinCircleMin, 0, 1);
            pLayoutCircle->addWidget(pLabelCircleMax, 1, 0);
            pLayoutCircle->addWidget(m_pSpinCircleMax, 1, 1);

            m_pGroupCircle->setLayout(pLayoutCircle);

            /* CONVEXITY */
            m_pGroupConvex = new QGroupBox(tr("Convexity"));
            m_pGroupConvex->setCheckable(true);
            m_pGroupConvex->setChecked(m_pParam->m_blobParam.filterByConvexity);

            auto pLayoutConvex = new QGridLayout;
            auto pLabelConvexMin = new QLabel(tr("Min convexity"));
            m_pSpinConvexMin = new QDoubleSpinBox;
            m_pSpinConvexMin->setMaximum(INT_MAX);
            m_pSpinConvexMin->setValue(m_pParam->m_blobParam.minConvexity);
            auto pLabelConvexMax = new QLabel(tr("Max convexity"));
            m_pSpinConvexMax = new QDoubleSpinBox;
            m_pSpinConvexMax->setMaximum(INT_MAX);
            m_pSpinConvexMax->setValue(m_pParam->m_blobParam.maxConvexity);
            pLayoutConvex->addWidget(pLabelConvexMin, 0, 0);
            pLayoutConvex->addWidget(m_pSpinConvexMin, 0, 1);
            pLayoutConvex->addWidget(pLabelConvexMax, 1, 0);
            pLayoutConvex->addWidget(m_pSpinConvexMax, 1, 1);

            m_pGroupConvex->setLayout(pLayoutConvex);

            /* INERTIA */
            m_pGroupInertia = new QGroupBox(tr("Inertia ratio"));
            m_pGroupInertia->setCheckable(true);
            m_pGroupInertia->setChecked(m_pParam->m_blobParam.filterByInertia);

            auto pLayoutInertia = new QGridLayout;
            auto pLabelInertiaMin = new QLabel(tr("Min inertia ratio"));
            m_pSpinInertiaMin = new QDoubleSpinBox;
            m_pSpinInertiaMin->setMaximum(INT_MAX);
            m_pSpinInertiaMin->setValue(m_pParam->m_blobParam.minInertiaRatio);
            auto pLabelInertiaMax = new QLabel(tr("Max inertia ratio"));
            m_pSpinInertiaMax = new QDoubleSpinBox;
            m_pSpinInertiaMax->setMaximum(INT_MAX);
            m_pSpinInertiaMax->setValue(m_pParam->m_blobParam.maxInertiaRatio);
            pLayoutInertia->addWidget(pLabelInertiaMin, 0, 0);
            pLayoutInertia->addWidget(m_pSpinInertiaMin, 0, 1);
            pLayoutInertia->addWidget(pLabelInertiaMax, 1, 0);
            pLayoutInertia->addWidget(m_pSpinInertiaMax, 1, 1);

            m_pGroupInertia->setLayout(pLayoutInertia);

            m_pLayout->addWidget(m_pGroupArea, count++, 0, 1, 2);
            m_pLayout->addWidget(m_pGroupCircle, count++, 0, 1, 2);
            m_pLayout->addWidget(m_pGroupConvex, count++, 0, 1, 2);
            m_pLayout->addWidget(m_pGroupInertia, count++, 0, 1, 2);
        }

        void onApply() override
        {
            m_pParam->m_blobParam.minThreshold = m_pSpinMinThresh->value();
            m_pParam->m_blobParam.maxThreshold = m_pSpinMaxThresh->value();
            m_pParam->m_blobParam.minDistBetweenBlobs = m_pSpinMinDist->value();
            m_pParam->m_blobParam.blobColor = m_pRadioDark->isChecked() ? 0 : 255;

            m_pParam->m_blobParam.filterByArea = m_pGroupArea->isChecked();
            m_pParam->m_blobParam.minArea = m_pSpinAreaMin->value();
            m_pParam->m_blobParam.maxArea = m_pSpinAreaMax->value();

            m_pParam->m_blobParam.filterByConvexity = m_pGroupConvex->isChecked();
            m_pParam->m_blobParam.minConvexity = m_pSpinConvexMin->value();
            m_pParam->m_blobParam.maxConvexity = m_pSpinConvexMax->value();

            m_pParam->m_blobParam.filterByCircularity = m_pGroupCircle->isChecked();
            m_pParam->m_blobParam.minCircularity = m_pSpinCircleMin->value();
            m_pParam->m_blobParam.maxCircularity = m_pSpinCircleMax->value();

            m_pParam->m_blobParam.filterByInertia = m_pGroupInertia->isChecked();
            m_pParam->m_blobParam.minInertiaRatio = m_pSpinInertiaMin->value();
            m_pParam->m_blobParam.maxInertiaRatio = m_pSpinInertiaMax->value();

            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSimpleBlobDetectorParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinMinThresh = nullptr;
        QDoubleSpinBox* m_pSpinMaxThresh = nullptr;
        QDoubleSpinBox* m_pSpinMinDist = nullptr;
        QDoubleSpinBox* m_pSpinAreaMin = nullptr;
        QDoubleSpinBox* m_pSpinAreaMax = nullptr;
        QDoubleSpinBox* m_pSpinConvexMin = nullptr;
        QDoubleSpinBox* m_pSpinConvexMax = nullptr;
        QDoubleSpinBox* m_pSpinCircleMin = nullptr;
        QDoubleSpinBox* m_pSpinCircleMax = nullptr;
        QDoubleSpinBox* m_pSpinInertiaMin = nullptr;
        QDoubleSpinBox* m_pSpinInertiaMax = nullptr;
        QRadioButton*   m_pRadioDark = nullptr;
        QGroupBox*      m_pGroupArea = nullptr;
        QGroupBox*      m_pGroupConvex = nullptr;
        QGroupBox*      m_pGroupCircle = nullptr;
        QGroupBox*      m_pGroupInertia = nullptr;
};

class COcvWidgetSimpleBlobDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetSimpleBlobDetectorFactory()
        {
            m_name = QObject::tr("SimpleBlobDetector").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSimpleBlobDetector>(pParam);
        }
};
#endif // COCVWIDGETSIMPLEBLOBDETECTOR_HPP
