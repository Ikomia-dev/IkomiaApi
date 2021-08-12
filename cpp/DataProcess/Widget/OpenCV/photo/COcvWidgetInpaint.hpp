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

#ifndef COCVWIDGETINPAINT_HPP
#define COCVWIDGETINPAINT_HPP

#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "Process/OpenCV/photo/COcvInpaint.hpp"

class COcvWidgetInpaint : public CWorkflowTaskWidget
{
    public:

        COcvWidgetInpaint(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetInpaint(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInpaintParam>(pParam);;
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInpaintParam>();

            QLabel* pLabelRadius = new QLabel(QObject::tr("Neighborhood diameter"));
            m_pSpinRadius = new QSpinBox;
            m_pSpinRadius->setMinimum(1);
            m_pSpinRadius->setMaximum(256);
            m_pSpinRadius->setValue(m_pParam->m_radius);

            QLabel* pLabelMethod = new QLabel(QObject::tr("Method"));
            m_pComboMethod = new QComboBox;
            m_pComboMethod->addItem("Navier-Stokes");
            m_pComboMethod->addItem("Telea");

            if(m_pParam->m_method == cv::INPAINT_NS)
                m_pComboMethod->setCurrentIndex(0);
            else
                m_pComboMethod->setCurrentIndex(0);
            
            m_pLayout->addWidget(pLabelRadius, 0, 0);
            m_pLayout->addWidget(m_pSpinRadius, 0, 1);
            m_pLayout->addWidget(pLabelMethod, 1, 0);
            m_pLayout->addWidget(m_pComboMethod, 1, 1);
        }

        void onApply() override
        {
            m_pParam->m_radius = m_pSpinRadius->value();
            m_pParam->m_method = m_pComboMethod->currentIndex();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvInpaintParam>   m_pParam = nullptr;
        QSpinBox*                           m_pSpinRadius = nullptr;
        QComboBox*                          m_pComboMethod = nullptr;
};

class COcvWidgetInpaintFactory : public CWidgetFactory
{
    public:

        COcvWidgetInpaintFactory()
        {
            m_name = QObject::tr("Inpaint filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInpaint>(pParam);
        }
};

#endif // COCVWIDGETINPAINT_HPP
