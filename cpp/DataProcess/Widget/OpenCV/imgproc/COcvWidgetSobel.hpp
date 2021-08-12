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

#ifndef COCVWIDGETSOBEL_HPP
#define COCVWIDGETSOBEL_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvSobel.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSobel : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSobel(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSobel(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSobelParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSobelParam>();

            m_pSpinX = addSpin(0, tr("x-derivative order"), m_pParam->m_dx, 0, 3);
            m_pSpinY = addSpin(1, tr("y-derivative order"), m_pParam->m_dy, 0, 3);
            m_pSpinSize = addSpin(2, tr("Kernel size"), m_pParam->m_ksize, 1, INT_MAX);
            m_pDblSpinScale = addDoubleSpin(3, tr("Scale"), m_pParam->m_scale);
            m_pDblSpinDelta = addDoubleSpin(4, tr("Delta"), m_pParam->m_delta);

            m_pComboBorder = addCombo(5, "Border type");
            m_pComboBorder->addItem("Default", cv::BORDER_DEFAULT);
            m_pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            m_pComboBorder->setCurrentIndex(m_pComboBorder->findData(m_pParam->m_border));
        }

        void onApply() override
        {
            m_pParam->m_dx = m_pSpinX->value();
            m_pParam->m_dy = m_pSpinY->value();
            m_pParam->m_ksize = m_pSpinSize->value();
            m_pParam->m_scale = m_pDblSpinScale->value();
            m_pParam->m_delta = m_pDblSpinDelta->value();
            m_pParam->m_border = m_pComboBorder->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSobelParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pDblSpinScale = nullptr;
        QDoubleSpinBox* m_pDblSpinDelta = nullptr;
        QSpinBox*       m_pSpinX = nullptr;
        QSpinBox*       m_pSpinY = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
        QComboBox*      m_pComboBorder = nullptr;
};

class COcvWidgetSobelFactory : public CWidgetFactory
{
    public:

        COcvWidgetSobelFactory()
        {
            m_name = QObject::tr("Sobel").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSobel>(pParam);
        }
};
#endif // COCVWIDGETSOBEL_HPP
