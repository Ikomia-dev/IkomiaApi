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

#ifndef COCVWIDGETFASTLINEDETECTOR_HPP
#define COCVWIDGETFASTLINEDETECTOR_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvFastLineDetector.hpp"

class COcvWidgetFastLineDetector : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFastLineDetector(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFastLineDetector(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFastLineDetectorParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFastLineDetectorParam>();

            auto pLabelLength = new QLabel(tr("Minimun length"));
            auto pSpinLength = new QSpinBox;
            pSpinLength->setValue(m_pParam->m_lengthThreshold);
            pSpinLength->setRange(1, 1024);
            pSpinLength->setSingleStep(1.0);

            auto pLabelDistance = new QLabel(tr("Minimum distance between lines"));
            auto pSpinDistance = new QDoubleSpinBox;
            pSpinDistance->setValue(m_pParam->m_distanceThreshold);
            pSpinDistance->setRange(1.0, 1024.0);
            pSpinDistance->setSingleStep(1.0);

            auto pLabelCanny1 = new QLabel(tr("Canny threshold 1"));
            auto pSpinCanny1 = new QSpinBox;
            pSpinCanny1->setValue(m_pParam->m_cannyThreshold1);
            pSpinCanny1->setRange(0, 255);
            pSpinCanny1->setSingleStep(1);

            auto pLabelCanny2 = new QLabel(tr("Canny threshold 2"));
            auto pSpinCanny2 = new QSpinBox;
            pSpinCanny2->setValue(m_pParam->m_cannyThreshold2);
            pSpinCanny2->setRange(1, 255);
            pSpinCanny2->setSingleStep(1);

            auto pLabelAperture = new QLabel(tr("Canny aperture size"));
            auto pSpinAperture = new QSpinBox;
            pSpinAperture->setValue(m_pParam->m_cannyApertureSize);
            pSpinAperture->setRange(1, 255);
            pSpinAperture->setSingleStep(2);

            auto pCheckMerge = new QCheckBox(tr("Merge"));
            pCheckMerge->setChecked(m_pParam->m_bMerge);

            
            m_pLayout->addWidget(pLabelLength, 0, 0);
            m_pLayout->addWidget(pSpinLength, 0, 1);
            m_pLayout->addWidget(pLabelDistance, 1, 0);
            m_pLayout->addWidget(pSpinDistance, 1, 1);
            m_pLayout->addWidget(pLabelCanny1, 2, 0);
            m_pLayout->addWidget(pSpinCanny1, 2, 1);
            m_pLayout->addWidget(pLabelCanny2, 3, 0);
            m_pLayout->addWidget(pSpinCanny2, 3, 1);
            m_pLayout->addWidget(pLabelAperture, 4, 0);
            m_pLayout->addWidget(pSpinAperture, 4, 1);
            m_pLayout->addWidget(pCheckMerge, 5, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_lengthThreshold = pSpinLength->value();
                m_pParam->m_distanceThreshold = pSpinDistance->value();
                m_pParam->m_cannyThreshold1 = pSpinCanny1->value();
                m_pParam->m_cannyThreshold2 = pSpinCanny2->value();
                m_pParam->m_cannyApertureSize = pSpinAperture->value();
                m_pParam->m_bMerge = pCheckMerge->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvFastLineDetectorParam> m_pParam = nullptr;
};

class COcvWidgetFastLineDetectorFactory : public CWidgetFactory
{
    public:

        COcvWidgetFastLineDetectorFactory()
        {
            m_name = QObject::tr("Fast Line Detector").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFastLineDetector>(pParam);
        }
};

#endif // COCVWIDGETFASTLINEDETECTOR_HPP
