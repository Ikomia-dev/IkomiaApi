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

#ifndef COCVWIDGETSUPERPIXELLSC_HPP
#define COCVWIDGETSUPERPIXELLSC_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSuperpixelLSC.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSuperpixelLSC : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSuperpixelLSC(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperpixelLSC(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperpixelLSCParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperpixelLSCParam>();

            m_pSpinSize = addSpin(0, tr("Region size"), m_pParam->m_regions_size);
            m_pSpinRatio = addDoubleSpin(1, tr("Ratio"), m_pParam->m_ratio, 0, 1, 0.001, 3);
            m_pSpinIter = addSpin(2, tr("Iterations"), m_pParam->m_num_iterations);
        }

        void onApply() override
        {
            m_pParam->m_regions_size = m_pSpinSize->value();
            m_pParam->m_ratio = m_pSpinRatio->value();
            m_pParam->m_num_iterations = m_pSpinIter->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSuperpixelLSCParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinRatio = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
        QSpinBox*       m_pSpinIter = nullptr;
};

class COcvWidgetSuperpixelLSCFactory : public CWidgetFactory
{
    public:

        COcvWidgetSuperpixelLSCFactory()
        {
            m_name = QObject::tr("SuperpixelLSC").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperpixelLSC>(pParam);
        }
};

#endif // COCVWIDGETSUPERPIXELLSC_HPP
