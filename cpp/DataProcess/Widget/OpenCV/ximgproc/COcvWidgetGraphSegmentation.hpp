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

#ifndef COCVWIDGETGRAPHSEGMENTATION_HPP
#define COCVWIDGETGRAPHSEGMENTATION_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGraphSegmentation.hpp"

class COcvWidgetGraphSegmentation : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGraphSegmentation(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGraphSegmentation(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGraphSegmentationParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGraphSegmentationParam>();

            auto pSpinSigma = addDoubleSpin(0, tr("Sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            auto pSpinK = addDoubleSpin(1, tr("K"), m_pParam->m_k);
            auto pSpinMinSize = addSpin(2, tr("Minimum size"), m_pParam->m_minSize);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigma = pSpinSigma->value();
                m_pParam->m_k = pSpinK->value();
                m_pParam->m_minSize = pSpinMinSize->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvGraphSegmentationParam> m_pParam = nullptr;
};

class COcvWidgetGraphSegmentationFactory : public CWidgetFactory
{
    public:

        COcvWidgetGraphSegmentationFactory()
        {
            m_name = QObject::tr("Graph Segmentation").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGraphSegmentation>(pParam);
        }
};

#endif // COCVWIDGETGRAPHSEGMENTATION_HPP
