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

#ifndef COCVWIDGETBRISK_HPP
#define COCVWIDGETBRISK_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvBRISK.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetBRISK : public CWorkflowTaskWidget
{
    public:

        COcvWidgetBRISK(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBRISK(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBRISKParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBRISKParam>();

            m_pSpinThresh = addSpin(0, tr("Threshold"), m_pParam->m_thresh, 0, INT_MAX);
            m_pSpinOctaves = addSpin(1, tr("Octaves"), m_pParam->m_octaves);
            m_pSpinScale = addDoubleSpin(2, tr("Pattern scale"), m_pParam->m_patternScale, 0.0, DBL_MAX, 0.1);
            m_pCheck = addCheck(3, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            m_pCheckDetect = addCheck(8, tr("Detect"), m_pParam->m_bDetect);
            m_pCheckDetect->setEnabled(false);
            m_pCheckCompute = addCheck(9, tr("Compute"), m_pParam->m_bCompute);

            connect(m_pCheck, &QCheckBox::clicked, [&](bool checked){
                if(checked)
                {
                    m_pCheckDetect->setEnabled(true);
                }
                else
                {
                    m_pCheckDetect->setChecked(true);
                    m_pCheckDetect->setEnabled(false);
                }
            });
        }

        void onApply() override
        {
            m_pParam->m_thresh = m_pSpinThresh->value();
            m_pParam->m_octaves = m_pSpinOctaves->value();
            m_pParam->m_patternScale = m_pSpinScale->value();
            m_pParam->m_bUseProvidedKeypoints = m_pCheck->isChecked();
            m_pParam->m_bDetect = m_pCheckDetect->isChecked();
            m_pParam->m_bCompute = m_pCheckCompute->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvBRISKParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinScale = nullptr;
        QSpinBox*       m_pSpinThresh = nullptr;
        QSpinBox*       m_pSpinOctaves = nullptr;
        QCheckBox*      m_pCheck = nullptr;
        QCheckBox*      m_pCheckDetect = nullptr;
        QCheckBox*      m_pCheckCompute = nullptr;
};

class COcvWidgetBRISKFactory : public CWidgetFactory
{
    public:

        COcvWidgetBRISKFactory()
        {
            m_name = "ocv_brisk";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBRISK>(pParam);
        }
};
#endif // COCVWIDGETBRISK_HPP
