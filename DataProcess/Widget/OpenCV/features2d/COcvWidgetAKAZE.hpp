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

#ifndef COCVWIDGETAKAZE_HPP
#define COCVWIDGETAKAZE_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvAKAZE.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetAKAZE : public CProtocolTaskWidget
{
    public:

        COcvWidgetAKAZE(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAKAZE(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAKAZEParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAKAZEParam>();

            auto pComboDescType = addCombo(0, tr("Descriptor type"));
            pComboDescType->addItem("DESCRIPTOR_KAZE_UPRIGHT", cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT);
            pComboDescType->addItem("DESCRIPTOR_KAZE", cv::AKAZE::DESCRIPTOR_KAZE);
            pComboDescType->addItem("DESCRIPTOR_MLDB_UPRIGHT", cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT);
            pComboDescType->addItem("DESCRIPTOR_MLDB", cv::AKAZE::DESCRIPTOR_MLDB);
            pComboDescType->setCurrentIndex(pComboDescType->findData(m_pParam->m_descriptor_type));
            auto pSpinDescSize = addSpin(1, tr("Descriptor size"), m_pParam->m_descriptor_size);
            auto pSpinDescCh = addSpin(2, tr("Descriptor channels"), m_pParam->m_descriptor_channels);
            auto pSpinThresh = addDoubleSpin(3, tr("Threshold"), m_pParam->m_threshold);
            pSpinThresh->setDecimals(3);
            pSpinThresh->setSingleStep(0.001);
            pSpinThresh->setValue(m_pParam->m_threshold);
            auto pSpinOctaves = addSpin(4, tr("Octaves"), m_pParam->m_nOctaves);
            auto pSpinOctaveLayers = addSpin(5, tr("Octave layers"), m_pParam->m_nOctaveLayers);
            auto pComboDiff = addCombo(6, tr("Diffusivity"));
            pComboDiff->addItem("DIFF_PM_G1", cv::KAZE::DIFF_PM_G1);
            pComboDiff->addItem("DIFF_PM_G2", cv::KAZE::DIFF_PM_G2);
            pComboDiff->addItem("DIFF_WEICKERT", cv::KAZE::DIFF_WEICKERT);
            pComboDiff->addItem("DIFF_CHARBONNIER", cv::KAZE::DIFF_CHARBONNIER);
            pComboDiff->setCurrentIndex(pComboDiff->findData(m_pParam->m_diffusivity));
            auto pCheck = addCheck(7, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            auto pCheckDetect = addCheck(8, tr("Detect"), m_pParam->m_bDetect);
            pCheckDetect->setEnabled(false);
            auto pCheckCompute = addCheck(9, tr("Compute"), m_pParam->m_bCompute);

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
                m_pParam->m_descriptor_type = pComboDescType->currentData().toInt();
                m_pParam->m_descriptor_size = pSpinDescSize->value();
                m_pParam->m_descriptor_channels = pSpinDescCh->value();
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

        std::shared_ptr<COcvAKAZEParam> m_pParam = nullptr;
};

class COcvWidgetAKAZEFactory : public CWidgetFactory
{
    public:

        COcvWidgetAKAZEFactory()
        {
            m_name = QObject::tr("AKAZE").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAKAZE>(pParam);
        }
};
#endif // COCVWIDGETAKAZE_HPP
