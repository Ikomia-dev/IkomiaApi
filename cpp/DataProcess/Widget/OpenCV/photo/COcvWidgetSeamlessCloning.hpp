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

#ifndef COCVWIDGETSEAMLESSCLONING_HPP
#define COCVWIDGETSEAMLESSCLONING_HPP
#include "Process/OpenCV/photo/COcvSeamlessCloning.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetSeamlessCloning : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSeamlessCloning(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSeamlessCloning(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSeamlessCloningParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSeamlessCloningParam>();

            auto pCombo = addCombo(0, tr("Cloning method"));
            pCombo->addItem(tr("NORMAL_CLONE"), cv::NORMAL_CLONE);
            pCombo->addItem(tr("MIXED_CLONE"), cv::MIXED_CLONE);
            pCombo->addItem(tr("MONOCHROME_TRANSFER"), cv::MONOCHROME_TRANSFER);
            pCombo->setCurrentIndex(pCombo->findData(m_pParam->m_flags));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_flags = pCombo->currentData().toInt();
                emit doApplyProcess(m_pParam); } );

            
        }

    private:

        std::shared_ptr<COcvSeamlessCloningParam> m_pParam = nullptr;
};

class COcvWidgetSeamlessCloningFactory : public CWidgetFactory
{
    public:

        COcvWidgetSeamlessCloningFactory()
        {
            m_name = QObject::tr("SeamlessCloning").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSeamlessCloning>(pParam);
        }
};
#endif // COCVWIDGETSEAMLESSCLONING_HPP
