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

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMorphologyExParam>();

            auto pLabelKernel = new QLabel(tr("Kernel"));
            m_pComboKernel = new QComboBox;
            m_pComboKernel->addItem("Rectangle", cv::MorphShapes::MORPH_RECT);
            m_pComboKernel->addItem("Cross", cv::MorphShapes::MORPH_CROSS);
            m_pComboKernel->addItem("Ellipse", cv::MorphShapes::MORPH_ELLIPSE);
            m_pComboKernel->setCurrentIndex(m_pComboKernel->findData(m_pParam->m_kernel));

            m_pComboKernelSize = new QComboBox;
            m_pComboKernelSize->addItem("3x3", 3);
            m_pComboKernelSize->addItem("5x5", 5);
            m_pComboKernelSize->addItem("7x7", 7);
            m_pComboKernelSize->addItem("9x9", 9);
            m_pComboKernelSize->addItem("11x11", 11);
            m_pComboKernelSize->setCurrentIndex(m_pComboKernelSize->findData(m_pParam->m_kernelSize.width));

            auto pLabelMethod = new QLabel(tr("Method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem("Erode", cv::MorphTypes::MORPH_ERODE);
            m_pComboMethod->addItem("Dilate", cv::MorphTypes::MORPH_DILATE);
            m_pComboMethod->addItem("Open", cv::MorphTypes::MORPH_OPEN);
            m_pComboMethod->addItem("Close", cv::MorphTypes::MORPH_CLOSE);
            m_pComboMethod->addItem("Gradient", cv::MorphTypes::MORPH_GRADIENT);
            m_pComboMethod->addItem("TopHat", cv::MorphTypes::MORPH_TOPHAT);
            m_pComboMethod->addItem("BlackHat", cv::MorphTypes::MORPH_BLACKHAT);
            m_pComboMethod->addItem("HitMiss", cv::MorphTypes::MORPH_HITMISS);
            m_pComboMethod->setCurrentIndex(m_pComboMethod->findData(m_pParam->m_op));

            auto pLabelIter = new QLabel(tr("Iterations"));
            m_pSpinIter = new QSpinBox;
            m_pSpinIter->setMinimum(1);
            m_pSpinIter->setValue(m_pParam->m_iterations);

            auto pLabelBorder = new QLabel(tr("Border type"));
            m_pComboBorder = new QComboBox;
            m_pComboBorder->addItem("Constant", cv::BORDER_CONSTANT);
            m_pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            m_pComboBorder->setCurrentIndex(m_pComboBorder->findData(m_pParam->m_borderType));
            
            m_pLayout->addWidget(pLabelMethod, 0, 0);
            m_pLayout->addWidget(m_pComboMethod, 0, 1);

            m_pLayout->addWidget(pLabelKernel, 1, 0);
            m_pLayout->addWidget(m_pComboKernel, 1, 1);
            m_pLayout->addWidget(m_pComboKernelSize, 1, 2);

            m_pLayout->addWidget(pLabelBorder, 2, 0);
            m_pLayout->addWidget(m_pComboBorder, 2, 1);

            m_pLayout->addWidget(pLabelIter, 3, 0);
            m_pLayout->addWidget(m_pSpinIter, 3, 1);
        }

        void onApply() override
        {
            m_pParam->m_op = m_pComboMethod->currentData().toInt();
            m_pParam->m_kernel = m_pComboKernel->currentData().toInt();
            int s = m_pComboKernelSize->currentData().toInt();;
            m_pParam->m_kernelSize = cv::Size(s, s);
            m_pParam->m_borderType = m_pComboBorder->currentData().toDouble();
            m_pParam->m_iterations = m_pSpinIter->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvMorphologyExParam> m_pParam = nullptr;
        QComboBox*  m_pComboMethod = nullptr;
        QComboBox*  m_pComboKernel = nullptr;
        QComboBox*  m_pComboKernelSize = nullptr;
        QComboBox*  m_pComboBorder = nullptr;
        QSpinBox*   m_pSpinIter = nullptr;
};

class COcvWidgetMorphologyExFactory : public CWidgetFactory
{
    public:

        COcvWidgetMorphologyExFactory()
        {
            m_name = "ocv_morphology_ex";
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_unique<COcvWidgetMorphologyEx>(pParam);
        }
};

#endif // COCVWIDGETMORPHOLOGYEX_HPP
