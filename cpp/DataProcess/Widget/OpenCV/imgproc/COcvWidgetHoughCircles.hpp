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

#ifndef COCVWIDGETHOUGHCIRCLES_HPP
#define COCVWIDGETHOUGHCIRCLES_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvHoughCircles.hpp"

class COcvWidgetHoughCircles : public CWorkflowTaskWidget
{
    public:

        COcvWidgetHoughCircles(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }
        COcvWidgetHoughCircles(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHoughCirclesParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHoughCirclesParam>();

            m_pComboMethod = addCombo(tr("Method"));
            m_pComboMethod->addItem("Hough gradient", cv::HOUGH_GRADIENT);
            m_pComboMethod->addItem("Hough gradient variant", cv::HOUGH_GRADIENT_ALT);
            m_pComboMethod->setCurrentIndex(m_pComboMethod->findData(m_pParam->m_method));

            m_pSpinDp = addDoubleSpin(tr("Accumulator ratio"), m_pParam->m_dp, 0.0, DBL_MAX, 1.0, 1);
            m_pSpinMinDst = addDoubleSpin(tr("Min distance between centers"), m_pParam->m_minDist, 0.0, DBL_MAX, 1.0, 1);
            m_pSpinParam1 = addDoubleSpin(tr("Canny high threshold"), m_pParam->m_param1, 0.0, DBL_MAX, 1.0, 1);
            m_pSpinParam2 = addDoubleSpin(tr("Accumulator threshold"), m_pParam->m_param2, 0.0, DBL_MAX, 1.0, 1);
            m_pSpinMinRaduis = addSpin(tr("Minimum radius"), m_pParam->m_minRadius, 0, INT_MAX, 1);
            m_pSpinMaxRaduis = addSpin(tr("Maximum radius"), m_pParam->m_maxRadius, 0, INT_MAX, 1);
        }

        void onApply() override
        {
            m_pParam->m_method = m_pComboMethod->currentData().toInt();
            m_pParam->m_dp = m_pSpinDp->value();
            m_pParam->m_minDist = m_pSpinMinDst->value();
            m_pParam->m_param1 = m_pSpinParam1->value();
            m_pParam->m_param2 = m_pSpinParam2->value();
            m_pParam->m_minRadius = m_pSpinMinRaduis->value();
            m_pParam->m_maxRadius = m_pSpinMaxRaduis->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvHoughCirclesParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinDp = nullptr;
        QDoubleSpinBox* m_pSpinMinDst = nullptr;
        QDoubleSpinBox* m_pSpinParam1 = nullptr;
        QDoubleSpinBox* m_pSpinParam2 = nullptr;
        QSpinBox*       m_pSpinMinRaduis = nullptr;
        QSpinBox*       m_pSpinMaxRaduis = nullptr;
        QComboBox*      m_pComboMethod = nullptr;
};

class COcvWidgetHoughCirclesFactory : public CWidgetFactory
{
    public:

        COcvWidgetHoughCirclesFactory()
        {
            m_name = QObject::tr("Hough circles").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetHoughCircles>(pParam);
        }
};

#endif // COCVWIDGETHOUGHCIRCLES_HPP
