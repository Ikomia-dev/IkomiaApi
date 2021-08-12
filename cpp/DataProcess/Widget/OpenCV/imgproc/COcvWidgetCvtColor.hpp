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

#ifndef COCVWIDGETCVTCOLOR_HPP
#define COCVWIDGETCVTCOLOR_HPP

#include "Process/OpenCV/imgproc/COcvCvtColor.hpp"
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetCvtColor : public CWorkflowTaskWidget
{
    public:

        COcvWidgetCvtColor(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCvtColor(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCvtColorParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCvtColorParam>();

            auto pLabel = new QLabel(tr("Color conversion"));
            m_pCombo = new QComboBox;
            m_pCombo->addItem("RGB2GRAY", cv::COLOR_RGB2GRAY);
            m_pCombo->addItem("RGB2BGR", cv::COLOR_RGB2BGR);
            m_pCombo->addItem("GRAY2RGB", cv::COLOR_GRAY2RGB);
            m_pCombo->addItem("RGB2XYZ", cv::COLOR_RGB2XYZ);
            m_pCombo->addItem("XYZ2RGB", cv::COLOR_XYZ2RGB);
            m_pCombo->addItem("RGB2YCrCb", cv::COLOR_RGB2YCrCb);
            m_pCombo->addItem("YCrCb2RGB", cv::COLOR_YCrCb2RGB);
            m_pCombo->addItem("RGB2YUV", cv::COLOR_RGB2YUV);
            m_pCombo->addItem("YUV2RGB", cv::COLOR_YUV2RGB);
            m_pCombo->addItem("RGB2HSV", cv::COLOR_RGB2HSV);
            m_pCombo->addItem("HSV2RGB", cv::COLOR_HSV2RGB);
            m_pCombo->addItem("RGB2HLS", cv::COLOR_RGB2HLS);
            m_pCombo->addItem("HLS2RGB", cv::COLOR_HLS2RGB);
            m_pCombo->addItem("RGB2Lab", cv::COLOR_RGB2Lab);
            m_pCombo->addItem("Lab2RGB", cv::COLOR_Lab2RGB);
            m_pCombo->addItem("RGB2Luv", cv::COLOR_RGB2Luv);
            m_pCombo->addItem("Luv2RGB", cv::COLOR_Luv2RGB);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_code));

            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(m_pCombo, 0, 1);
        }

        void onApply() override
        {
            m_pParam->m_code = m_pCombo->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvCvtColorParam> m_pParam = nullptr;
        QComboBox*  m_pCombo = nullptr;
};

class COcvWidgetCvtColorFactory : public CWidgetFactory
{
    public:

        COcvWidgetCvtColorFactory()
        {
            m_name = QObject::tr("Color Conversion").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCvtColor>(pParam);
        }
};

#endif // COCVWIDGETCVTCOLOR_HPP
