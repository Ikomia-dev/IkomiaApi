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

#ifndef COCVWIDGETDISOPTICALFLOW_HPP
#define COCVWIDGETDISOPTICALFLOW_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvDISOpticalFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetDISOF : public CWorkflowTaskWidget
{
    public:

        COcvWidgetDISOF(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDISOF(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDISOFParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDISOFParam>();

            m_pCombo = addCombo(0, tr("Preset mode"));
            m_pCombo->addItem("PRESET_MEDIUM", cv::DISOpticalFlow::PRESET_MEDIUM);
            m_pCombo->addItem("PRESET_FAST", cv::DISOpticalFlow::PRESET_FAST);
            m_pCombo->addItem("PRESET_ULTRAFAST", cv::DISOpticalFlow::PRESET_ULTRAFAST);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_preset));
            m_pCheck = addCheck(1, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_preset = m_pCombo->currentData().toInt();
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvDISOFParam> m_pParam = nullptr;
        QComboBox*  m_pCombo = nullptr;
        QCheckBox*  m_pCheck = nullptr;
};

class COcvWidgetDISOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetDISOFFactory()
        {
            m_name = QObject::tr("DIS Optical Flow").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDISOF>(pParam);
        }
};
#endif // COCVWIDGETDISOPTICALFLOW_HPP
