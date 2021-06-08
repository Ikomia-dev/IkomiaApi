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

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBFMatcherParam>();

            auto pCombo = addCombo(0, tr("Norm type"));
            pCombo->addItem(tr("NORM_INF"), cv::NORM_INF);
            pCombo->addItem(tr("NORM_L1"), cv::NORM_L1);
            pCombo->addItem(tr("NORM_L2"), cv::NORM_L2);
            pCombo->addItem(tr("NORM_L2SQR"), cv::NORM_L2SQR);
            pCombo->addItem(tr("NORM_HAMMING"), cv::NORM_HAMMING);
            pCombo->addItem(tr("NORM_HAMMING2"), cv::NORM_HAMMING2);
            pCombo->addItem(tr("NORM_TYPE_MASK"), cv::NORM_TYPE_MASK);
            pCombo->addItem(tr("NORM_MINMAX"), cv::NORM_MINMAX);
            pCombo->addItem(tr("NORM_RELATIVE"), cv::NORM_RELATIVE);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_normType));

            auto pCheck = addCheck(1, tr("Cross check"), m_pParam->m_bCrossCheck);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_normType = pCombo->currentData().toInt();
                m_pParam->m_bCrossCheck = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvBFMatcherParam> m_pParam = nullptr;
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
