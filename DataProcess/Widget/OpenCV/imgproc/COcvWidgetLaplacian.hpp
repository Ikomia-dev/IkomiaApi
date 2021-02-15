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

#ifndef COCVWIDGETLAPLACIAN_HPP
#define COCVWIDGETLAPLACIAN_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvLaplacian.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetLaplacian : public CProtocolTaskWidget
{
    public:

        COcvWidgetLaplacian(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetLaplacian(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvLaplacianParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvLaplacianParam>();

            auto pSpin = addSpin(0, tr("Filter size d"), m_pParam->m_ksize);
            auto pSpinScale = addDoubleSpin(1, tr("Scale"), m_pParam->m_scale);
            auto pSpinDelta = addDoubleSpin(2, tr("Delta"), m_pParam->m_delta);

            auto pComboDepth = addCombo(3, tr("Output depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_ddepth));

            auto pComboBorder = addCombo(4, tr("Border type"));
            pComboBorder->addItem("Default", cv::BORDER_DEFAULT);
            pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_borderType));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_ksize = pSpin->value();
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_delta = pSpinDelta->value();
                m_pParam->m_ddepth = pComboDepth->currentData().toInt();
                m_pParam->m_borderType = pComboBorder->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvLaplacianParam> m_pParam = nullptr;
};

class COcvWidgetLaplacianFactory : public CWidgetFactory
{
    public:

        COcvWidgetLaplacianFactory()
        {
            m_name = QObject::tr("Laplacian").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetLaplacian>(pParam);
        }
};
#endif // COCVWIDGETLAPLACIAN_HPP
