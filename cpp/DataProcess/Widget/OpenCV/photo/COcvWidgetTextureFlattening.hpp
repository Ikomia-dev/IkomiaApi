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

#ifndef COCVWIDGETTEXTUREFLATTENING_HPP
#define COCVWIDGETTEXTUREFLATTENING_HPP
#include "../../../Process/OpenCV/photo/COcvTextureFlattening.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetTextureFlattening : public CWorkflowTaskWidget
{
    public:

        COcvWidgetTextureFlattening(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetTextureFlattening(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvTextureFlatteningParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvTextureFlatteningParam>();

            auto pLabelLow = new QLabel(tr("Low threshold"));
            auto pLabelHigh = new QLabel(tr("High threshold"));
            auto pLabelKer = new QLabel(tr("Kernel size"));

            m_pSpinLow = new QDoubleSpinBox;
            m_pSpinHigh = new QDoubleSpinBox;
            m_pSpinKer = new QDoubleSpinBox;

            m_pSpinLow->setValue(m_pParam->m_lowThresh);
            m_pSpinLow->setRange(0, 100);
            m_pSpinLow->setSingleStep(1);

            m_pSpinHigh->setValue(m_pParam->m_highThresh);
            m_pSpinHigh->setRange(0, 100);
            m_pSpinHigh->setSingleStep(1);

            m_pSpinKer->setValue(m_pParam->m_kernelSize);
            m_pSpinKer->setRange(0, 100);
            m_pSpinKer->setSingleStep(1);

            m_pLayout->addWidget(pLabelLow, 0, 0);
            m_pLayout->addWidget(m_pSpinLow, 0, 1);

            m_pLayout->addWidget(pLabelHigh, 1, 0);
            m_pLayout->addWidget(m_pSpinHigh, 1, 1);

            m_pLayout->addWidget(pLabelKer, 2, 0);
            m_pLayout->addWidget(m_pSpinKer, 2, 1);
        }

        void onApply() override
        {
            m_pParam->m_lowThresh = m_pSpinLow->value();
            m_pParam->m_highThresh = m_pSpinHigh->value();
            m_pParam->m_kernelSize = m_pSpinKer->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvTextureFlatteningParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinLow = nullptr;
        QDoubleSpinBox* m_pSpinHigh = nullptr;
        QDoubleSpinBox* m_pSpinKer = nullptr;
};

class COcvWidgetTextureFlatteningFactory : public CWidgetFactory
{
    public:

        COcvWidgetTextureFlatteningFactory()
        {
            m_name = "ocv_texture_flattening";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTextureFlattening>(pParam);
        }
};

#endif // COCVWIDGETTEXTUREFLATTENING_HPP
