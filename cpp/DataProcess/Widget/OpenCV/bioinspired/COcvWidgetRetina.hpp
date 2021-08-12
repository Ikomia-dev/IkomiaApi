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

#ifndef COCVWIDGETRETINA_HPP
#define COCVWIDGETRETINA_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/bioinspired/COcvRetina.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetRetina : public CWorkflowTaskWidget
{
    public:

        COcvWidgetRetina(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetRetina(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRetinaParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRetinaParam>();

            m_pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvRetinaParam> m_pParam = nullptr;
        QCheckBox* m_pCheck = nullptr;
};

class COcvWidgetRetinaFactory : public CWidgetFactory
{
    public:

        COcvWidgetRetinaFactory()
        {
            m_name = QObject::tr("Retina").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRetina>(pParam);
        }
};

#endif // COCVWIDGETRETINA_HPP
