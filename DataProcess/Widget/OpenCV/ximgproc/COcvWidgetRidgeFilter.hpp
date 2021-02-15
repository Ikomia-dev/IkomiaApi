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

#ifndef COCVWIDGETRIDGEFILTER_HPP
#define COCVWIDGETRIDGEFILTER_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvRidgeFilter.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLabel>

class COcvWidgetRidgeFilter : public CProtocolTaskWidget
{
    public:

        COcvWidgetRidgeFilter(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetRidgeFilter(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRidgeFilterParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRidgeFilterParam>();

            auto pComboDepth = addCombo(0, tr("Output depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_8S", CV_8S);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_16S", CV_16S);
            pComboDepth->addItem("CV_32S", CV_32S);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_ddepth));
            auto pSpinDx = addSpin(1, tr("dx"), m_pParam->m_dx);
            auto pSpinDy = addSpin(2, tr("dy"), m_pParam->m_dy);
            auto pSpinSize = addSpin(3, tr("ksize"), m_pParam->m_ksize);
            auto pComboType = addCombo(4, tr("Out dtype"));
            pComboType->addItem("Default", -1);
            pComboType->addItem("CV_8U", CV_8U);
            pComboType->addItem("CV_8S", CV_8S);
            pComboType->addItem("CV_16U", CV_16U);
            pComboType->addItem("CV_16S", CV_16S);
            pComboType->addItem("CV_32S", CV_32S);
            pComboType->addItem("CV_32F", CV_32F);
            pComboType->addItem("CV_64F", CV_64F);
            pComboType->setCurrentIndex(pComboDepth->findData(m_pParam->m_out_dtype));
            auto pSpinScale = addDoubleSpin(5, tr("Scale"), m_pParam->m_scale);
            auto pSpinDelta = addDoubleSpin(6, tr("Delta"), m_pParam->m_delta);
            auto pComboBorder = addCombo(7, tr("Border type"));
            pComboBorder->addItem("BORDER_DEFAULT", cv::BORDER_DEFAULT);
            pComboBorder->addItem("BORDER_CONSTANT", cv::BORDER_CONSTANT);
            pComboBorder->addItem("BORDER_REFLECT", cv::BORDER_REFLECT);
            pComboBorder->addItem("BORDER_WRAP", cv::BORDER_WRAP);
            pComboBorder->addItem("BORDER_REFLECT_101", cv::BORDER_REFLECT_101);
            pComboBorder->addItem("BORDER_REFLECT101", cv::BORDER_REFLECT101);
            pComboBorder->addItem("BORDER_TRANSPARENT", cv::BORDER_TRANSPARENT);
            pComboBorder->addItem("BORDER_ISOLATED", cv::BORDER_ISOLATED);
            pComboBorder->addItem("BORDER_REPLICATE", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_borderType));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_ddepth = pComboDepth->currentData().toInt();
                m_pParam->m_dx= pSpinDx->value();
                m_pParam->m_dy = pSpinDy->value();
                m_pParam->m_ksize = pSpinSize->value();
                m_pParam->m_out_dtype = pComboType->currentData().toInt();
                m_pParam->m_scale= pSpinScale->value();
                m_pParam->m_delta= pSpinDelta->value();
                m_pParam->m_borderType = pComboBorder->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
        }

    private:

        std::shared_ptr<COcvRidgeFilterParam> m_pParam = nullptr;
};

class COcvWidgetRidgeFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetRidgeFilterFactory()
        {
            m_name = QObject::tr("Ridge Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRidgeFilter>(pParam);
        }
};
#endif // COCVWIDGETRIDGEFILTER_HPP
