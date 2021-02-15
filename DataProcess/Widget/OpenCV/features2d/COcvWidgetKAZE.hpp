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

class COcvWidgetKAZE : public CProtocolTaskWidget
{
    public:

        COcvWidgetKAZE(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetKAZE(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvKAZEParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvKAZEParam>();

            auto pCheckExt = addCheck(0, tr("Extended"), m_pParam->m_bExtended);
            auto pCheckUpright = addCheck(1, tr("Upright"), m_pParam->m_bUpright);

            auto pSpinThresh = addDoubleSpin(2, tr("Threshold"), m_pParam->m_threshold);
            pSpinThresh->setDecimals(3);
            pSpinThresh->setSingleStep(0.001);
            pSpinThresh->setValue(m_pParam->m_threshold);
            auto pSpinOctaves = addSpin(3, tr("Octaves"), m_pParam->m_nOctaves);
            auto pSpinOctaveLayers = addSpin(4, tr("Octave layers"), m_pParam->m_nOctaveLayers);
            auto pComboDiff = addCombo(5, tr("Diffusivity"));
            pComboDiff->addItem("DIFF_PM_G1", cv::KAZE::DIFF_PM_G1);
            pComboDiff->addItem("DIFF_PM_G2", cv::KAZE::DIFF_PM_G2);
            pComboDiff->addItem("DIFF_WEICKERT", cv::KAZE::DIFF_WEICKERT);
            pComboDiff->addItem("DIFF_CHARBONNIER", cv::KAZE::DIFF_CHARBONNIER);
            pComboDiff->setCurrentIndex(pComboDiff->findData(m_pParam->m_diffusivity));
            auto pCheck = addCheck(6, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            auto pCheckDetect = addCheck(7, tr("Detect"), m_pParam->m_bDetect);
            pCheckDetect->setEnabled(false);
            auto pCheckCompute = addCheck(8, tr("Compute"), m_pParam->m_bCompute);

            connect(pCheck, &QCheckBox::clicked, [=](bool checked){
                if(checked)
                {
                    pCheckDetect->setEnabled(true);
                }
                else
                {
                    pCheckDetect->setChecked(true);
                    pCheckDetect->setEnabled(false);
                }
            });

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bExtended = pCheckExt->isChecked();
                m_pParam->m_bUpright = pCheckUpright->isChecked();
                m_pParam->m_threshold = pSpinThresh->value();
                m_pParam->m_nOctaves = pSpinOctaves->value();
                m_pParam->m_nOctaveLayers = pSpinOctaveLayers->value();
                m_pParam->m_diffusivity = pComboDiff->currentData().toInt();
                m_pParam->m_bUseProvidedKeypoints = pCheck->isChecked();
                m_pParam->m_bDetect = pCheckDetect->isChecked();
                m_pParam->m_bCompute = pCheckCompute->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvKAZEParam> m_pParam = nullptr;
};

class COcvWidgetKAZEFactory : public CWidgetFactory
{
    public:

        COcvWidgetKAZEFactory()
        {
            m_name = QObject::tr("KAZE").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetKAZE>(pParam);
        }
};
#endif // COCVWIDGETKAZE_HPP
