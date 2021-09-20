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

#ifndef COCVWIDGETSEAMLESSCLONING_HPP
#define COCVWIDGETSEAMLESSCLONING_HPP
#include "Process/OpenCV/photo/COcvSeamlessCloning.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetSeamlessCloning : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSeamlessCloning(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSeamlessCloning(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSeamlessCloningParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSeamlessCloningParam>();

            m_pCombo = addCombo(0, tr("Cloning method"));
            m_pCombo->addItem(tr("NORMAL_CLONE"), cv::NORMAL_CLONE);
            m_pCombo->addItem(tr("MIXED_CLONE"), cv::MIXED_CLONE);
            m_pCombo->addItem(tr("MONOCHROME_TRANSFER"), cv::MONOCHROME_TRANSFER);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_flags));
        }

        void onApply() override
        {
            m_pParam->m_flags = m_pCombo->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSeamlessCloningParam> m_pParam = nullptr;
        QComboBox* m_pCombo = nullptr;
};

class COcvWidgetSeamlessCloningFactory : public CWidgetFactory
{
    public:

        COcvWidgetSeamlessCloningFactory()
        {
            m_name = "ocv_seamless_cloning";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSeamlessCloning>(pParam);
        }
};

#endif // COCVWIDGETSEAMLESSCLONING_HPP
