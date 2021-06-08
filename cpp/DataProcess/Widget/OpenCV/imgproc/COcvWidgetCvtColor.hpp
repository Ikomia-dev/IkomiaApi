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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCvtColorParam>();

            auto pLabel = new QLabel(tr("Color conversion"));
            auto pCombo = new QComboBox;
            pCombo->addItem("RGB2GRAY", cv::COLOR_RGB2GRAY);
            pCombo->addItem("RGB2BGR", cv::COLOR_RGB2BGR);
            pCombo->addItem("GRAY2RGB", cv::COLOR_GRAY2RGB);
            pCombo->addItem("RGB2XYZ", cv::COLOR_RGB2XYZ);
            pCombo->addItem("XYZ2RGB", cv::COLOR_XYZ2RGB);
            pCombo->addItem("RGB2YCrCb", cv::COLOR_RGB2YCrCb);
            pCombo->addItem("YCrCb2RGB", cv::COLOR_YCrCb2RGB);
            pCombo->addItem("RGB2YUV", cv::COLOR_RGB2YUV);
            pCombo->addItem("YUV2RGB", cv::COLOR_YUV2RGB);
            pCombo->addItem("RGB2HSV", cv::COLOR_RGB2HSV);
            pCombo->addItem("HSV2RGB", cv::COLOR_HSV2RGB);
            pCombo->addItem("RGB2HLS", cv::COLOR_RGB2HLS);
            pCombo->addItem("HLS2RGB", cv::COLOR_HLS2RGB);
            pCombo->addItem("RGB2Lab", cv::COLOR_RGB2Lab);
            pCombo->addItem("Lab2RGB", cv::COLOR_Lab2RGB);
            pCombo->addItem("RGB2Luv", cv::COLOR_RGB2Luv);
            pCombo->addItem("Luv2RGB", cv::COLOR_Luv2RGB);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_code));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_code = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam); } );

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pCombo, 0, 1);
            
        }

    private:

        std::shared_ptr<COcvCvtColorParam> m_pParam = nullptr;
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
