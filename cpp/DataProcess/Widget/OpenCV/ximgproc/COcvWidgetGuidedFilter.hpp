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

#ifndef COCVWIDGETGUIDEDFILTER_HPP
#define COCVWIDGETGUIDEDFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGuidedFilter.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>

class COcvWidgetGuidedFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGuidedFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetGuidedFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGuidedFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGuidedFilterParam>();

            m_pSpinRadius = addSpin(0, tr("Radius"), m_pParam->m_radius);
            m_pSpinEps = addDoubleSpin(1, tr("Epsilon"), m_pParam->m_eps, 0, 1, 0.1);
            m_pComboDepth = addCombo(2, tr("Depth"));
            m_pComboDepth->addItem("Default", -1);
            m_pComboDepth->addItem("CV_8U", CV_8U);
            m_pComboDepth->addItem("CV_8S", CV_8S);
            m_pComboDepth->addItem("CV_16U", CV_16U);
            m_pComboDepth->addItem("CV_16S", CV_16S);
            m_pComboDepth->addItem("CV_32S", CV_32S);
            m_pComboDepth->addItem("CV_32F", CV_32F);
            m_pComboDepth->addItem("CV_64F", CV_64F);
            m_pComboDepth->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_ddepth));
        }

        void onApply() override
        {
            m_pParam->m_radius = m_pSpinRadius->value();
            m_pParam->m_eps = m_pSpinEps->value();
            m_pParam->m_ddepth = m_pComboDepth->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvGuidedFilterParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinEps = nullptr;
        QSpinBox*       m_pSpinRadius = nullptr;
        QComboBox*      m_pComboDepth = nullptr;
};

class COcvWidgetGuidedFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetGuidedFilterFactory()
        {
            m_name = QObject::tr("Guided Filter").toStdString();
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGuidedFilter>(pParam);
        }
};

#endif // COCVWIDGETGUIDEDFILTER_HPP
