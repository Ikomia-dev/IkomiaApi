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

#ifndef COCVWIDGETBOXFILTER_HPP
#define COCVWIDGETBOXFILTER_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvBoxFilter.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetBoxFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBoxFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBoxFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBoxFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBoxFilterParam>();

            m_pSpin = new QSpinBox;
            m_pSpin->setValue(m_pParam->m_ksize.width);
            QLabel* pLabelSpin = new QLabel(tr("Filter size d"));

            m_pCheckNormalize = new QCheckBox("Normalize");
            m_pCheckNormalize->setChecked(m_pParam->m_bNormalize);

            auto pLabelBorder = new QLabel(tr("Border type"));
            m_pComboBorder = new QComboBox;
            m_pComboBorder->addItem("Default", cv::BORDER_DEFAULT);
            m_pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            m_pComboBorder->setCurrentIndex(m_pComboBorder->findData(m_pParam->m_borderType));

            m_pLayout->addWidget(pLabelSpin, 0, 0);
            m_pLayout->addWidget(m_pSpin, 0, 1);
            m_pLayout->addWidget(m_pCheckNormalize, 1, 0);
            m_pLayout->addWidget(pLabelBorder, 2, 0);
            m_pLayout->addWidget(m_pComboBorder, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_ksize = cv::Size(m_pSpin->value(), m_pSpin->value());
            m_pParam->m_bNormalize = m_pCheckNormalize->isChecked();
            m_pParam->m_borderType = m_pComboBorder->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBoxFilterParam> m_pParam = nullptr;
        QSpinBox*                           m_pSpin = nullptr;
        QCheckBox*                          m_pCheckNormalize = nullptr;
        QComboBox*                          m_pComboBorder = nullptr;
};

class COcvWidgetBoxFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetBoxFilterFactory()
        {
            m_name = QObject::tr("Box Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBoxFilter>(pParam);
        }
};

#endif // COCVWIDGETBOXFILTER_HPP
