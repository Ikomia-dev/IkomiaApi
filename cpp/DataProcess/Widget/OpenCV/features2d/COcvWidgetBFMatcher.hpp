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

#ifndef COCVWIDGETBFMATCHER_HPP
#define COCVWIDGETBFMATCHER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvBFMatcher.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetBFMatcher : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBFMatcher(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBFMatcher(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBFMatcherParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBFMatcherParam>();

            m_pCombo = addCombo(0, tr("Norm type"));
            m_pCombo->addItem(tr("NORM_INF"), cv::NORM_INF);
            m_pCombo->addItem(tr("NORM_L1"), cv::NORM_L1);
            m_pCombo->addItem(tr("NORM_L2"), cv::NORM_L2);
            m_pCombo->addItem(tr("NORM_L2SQR"), cv::NORM_L2SQR);
            m_pCombo->addItem(tr("NORM_HAMMING"), cv::NORM_HAMMING);
            m_pCombo->addItem(tr("NORM_HAMMING2"), cv::NORM_HAMMING2);
            m_pCombo->addItem(tr("NORM_TYPE_MASK"), cv::NORM_TYPE_MASK);
            m_pCombo->addItem(tr("NORM_MINMAX"), cv::NORM_MINMAX);
            m_pCombo->addItem(tr("NORM_RELATIVE"), cv::NORM_RELATIVE);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_normType));

            m_pCheck = addCheck(1, tr("Cross check"), m_pParam->m_bCrossCheck);
        }

        void onApply() override
        {
            m_pParam->m_normType = m_pCombo->currentData().toInt();
            m_pParam->m_bCrossCheck = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBFMatcherParam> m_pParam = nullptr;
        QComboBox*  m_pCombo = nullptr;
        QCheckBox*  m_pCheck = nullptr;
};

class COcvWidgetBFMatcherFactory : public CWidgetFactory
{
    public:

        COcvWidgetBFMatcherFactory()
        {
            m_name = QObject::tr("BFMatcher").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBFMatcher>(pParam);
        }
};
#endif // COCVWIDGETBFMATCHER_HPP
