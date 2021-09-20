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

        void init()
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
            m_pSpinNumIter = new QSpinBox;
            m_pSpinNumIter->setValue(m_pParam->m_numSlicIter);
            pLayout1->addWidget(pLabelNumSlicIter, 0, 0);
            pLayout1->addWidget(m_pSpinNumIter, 0, 1);

            auto pLabelSlicS = new QLabel(tr("SlicSpixelSize"));
            m_pSpinSlicS = new QSpinBox;
            m_pSpinSlicS->setValue(m_pParam->m_slicSpixelSize);
            pLayout1->addWidget(pLabelSlicS, 1, 0);
            pLayout1->addWidget(m_pSpinSlicS, 1, 1);

            auto pLabelWeight = new QLabel(tr("Spatial weight"));
            m_pSpinWeight = new QDoubleSpinBox;
            m_pSpinWeight->setValue(m_pParam->m_spatialWeight);
            pLayout1->addWidget(pLabelWeight, 2, 0);
            pLayout1->addWidget(m_pSpinWeight, 2, 1);

            /* STAGE 2 */
            auto pLabelMinRegionSizeI =new QLabel(tr("MinRegionSizeI"));
            m_pSpinSizeI = new QSpinBox;
            m_pSpinSizeI->setRange(1, INT_MAX);
            m_pSpinSizeI->setValue(m_pParam->m_minRegionSizeI);
            pLayout2->addWidget(pLabelMinRegionSizeI, 0, 0);
            pLayout2->addWidget(m_pSpinSizeI, 0, 1);

            auto pLabelSegEgbThresholdI =new QLabel(tr("SegEgbThresholdI"));
            m_pSpinThresholdI = new QDoubleSpinBox;
            m_pSpinThresholdI->setValue(m_pParam->m_segEgbThresholdI);
            m_pSpinThresholdI->setSingleStep(0.1);
            pLayout2->addWidget(pLabelSegEgbThresholdI, 1, 0);
            pLayout2->addWidget(m_pSpinThresholdI, 1, 1);

            /* STAGE 3 */
            auto pLabelMinRegionSizeII =new QLabel(tr("MinRegionSizeII"));
            m_pSpinSizeII = new QSpinBox;
            m_pSpinSizeII->setRange(1, INT_MAX);
            m_pSpinSizeII->setValue(m_pParam->m_minRegionSizeII);
            pLayout3->addWidget(pLabelMinRegionSizeII, 0, 0);
            pLayout3->addWidget(m_pSpinSizeII, 0, 1);

            auto pLabelSegEgbThresholdII =new QLabel(tr("SegEgbThresholdII"));
            m_pSpinThresholdII = new QDoubleSpinBox;
            m_pSpinThresholdII->setValue(m_pParam->m_segEgbThresholdII);
            m_pSpinThresholdII->setSingleStep(0.1);
            pLayout3->addWidget(pLabelSegEgbThresholdII, 1, 0);
            pLayout3->addWidget(m_pSpinThresholdII, 1, 1);

            m_pLayout->addWidget(pGroup1, 0, 0);
            m_pLayout->addWidget(pGroup2, 0, 1);
            m_pLayout->addWidget(pGroup3, 0, 2);
        }

        void onApply() override
        {
            m_pParam->m_minRegionSizeI = m_pSpinSizeI->value();
            m_pParam->m_minRegionSizeII = m_pSpinSizeII->value();
            m_pParam->m_numSlicIter = m_pSpinNumIter->value();
            m_pParam->m_segEgbThresholdI = m_pSpinThresholdI->value();
            m_pParam->m_segEgbThresholdII = m_pSpinThresholdII->value();
            m_pParam->m_slicSpixelSize = m_pSpinSlicS->value();
            m_pParam->m_spatialWeight = m_pSpinWeight->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvHfsSegmentParam>    m_pParam = nullptr;
        QSpinBox*                               m_pSpinSizeI = nullptr;
        QSpinBox*                               m_pSpinSizeII = nullptr;
        QSpinBox*                               m_pSpinNumIter = nullptr;
        QDoubleSpinBox*                         m_pSpinThresholdI = nullptr;
        QDoubleSpinBox*                         m_pSpinThresholdII = nullptr;
        QSpinBox*                               m_pSpinSlicS = nullptr;
        QDoubleSpinBox*                         m_pSpinWeight = nullptr;
};

class COcvWidgetHfsSegmentFactory : public CWidgetFactory
{
    public:

        COcvWidgetHfsSegmentFactory()
        {
            m_name = "ocv_hierarchical_feature_selection";
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetHfsSegment>(pParam);
        }
};

#endif // COCVWIDGETHFSSEGMENT_HPP
