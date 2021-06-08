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

#ifndef CGMICWIDGETSKELETON_HPP
#define CGMICWIDGETSKELETON_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicSkeleton.hpp"

class CGmicWidgetSkeleton : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetSkeleton(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSkeleton(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSkeletonParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSkeletonParam>();

            auto pComboMethod = addCombo(0, tr("Method"));
            pComboMethod->addItem(tr("Distance(Fast)"), CGmicSkeletonParam::DISTANCE);
            pComboMethod->addItem(tr("Thinning(Slow)"), CGmicSkeletonParam::THINNING);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_method));

            auto pSpinSmooth = addDoubleSpin(1, tr("Smoothness"), m_pParam->m_smoothness, 0, 10, 0.5);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_method = pComboMethod->currentData().toInt();
                m_pParam->m_smoothness = pSpinSmooth->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSkeletonParam> m_pParam = nullptr;
};

class CGmicWidgetSkeletonFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSkeletonFactory()
        {
            m_name = QObject::tr("Skeleton").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSkeleton>(pParam);
        }
};

#endif // CGMICWIDGETSKELETON_HPP
