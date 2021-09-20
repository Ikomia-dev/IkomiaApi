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

class COcvWidgetFarnebackOF : public CWorkflowTaskWidget
{
    public:

        COcvWidgetFarnebackOF(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetFarnebackOF(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvFarnebackOFParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvFarnebackOFParam>();

            m_pSpinLevels = addSpin(0, tr("Levels"), m_pParam->m_numLevels);
            m_pSpinScale = addDoubleSpin(1, tr("Pyramid scale"), m_pParam->m_pyrScale);
            m_pCheckFastPyr = addCheck(2, tr("Fast pyramids"), m_pParam->m_fastPyramids);
            m_pSpinSize = addSpin(3, tr("Window size"), m_pParam->m_winSize);
            m_pSpinIter = addSpin(4, tr("Iterations"), m_pParam->m_numIters);
            m_pSpinPolyN = addSpin(5, tr("PolyN"), m_pParam->m_polyN);
            m_pSpinSigma = addDoubleSpin(6, tr("PolySigma"), m_pParam->m_polySigma);

            m_pCheck = addCheck(7, tr("Use OpenCL"), m_pParam->m_bUseOCL);
        }

        void onApply() override
        {
            m_pParam->m_numLevels = m_pSpinLevels->value();
            m_pParam->m_pyrScale = m_pSpinScale->value();
            m_pParam->m_fastPyramids = m_pCheckFastPyr->isChecked();
            m_pParam->m_winSize = m_pSpinSize->value();
            m_pParam->m_numIters = m_pSpinIter->value();
            m_pParam->m_polyN = m_pSpinPolyN->value();
            m_pParam->m_polySigma = m_pSpinSigma->value();
            m_pParam->m_bUseOCL = m_pCheck->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvFarnebackOFParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinScale = nullptr;
        QDoubleSpinBox* m_pSpinSigma = nullptr;
        QSpinBox*       m_pSpinLevels = nullptr;
        QSpinBox*       m_pSpinSize = nullptr;
        QSpinBox*       m_pSpinIter = nullptr;
        QSpinBox*       m_pSpinPolyN = nullptr;
        QCheckBox*      m_pCheck = nullptr;
        QCheckBox*      m_pCheckFastPyr = nullptr;
};

class COcvWidgetFarnebackOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetFarnebackOFFactory()
        {
            m_name = "ocv_farneback_flow";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetFarnebackOF>(pParam);
        }
};

#endif // COCVWIDGETFARNEBACKOPTICALFLOW_HPP
