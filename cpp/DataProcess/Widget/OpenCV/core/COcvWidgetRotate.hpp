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

#ifndef COCVWIDGETROTATE_HPP
#define COCVWIDGETROTATE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvRotate.hpp"
#include <QComboBox>

class COcvWidgetRotate : public CWorkflowTaskWidget
{
    public:

        COcvWidgetRotate(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetRotate(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRotateParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRotateParam>();

            auto pLabelAngle = new QLabel(tr("Angle"));
            m_pComboAngle = new QComboBox;
            m_pComboAngle->addItem(tr("90°"), cv::ROTATE_90_CLOCKWISE);
            m_pComboAngle->addItem(tr("180°"), cv::ROTATE_180);
            m_pComboAngle->addItem(tr("270°"), cv::ROTATE_90_COUNTERCLOCKWISE);
            m_pComboAngle->setCurrentIndex(m_pComboAngle->findData(m_pParam->m_rotateCode));

            m_pLayout->addWidget(pLabelAngle, 0, 0);
            m_pLayout->addWidget(m_pComboAngle, 0, 1);
        }

        void onApply() override
        {
            m_pParam->m_rotateCode = m_pComboAngle->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvRotateParam>    m_pParam = nullptr;
        QComboBox*                          m_pComboAngle = nullptr;
};

class COcvWidgetRotateFactory : public CWidgetFactory
{
    public:

        COcvWidgetRotateFactory()
        {
            m_name = "ocv_rotate";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRotate>(pParam);
        }
};

#endif // COCVWIDGETROTATE_HPP
