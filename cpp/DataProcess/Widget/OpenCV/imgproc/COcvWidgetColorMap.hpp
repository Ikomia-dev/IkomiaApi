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

#ifndef COCVWIDGETCOLORMAP_HPP
#define COCVWIDGETCOLORMAP_HPP

#include "Process/OpenCV/imgproc/COcvColorMap.hpp"
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class COcvWidgetColorMap : public CWorkflowTaskWidget
{
    public:

        COcvWidgetColorMap(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetColorMap(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvColorMapParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvColorMapParam>();

            auto pLabel = new QLabel(tr("Colormap type"));
            auto pCombo = new QComboBox;
            pCombo->addItem("Autumn", cv::COLORMAP_AUTUMN);
            pCombo->addItem("Bone", cv::COLORMAP_BONE);
            pCombo->addItem("Jet", cv::COLORMAP_JET);
            pCombo->addItem("Winter", cv::COLORMAP_WINTER);
            pCombo->addItem("Rainbow", cv::COLORMAP_RAINBOW);
            pCombo->addItem("Ocean", cv::COLORMAP_OCEAN);
            pCombo->addItem("Summer", cv::COLORMAP_SUMMER);
            pCombo->addItem("Spring", cv::COLORMAP_SPRING);
            pCombo->addItem("Cool", cv::COLORMAP_COOL);
            pCombo->addItem("Hsv", cv::COLORMAP_HSV);
            pCombo->addItem("Pink", cv::COLORMAP_PINK);
            pCombo->addItem("Hot", cv::COLORMAP_HOT);
            pCombo->addItem("Parula", cv::COLORMAP_PARULA);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_type));

            
            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pCombo, 0, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_type = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvColorMapParam> m_pParam = nullptr;
};

class COcvWidgetColorMapFactory : public CWidgetFactory
{
    public:

        COcvWidgetColorMapFactory()
        {
            m_name = QObject::tr("Colormap").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetColorMap>(pParam);
        }
};

#endif // COCVWIDGETCOLORMAP_HPP
