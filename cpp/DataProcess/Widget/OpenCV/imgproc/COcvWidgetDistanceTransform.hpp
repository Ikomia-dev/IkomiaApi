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

#ifndef COCVWIDGETDISTANCETRANSFORM_HPP
#define COCVWIDGETDISTANCETRANSFORM_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvDistanceTransform.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDistanceTransform : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDistanceTransform(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDistanceTransform(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDistanceTransformParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDistanceTransformParam>();

            auto pCheckVoronoi = new QCheckBox(tr("Voronoï output"));
            pCheckVoronoi->setCheckable(true);
            pCheckVoronoi->setChecked(m_pParam->m_bVoronoi);

            auto pLabelType = new QLabel(tr("Distance type"));
            m_pComboType = new QComboBox;
            m_pComboType->addItem("C", cv::DIST_C);
            m_pComboType->addItem("L1", cv::DIST_L1);
            m_pComboType->addItem("L2", cv::DIST_L2);
            m_pComboType->addItem("L12", cv::DIST_L12);
            m_pComboType->addItem("FAIR", cv::DIST_FAIR);
            m_pComboType->addItem("HUBER", cv::DIST_HUBER);
            m_pComboType->addItem("WELSCH", cv::DIST_WELSCH);
            m_pComboType->setCurrentIndex(m_pComboType->findData(m_pParam->m_type));

            auto pLabelMaskSize = new QLabel(tr("Mask size"));
            m_pComboMaskSize = new QComboBox;
            m_pComboMaskSize->addItem("3x3", cv::DIST_MASK_3);
            m_pComboMaskSize->addItem("5x5", cv::DIST_MASK_5);
            m_pComboMaskSize->addItem(tr("Precise"), cv::DIST_MASK_PRECISE);
            m_pComboMaskSize->setCurrentIndex(m_pComboMaskSize->findData(m_pParam->m_maskSize));

            auto pLabelLabelType = new QLabel(tr("Label type"));
            m_pComboLabelType = new QComboBox;
            m_pComboLabelType->addItem("Nearest connected component", cv::DIST_LABEL_CCOMP);
            m_pComboLabelType->addItem("Nearest zero pixel", cv::DIST_LABEL_PIXEL);
            m_pComboLabelType->setCurrentIndex(m_pComboLabelType->findData(m_pParam->m_labelType));
            pLabelLabelType->setEnabled(m_pParam->m_bVoronoi);
            m_pComboLabelType->setEnabled(m_pParam->m_bVoronoi);

            m_pLayout->addWidget(pCheckVoronoi, 0, 0, 1, 2);
            m_pLayout->addWidget(pLabelType, 1, 0);
            m_pLayout->addWidget(m_pComboType, 1, 1);
            m_pLayout->addWidget(pLabelMaskSize, 2, 0);
            m_pLayout->addWidget(m_pComboMaskSize, 2, 1);
            m_pLayout->addWidget(pLabelLabelType, 3, 0);
            m_pLayout->addWidget(m_pComboLabelType, 3, 1);
            
            connect(pCheckVoronoi, &QCheckBox::toggled, [&](bool bChecked)
            {
                m_pParam->m_bVoronoi = bChecked;
                pLabelLabelType->setEnabled(bChecked);
                m_pComboLabelType->setEnabled(bChecked);
            });
        }

        void onApply() override
        {
            m_pParam->m_type = m_pComboType->currentData().toInt();
            m_pParam->m_maskSize = m_pComboMaskSize->currentData().toInt();
            m_pParam->m_labelType = m_pComboLabelType->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDistanceTransformParam> m_pParam = nullptr;
        QComboBox*  m_pComboType = nullptr;
        QComboBox*  m_pComboMaskSize = nullptr;
        QComboBox*  m_pComboLabelType = nullptr;
};

class COcvWidgetDistanceTransformFactory : public CWidgetFactory
{
    public:

        COcvWidgetDistanceTransformFactory()
        {
            m_name = QObject::tr("Distance Transform").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetDistanceTransform>(pParam);
        }
};

#endif // COCVWIDGETDISTANCETRANSFORM_HPP
