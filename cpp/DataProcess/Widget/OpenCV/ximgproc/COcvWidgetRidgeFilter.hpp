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

class COcvWidgetRidgeFilter : public CWorkflowTaskWidget
{
    public:

        COcvWidgetRidgeFilter(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetRidgeFilter(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvRidgeFilterParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvRidgeFilterParam>();

            m_pComboDepth = addCombo(0, tr("Output depth"));
            m_pComboDepth->addItem("Default", -1);
            m_pComboDepth->addItem("CV_8U", CV_8U);
            m_pComboDepth->addItem("CV_8S", CV_8S);
            m_pComboDepth->addItem("CV_16U", CV_16U);
            m_pComboDepth->addItem("CV_16S", CV_16S);
            m_pComboDepth->addItem("CV_32S", CV_32S);
            m_pComboDepth->addItem("CV_32F", CV_32F);
            m_pComboDepth->addItem("CV_64F", CV_64F);
            m_pComboDepth->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_ddepth));
            m_pSpinDx = addSpin(1, tr("dx"), m_pParam->m_dx);
            m_pSpinDy = addSpin(2, tr("dy"), m_pParam->m_dy);
            m_pSpinSize = addSpin(3, tr("ksize"), m_pParam->m_ksize);
            m_pComboType = addCombo(4, tr("Out dtype"));
            m_pComboType->addItem("Default", -1);
            m_pComboType->addItem("CV_8U", CV_8U);
            m_pComboType->addItem("CV_8S", CV_8S);
            m_pComboType->addItem("CV_16U", CV_16U);
            m_pComboType->addItem("CV_16S", CV_16S);
            m_pComboType->addItem("CV_32S", CV_32S);
            m_pComboType->addItem("CV_32F", CV_32F);
            m_pComboType->addItem("CV_64F", CV_64F);
            m_pComboType->setCurrentIndex(m_pComboDepth->findData(m_pParam->m_out_dtype));
            m_pSpinScale = addDoubleSpin(5, tr("Scale"), m_pParam->m_scale);
            m_pSpinDelta = addDoubleSpin(6, tr("Delta"), m_pParam->m_delta);
            m_pComboBorder = addCombo(7, tr("Border type"));
            m_pComboBorder->addItem("BORDER_DEFAULT", cv::BORDER_DEFAULT);
            m_pComboBorder->addItem("BORDER_CONSTANT", cv::BORDER_CONSTANT);
            m_pComboBorder->addItem("BORDER_REFLECT", cv::BORDER_REFLECT);
            m_pComboBorder->addItem("BORDER_WRAP", cv::BORDER_WRAP);
            m_pComboBorder->addItem("BORDER_REFLECT_101", cv::BORDER_REFLECT_101);
            m_pComboBorder->addItem("BORDER_REFLECT101", cv::BORDER_REFLECT101);
            m_pComboBorder->addItem("BORDER_TRANSPARENT", cv::BORDER_TRANSPARENT);
            m_pComboBorder->addItem("BORDER_ISOLATED", cv::BORDER_ISOLATED);
            m_pComboBorder->addItem("BORDER_REPLICATE", cv::BORDER_REPLICATE);
            m_pComboBorder->setCurrentIndex(m_pComboBorder->findData(m_pParam->m_borderType));
        }

        void onApply() override
        {
            m_pParam->m_ddepth = m_pComboDepth->currentData().toInt();
            m_pParam->m_dx= m_pSpinDx->value();
            m_pParam->m_dy = m_pSpinDy->value();
            m_pParam->m_ksize = m_pSpinSize->value();
            m_pParam->m_out_dtype = m_pComboType->currentData().toInt();
            m_pParam->m_scale= m_pSpinScale->value();
            m_pParam->m_delta= m_pSpinDelta->value();
            m_pParam->m_borderType = m_pComboBorder->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvRidgeFilterParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinScale = nullptr;
        QDoubleSpinBox* m_pSpinDelta = nullptr;
        QSpinBox*       m_pSpinDx = nullptr;
        QSpinBox*       m_pSpinDy = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
        QComboBox*      m_pComboDepth = nullptr;
        QComboBox*      m_pComboType = nullptr;
        QComboBox*      m_pComboBorder = nullptr;
};

class COcvWidgetRidgeFilterFactory : public CWidgetFactory
{
    public:

        COcvWidgetRidgeFilterFactory()
        {
            m_name = "ocv_ridge_filter";
        }

        virtual WorkflowTaskWidgetPtr create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetRidgeFilter>(pParam);
        }
};

#endif // COCVWIDGETRIDGEFILTER_HPP
