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

class COcvWidgetHoughCircles : public CProtocolTaskWidget
{
    public:

        COcvWidgetHoughCircles(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetHoughCircles(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvHoughCirclesParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvHoughCirclesParam>();

            auto pComboMethod = addCombo(tr("Method"));
            pComboMethod->addItem("Hough gradient", cv::HOUGH_GRADIENT);
            pComboMethod->addItem("Hough gradient variant", cv::HOUGH_GRADIENT_ALT);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_method));

            auto pSpinDp = addDoubleSpin(tr("Accumulator ratio"), m_pParam->m_dp, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMinDst = addDoubleSpin(tr("Min distance between centers"), m_pParam->m_minDist, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinParam1 = addDoubleSpin(tr("Canny high threshold"), m_pParam->m_param1, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinParam2 = addDoubleSpin(tr("Accumulator threshold"), m_pParam->m_param2, 0.0, DBL_MAX, 1.0, 1);
            auto pSpinMinRaduis = addSpin(tr("Minimum radius"), m_pParam->m_minRadius, 0, INT_MAX, 1);
            auto pSpinMaxRaduis = addSpin(tr("Maximum radius"), m_pParam->m_maxRadius, 0, INT_MAX, 1);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_method = pComboMethod->currentData().toInt();
                m_pParam->m_dp = pSpinDp->value();
                m_pParam->m_minDist = pSpinMinDst->value();
                m_pParam->m_param1 = pSpinParam1->value();
                m_pParam->m_param2 = pSpinParam2->value();
                m_pParam->m_minRadius = pSpinMinRaduis->value();
                m_pParam->m_maxRadius = pSpinMaxRaduis->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvHoughCirclesParam> m_pParam = nullptr;
};

class COcvWidgetHoughCirclesFactory : public CWidgetFactory
{
    public:

        COcvWidgetHoughCirclesFactory()
        {
            m_name = QObject::tr("Hough circles").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetHoughCircles>(pParam);
        }
};

#endif // COCVWIDGETHOUGHCIRCLES_HPP
