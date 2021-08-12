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

#ifndef COCVWIDGETSUPERPIXELSEEDS_HPP
#define COCVWIDGETSUPERPIXELSEEDS_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSuperpixelSEEDS.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSuperpixelSEEDS : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSuperpixelSEEDS(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperpixelSEEDS(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperpixelSEEDSParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperpixelSEEDSParam>();

            m_pSpinPixels = addSpin(0, tr("Superpixels"), m_pParam->m_num_superpixels);
            m_pSpinLevels = addSpin(1, tr("Levels"), m_pParam->m_num_levels, 0, 10);
            m_pSpinPrior = addSpin(2, tr("Prior"), m_pParam->m_prior, 0, 5);
            m_pSpinBins = addSpin(3, tr("Histogram bins"), m_pParam->m_histogram_bins);
            m_pSpinIter = addSpin(4, tr("Iterations"), m_pParam->m_num_iterations, 0, 12);
        }

        void onApply() override
        {
            m_pParam->m_num_superpixels = m_pSpinPixels->value();
            m_pParam->m_num_levels = m_pSpinLevels->value();
            m_pParam->m_prior = m_pSpinPrior->value();
            m_pParam->m_histogram_bins = m_pSpinBins->value();
            m_pParam->m_num_iterations = m_pSpinIter->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSuperpixelSEEDSParam> m_pParam = nullptr;
        QSpinBox* m_pSpinPixels = nullptr;
        QSpinBox* m_pSpinLevels = nullptr;
        QSpinBox* m_pSpinPrior = nullptr;
        QSpinBox* m_pSpinBins = nullptr;
        QSpinBox* m_pSpinIter = nullptr;
};

class COcvWidgetSuperpixelSEEDSFactory : public CWidgetFactory
{
    public:

        COcvWidgetSuperpixelSEEDSFactory()
        {
            m_name = QObject::tr("SuperpixelSEEDS").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperpixelSEEDS>(pParam);
        }
};

#endif // COCVWIDGETSUPERPIXELSEEDS_HPP
