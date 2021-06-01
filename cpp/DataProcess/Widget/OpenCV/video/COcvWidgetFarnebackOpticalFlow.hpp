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

#ifndef COCVWIDGETFARNEBACKOPTICALFLOW_HPP
#define COCVWIDGETFARNEBACKOPTICALFLOW_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/video/COcvFarnebackOpticalFlow.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetFarnebackOF : public CProtocolTaskWidget
{
    public:

        COcvWidgetFarnebackOF(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFarnebackOF(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFarnebackOFParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFarnebackOFParam>();

            auto pSpinLevels = addSpin(0, tr("Levels"), m_pParam->m_numLevels);
            auto pSpinScale = addDoubleSpin(1, tr("Pyramid scale"), m_pParam->m_pyrScale);
            auto pCheckFastPyr = addCheck(2, tr("Fast pyramids"), m_pParam->m_fastPyramids);
            auto pSpinSize = addSpin(3, tr("Window size"), m_pParam->m_winSize);
            auto pSpinIter = addSpin(4, tr("Iterations"), m_pParam->m_numIters);
            auto pSpinPolyN = addSpin(5, tr("PolyN"), m_pParam->m_polyN);
            auto pSpinSigma = addDoubleSpin(6, tr("PolySigma"), m_pParam->m_polySigma);

            auto pCheck = addCheck(7, tr("Use OpenCL"), m_pParam->m_bUseOCL);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_numLevels = pSpinLevels->value();
                m_pParam->m_pyrScale = pSpinScale->value();
                m_pParam->m_fastPyramids = pCheckFastPyr->isChecked();
                m_pParam->m_winSize = pSpinSize->value();
                m_pParam->m_numIters = pSpinIter->value();
                m_pParam->m_polyN = pSpinPolyN->value();
                m_pParam->m_polySigma = pSpinSigma->value();
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvFarnebackOFParam> m_pParam = nullptr;
        //QSpinBox*                           m_pSpin = nullptr;
};

class COcvWidgetFarnebackOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetFarnebackOFFactory()
        {
            m_name = QObject::tr("Farneback Optical Flow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFarnebackOF>(pParam);
        }
};

#endif // COCVWIDGETFARNEBACKOPTICALFLOW_HPP
