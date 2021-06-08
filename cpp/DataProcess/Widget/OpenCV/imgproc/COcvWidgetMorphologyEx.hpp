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

#ifndef COCVWIDGETMORPHOLOGYEX_HPP
#define COCVWIDGETMORPHOLOGYEX_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvMorphologyEx.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetMorphologyEx : public CWorkflowTaskWidget
{
    public:

        COcvWidgetMorphologyEx(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMorphologyEx(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMorphologyExParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMorphologyExParam>();

            auto pLabelKernel = new QLabel(tr("Kernel"));
            auto pComboKernel = new QComboBox;
            pComboKernel->addItem("Rectangle", cv::MorphShapes::MORPH_RECT);
            pComboKernel->addItem("Cross", cv::MorphShapes::MORPH_CROSS);
            pComboKernel->addItem("Ellipse", cv::MorphShapes::MORPH_ELLIPSE);
            pComboKernel->setCurrentIndex(pComboKernel->findData(m_pParam->m_kernel));

            auto pComboKernelSize = new QComboBox;
            pComboKernelSize->addItem("3x3", 3);
            pComboKernelSize->addItem("5x5", 5);
            pComboKernelSize->addItem("7x7", 7);
            pComboKernelSize->addItem("9x9", 9);
            pComboKernelSize->addItem("11x11", 11);
            pComboKernelSize->setCurrentIndex(pComboKernelSize->findData(m_pParam->m_kernelSize.width));

            auto pLabelMethod = new QLabel(tr("Method"));
            auto pComboMethod = new QComboBox;
            pComboMethod->addItem("Erode", cv::MorphTypes::MORPH_ERODE);
            pComboMethod->addItem("Dilate", cv::MorphTypes::MORPH_DILATE);
            pComboMethod->addItem("Open", cv::MorphTypes::MORPH_OPEN);
            pComboMethod->addItem("Close", cv::MorphTypes::MORPH_CLOSE);
            pComboMethod->addItem("Gradient", cv::MorphTypes::MORPH_GRADIENT);
            pComboMethod->addItem("TopHat", cv::MorphTypes::MORPH_TOPHAT);
            pComboMethod->addItem("BlackHat", cv::MorphTypes::MORPH_BLACKHAT);
            pComboMethod->addItem("HitMiss", cv::MorphTypes::MORPH_HITMISS);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_op));

            auto pLabelIter = new QLabel(tr("Iterations"));
            auto pSpinIter = new QSpinBox;
            pSpinIter->setMinimum(1);
            pSpinIter->setValue(m_pParam->m_iterations);

            auto pLabelBorder = new QLabel(tr("Border type"));
            auto pComboBorder = new QComboBox;
            pComboBorder->addItem("Constant", cv::BORDER_CONSTANT);
            pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_borderType));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_op = pComboMethod->currentData().toInt();
                m_pParam->m_kernel = pComboKernel->currentData().toInt();
                int s = pComboKernelSize->currentData().toInt();;
                m_pParam->m_kernelSize = cv::Size(s,s);
                m_pParam->m_borderType = pComboBorder->currentData().toDouble();
                m_pParam->m_iterations = pSpinIter->value();
                emit doApplyProcess(m_pParam);
            });

            
            m_pLayout->addWidget(pLabelMethod, 0, 0);
            m_pLayout->addWidget(pComboMethod, 0, 1);

            m_pLayout->addWidget(pLabelKernel, 1, 0);
            m_pLayout->addWidget(pComboKernel, 1, 1);
            m_pLayout->addWidget(pComboKernelSize, 1, 2);

            m_pLayout->addWidget(pLabelBorder, 2, 0);
            m_pLayout->addWidget(pComboBorder, 2, 1);

            m_pLayout->addWidget(pLabelIter, 3, 0);
            m_pLayout->addWidget(pSpinIter, 3, 1);

            
        }

    private:

        std::shared_ptr<COcvMorphologyExParam> m_pParam = nullptr;
};

class COcvWidgetMorphologyExFactory : public CWidgetFactory
{
    public:

        COcvWidgetMorphologyExFactory()
        {
            m_name = QObject::tr("MorphologyEx").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetMorphologyEx>(pParam);
        }
};

#endif // COCVWIDGETMORPHOLOGYEX_HPP
