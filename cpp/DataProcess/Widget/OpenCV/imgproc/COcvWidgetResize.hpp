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

#ifndef COCVWIDGETRESIZE_HPP
#define COCVWIDGETRESIZE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvResize.hpp"
#include <QComboBox>
#include <QLineEdit>

class COcvWidgetResize : public CWorkflowTaskWidget
{
    public:

        COcvWidgetResize(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetResize(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvResizeParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvResizeParam>();

            auto pLabelUnit = new QLabel(tr("Unit"));
            m_pComboUnit = new QComboBox;
            m_pComboUnit->addItem(tr("Pixels"));
            m_pComboUnit->addItem(tr("Percentage"));
            m_pComboUnit->setCurrentIndex(m_pParam->m_bPixelUnit == true ? 0 : 1);

            auto pLabelWidth = new QLabel(tr("Width"));
            m_pEditWidth = new QLineEdit;
            m_pParam->m_bPixelUnit == true ? m_pEditWidth->setText(QString::number(m_pParam->m_newWidth)) : m_pEditWidth->setText(QString::number(m_pParam->m_fx*100));

            auto pLabelHeight = new QLabel(tr("Height"));
            m_pEditHeight = new QLineEdit;
            m_pParam->m_bPixelUnit == true ? m_pEditHeight->setText(QString::number(m_pParam->m_newHeight)) : m_pEditHeight->setText(QString::number(m_pParam->m_fy*100));

            auto pLabelInterpolation = new QLabel(tr("Interpolation"));
            m_pComboInterpolation = new QComboBox;
            m_pComboInterpolation->addItem(tr("Nearest neighbor"), cv::INTER_NEAREST);
            m_pComboInterpolation->addItem(tr("Bilinear"), cv::INTER_LINEAR);
            m_pComboInterpolation->addItem(tr("Cubic"), cv::INTER_CUBIC);
            m_pComboInterpolation->addItem(tr("Area"), cv::INTER_AREA);
            m_pComboInterpolation->addItem(tr("Lanczos 4"), cv::INTER_LANCZOS4);
            m_pComboInterpolation->addItem(tr("Exact bilinear"), cv::INTER_LINEAR_EXACT);
            m_pComboInterpolation->setCurrentIndex(m_pComboInterpolation->findData(m_pParam->m_interpolation));

            
            m_pLayout->addWidget(pLabelUnit, 0, 0);
            m_pLayout->addWidget(m_pComboUnit, 0, 1);
            m_pLayout->addWidget(pLabelWidth, 1, 0);
            m_pLayout->addWidget(m_pEditWidth, 1, 1);
            m_pLayout->addWidget(pLabelHeight, 2, 0);
            m_pLayout->addWidget(m_pEditHeight, 2, 1);
            m_pLayout->addWidget(pLabelInterpolation, 3, 0);
            m_pLayout->addWidget(m_pComboInterpolation, 3, 1);
            

            initConnections();
        }

        void initConnections()
        {
            connect(m_pApplyBtn, &QPushButton::clicked, [&]
            {
                if(m_pComboUnit->currentIndex() == 0)
                {
                    m_pParam->m_bPixelUnit = true;
                    m_pParam->m_fx = 0;
                    m_pParam->m_fy = 0;
                    m_pParam->m_newWidth = m_pEditWidth->text().toInt();
                    m_pParam->m_newHeight = m_pEditHeight->text().toInt();
                }
                else
                {
                    m_pParam->m_bPixelUnit = false;
                    m_pParam->m_newWidth = 0;
                    m_pParam->m_newHeight = 0;
                    m_pParam->m_fx = m_pEditWidth->text().toDouble() / 100;
                    m_pParam->m_fy = m_pEditHeight->text().toDouble() / 100;
                }
                m_pParam->m_interpolation = m_pComboInterpolation->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvResizeParam>    m_pParam = nullptr;
        QComboBox*                          m_pComboUnit = nullptr;
        QComboBox*                          m_pComboInterpolation = nullptr;
        QLineEdit*                          m_pEditWidth = nullptr;
        QLineEdit*                          m_pEditHeight = nullptr;
};

class COcvWidgetResizeFactory : public CWidgetFactory
{
    public:

        COcvWidgetResizeFactory()
        {
            m_name = QObject::tr("Resize").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetResize>(pParam);
        }
};

#endif // COCVWIDGETRESIZE_HPP
