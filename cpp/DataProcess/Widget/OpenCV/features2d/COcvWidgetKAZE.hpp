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

#ifndef COCVWIDGETKAZE_HPP
#define COCVWIDGETKAZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvKAZE.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetKAZE : public CWorkflowTaskWidget
{
    public:

        COcvWidgetKAZE(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetKAZE(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvKAZEParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvKAZEParam>();

            m_pCheckExt = addCheck(0, tr("Extended"), m_pParam->m_bExtended);
            m_pCheckUpright = addCheck(1, tr("Upright"), m_pParam->m_bUpright);
            m_pSpinThresh = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold, 0.0, DBL_MAX, 0.001, 3);
            m_pSpinOctaves = addSpin(3, tr("Octaves"), m_pParam->m_nOctaves);
            m_pSpinOctaveLayers = addSpin(4, tr("Octave layers"), m_pParam->m_nOctaveLayers);

            m_pComboDiff = addCombo(5, tr("Diffusivity"));
            m_pComboDiff->addItem("DIFF_PM_G1", cv::KAZE::DIFF_PM_G1);
            m_pComboDiff->addItem("DIFF_PM_G2", cv::KAZE::DIFF_PM_G2);
            m_pComboDiff->addItem("DIFF_WEICKERT", cv::KAZE::DIFF_WEICKERT);
            m_pComboDiff->addItem("DIFF_CHARBONNIER", cv::KAZE::DIFF_CHARBONNIER);
            m_pComboDiff->setCurrentIndex(m_pComboDiff->findData(m_pParam->m_diffusivity));

            m_pCheck = addCheck(6, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            m_pCheckDetect = addCheck(7, tr("Detect"), m_pParam->m_bDetect);
            m_pCheckDetect->setEnabled(false);
            m_pCheckCompute = addCheck(8, tr("Compute"), m_pParam->m_bCompute);

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
            m_pParam->m_bExtended = m_pCheckExt->isChecked();
            m_pParam->m_bUpright = m_pCheckUpright->isChecked();
            m_pParam->m_threshold = m_pSpinThresh->value();
            m_pParam->m_nOctaves = m_pSpinOctaves->value();
            m_pParam->m_nOctaveLayers = m_pSpinOctaveLayers->value();
            m_pParam->m_diffusivity = m_pComboDiff->currentData().toInt();
            m_pParam->m_bUseProvidedKeypoints = m_pCheck->isChecked();
            m_pParam->m_bDetect = m_pCheckDetect->isChecked();
            m_pParam->m_bCompute = m_pCheckCompute->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvKAZEParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinThresh = nullptr;
        QSpinBox*       m_pSpinOctaves = nullptr;
        QSpinBox*       m_pSpinOctaveLayers = nullptr;
        QComboBox*      m_pComboDiff = nullptr;
        QCheckBox*      m_pCheckExt = nullptr;
        QCheckBox*      m_pCheckUpright = nullptr;
        QCheckBox*      m_pCheck = nullptr;
        QCheckBox*      m_pCheckDetect = nullptr;
        QCheckBox*      m_pCheckCompute = nullptr;
};

class COcvWidgetKAZEFactory : public CWidgetFactory
{
    public:

        COcvWidgetKAZEFactory()
        {
            m_name = QObject::tr("KAZE").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetKAZE>(pParam);
        }
};
#endif // COCVWIDGETKAZE_HPP
