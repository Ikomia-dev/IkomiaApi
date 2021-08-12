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

#ifndef COCVWIDGETSPARSETODENSEOF_HPP
#define COCVWIDGETSPARSETODENSEOF_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvSparseToDenseOF.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSparseToDenseOF : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSparseToDenseOF(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSparseToDenseOF(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSparseToDenseOFParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSparseToDenseOFParam>();

            m_pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSparseToDenseOFParam> m_pParam = nullptr;
        QCheckBox*  m_pCheck = nullptr;
};

class COcvWidgetSparseToDenseOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetSparseToDenseOFFactory()
        {
            m_name = QObject::tr("SparseToDense Flow").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSparseToDenseOF>(pParam);
        }
};

#endif // COCVWIDGETSPARSETODENSEOF_HPP
