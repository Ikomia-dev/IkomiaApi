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

#ifndef COCVWIDGETGFTT_HPP
#define COCVWIDGETGFTT_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvGFTT.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetGFTT : public CWorkflowTaskWidget
{
    public:

        COcvWidgetGFTT(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetGFTT(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGFTTParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGFTTParam>();

            m_pSpinCorners = addSpin(0, tr("Max corners"), m_pParam->m_maxCorners, 0, INT_MAX);
            m_pSpinQuality = addDoubleSpin(1, tr("Quality level"), m_pParam->m_qualityLevel, 0.0, DBL_MAX, 0.01);
            m_pSpinDist = addDoubleSpin(2, tr("Min distance"), m_pParam->m_minDistance);
            m_pSpinBlockSize = addSpin(3, tr("Block size"), m_pParam->m_blockSize);
            m_pCheckHarris = addCheck(4, tr("Use Harris"), m_pParam->m_bUseHarrisDetector);
            m_pSpinCoeff = addDoubleSpin(5, tr("Harris coefficient"), m_pParam->m_k);
        }

        void onApply() override
        {
            m_pParam->m_maxCorners = m_pSpinCorners->value();
            m_pParam->m_qualityLevel = m_pSpinQuality->value();
            m_pParam->m_minDistance = m_pSpinDist->value();
            m_pParam->m_blockSize = m_pSpinBlockSize->value();
            m_pParam->m_bUseHarrisDetector = m_pCheckHarris->isChecked();
            m_pParam->m_k = m_pSpinCoeff->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvGFTTParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinQuality = nullptr;
        QDoubleSpinBox* m_pSpinDist = nullptr;
        QDoubleSpinBox* m_pSpinCoeff = nullptr;
        QSpinBox*       m_pSpinCorners = nullptr;
        QSpinBox*       m_pSpinBlockSize = nullptr;
        QCheckBox*      m_pCheckHarris = nullptr;
};

class COcvWidgetGFTTFactory : public CWidgetFactory
{
    public:

        COcvWidgetGFTTFactory()
        {
            m_name = QObject::tr("GFTTDetector").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGFTT>(pParam);
        }
};
#endif // COCVWIDGETGFTT_HPP
