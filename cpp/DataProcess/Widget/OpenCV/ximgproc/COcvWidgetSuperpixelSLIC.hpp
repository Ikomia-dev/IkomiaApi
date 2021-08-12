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

#ifndef COCVWIDGETSUPERPIXELSLIC_HPP
#define COCVWIDGETSUPERPIXELSLIC_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSuperpixelSLIC.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSuperpixelSLIC : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSuperpixelSLIC(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperpixelSLIC(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperpixelSLICParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperpixelSLICParam>();

            m_pCombo = addCombo(0, tr("Algorithm"));
            m_pCombo->addItem("SLIC", cv::ximgproc::SLIC);
            m_pCombo->addItem("SLICO", cv::ximgproc::SLICO);
            m_pCombo->addItem("MSLIC", cv::ximgproc::MSLIC);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_algorithm));
            m_pSpinSize = addSpin(1, tr("Region size"), m_pParam->m_region_size);
            m_pSpinRuler = addDoubleSpin(2, tr("Ruler"), m_pParam->m_ruler);
            m_pSpinIter = addSpin(3, tr("Iterations"), m_pParam->m_num_iterations);
        }

        void onApply() override
        {
            m_pParam->m_region_size = m_pSpinSize->value();
            m_pParam->m_ruler = m_pSpinRuler->value();
            m_pParam->m_num_iterations = m_pSpinIter->value();
            m_pParam->m_algorithm = m_pCombo->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSuperpixelSLICParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRuler = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
        QSpinBox*       m_pSpinIter = nullptr;
        QComboBox*      m_pCombo = nullptr;
};

class COcvWidgetSuperpixelSLICFactory : public CWidgetFactory
{
    public:

        COcvWidgetSuperpixelSLICFactory()
        {
            m_name = QObject::tr("SuperpixelSLIC").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperpixelSLIC>(pParam);
        }
};

#endif // COCVWIDGETSUPERPIXELSLIC_HPP
