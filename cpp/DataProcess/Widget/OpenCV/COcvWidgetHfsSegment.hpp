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

#ifndef COCVWIDGETHFSSEGMENT_HPP
#define COCVWIDGETHFSSEGMENT_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/COcvHfsSegment.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>

class COcvWidgetHfsSegment : public CWorkflowTaskWidget
{
    public:

        COcvWidgetHfsSegment(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetHfsSegment(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHfsSegmentParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHfsSegmentParam>();

            auto pGroup1 = new QGroupBox(tr("Stage 1"));
            auto pGroup2 = new QGroupBox(tr("Stage 2"));
            auto pGroup3 = new QGroupBox(tr("Stage 3"));

            auto pLayout1 = new QGridLayout;
            auto pLayout2 = new QGridLayout;
            auto pLayout3 = new QGridLayout;

            pGroup1->setLayout(pLayout1);
            pGroup2->setLayout(pLayout2);
            pGroup3->setLayout(pLayout3);

            /* STAGE 1 */
            auto pLabelNumSlicIter = new QLabel(tr("NumSlicIter"));
            auto pSpinNumIter = new QSpinBox;

            pSpinNumIter->setValue(m_pParam->m_numSlicIter);
            pLayout1->addWidget(pLabelNumSlicIter, 0, 0);
            pLayout1->addWidget(pSpinNumIter, 0, 1);

            auto pLabelSlicS = new QLabel(tr("SlicSpixelSize"));
            auto pSpinSlicS = new QSpinBox;

            pSpinSlicS->setValue(m_pParam->m_slicSpixelSize);
            pLayout1->addWidget(pLabelSlicS, 1, 0);
            pLayout1->addWidget(pSpinSlicS, 1, 1);

            auto pLabelWeight = new QLabel(tr("Spatial weight"));
            auto pSpinWeight = new QDoubleSpinBox;

            pSpinWeight->setValue(m_pParam->m_spatialWeight);
            pLayout1->addWidget(pLabelWeight, 2, 0);
            pLayout1->addWidget(pSpinWeight, 2, 1);

            /* STAGE 2 */
            auto pLabelMinRegionSizeI =new QLabel(tr("MinRegionSizeI"));
            auto pSpinSizeI = new QSpinBox;

            pSpinSizeI->setRange(1, INT_MAX);
            pSpinSizeI->setValue(m_pParam->m_minRegionSizeI);
            pLayout2->addWidget(pLabelMinRegionSizeI, 0, 0);
            pLayout2->addWidget(pSpinSizeI, 0, 1);

            auto pLabelSegEgbThresholdI =new QLabel(tr("SegEgbThresholdI"));
            auto pSpinThresholdI = new QDoubleSpinBox;

            pSpinThresholdI->setValue(m_pParam->m_segEgbThresholdI);
            pSpinThresholdI->setSingleStep(0.1);
            pLayout2->addWidget(pLabelSegEgbThresholdI, 1, 0);
            pLayout2->addWidget(pSpinThresholdI, 1, 1);

            /* STAGE 3 */
            auto pLabelMinRegionSizeII =new QLabel(tr("MinRegionSizeII"));
            auto pSpinSizeII = new QSpinBox;

            pSpinSizeII->setRange(1, INT_MAX);
            pSpinSizeII->setValue(m_pParam->m_minRegionSizeII);
            pLayout3->addWidget(pLabelMinRegionSizeII, 0, 0);
            pLayout3->addWidget(pSpinSizeII, 0, 1);

            auto pLabelSegEgbThresholdII =new QLabel(tr("SegEgbThresholdII"));
            auto pSpinThresholdII = new QDoubleSpinBox;

            pSpinThresholdII->setValue(m_pParam->m_segEgbThresholdII);
            pSpinThresholdII->setSingleStep(0.1);
            pLayout3->addWidget(pLabelSegEgbThresholdII, 1, 0);
            pLayout3->addWidget(pSpinThresholdII, 1, 1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_minRegionSizeI = pSpinSizeI->value();
                m_pParam->m_minRegionSizeII = pSpinSizeII->value();
                m_pParam->m_numSlicIter = pSpinNumIter->value();
                m_pParam->m_segEgbThresholdI = pSpinThresholdI->value();
                m_pParam->m_segEgbThresholdII = pSpinThresholdII->value();
                m_pParam->m_slicSpixelSize = pSpinSlicS->value();
                m_pParam->m_spatialWeight = pSpinWeight->value();

                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pGroup1, 0, 0);
            m_pLayout->addWidget(pGroup2, 0, 1);
            m_pLayout->addWidget(pGroup3, 0, 2);

            
        }

    private:

        std::shared_ptr<COcvHfsSegmentParam> m_pParam = nullptr;
};

class COcvWidgetHfsSegmentFactory : public CWidgetFactory
{
    public:

        COcvWidgetHfsSegmentFactory()
        {
            m_name = QObject::tr("Hierarchical Feature Selection Segmentation").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetHfsSegment>(pParam);
        }
};

#endif // COCVWIDGETHFSSEGMENT_HPP
