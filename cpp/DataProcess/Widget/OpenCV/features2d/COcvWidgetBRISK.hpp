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

class COcvWidgetBRISK : public CProtocolTaskWidget
{
    public:

        COcvWidgetBRISK(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetBRISK(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvBRISKParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvBRISKParam>();

            auto pSpinThresh = addSpin(0, tr("Threshold"), m_pParam->m_thresh);
            pSpinThresh->setRange(0, INT_MAX);
            auto pSpinOctaves = addSpin(1, tr("Octaves"), m_pParam->m_octaves);
            auto pSpinScale = addDoubleSpin(2, tr("Pattern scale"), m_pParam->m_patternScale);
            pSpinScale->setSingleStep(0.1);
            auto pCheck = addCheck(3, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
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
                m_pParam->m_thresh = pSpinThresh->value();
                m_pParam->m_octaves = pSpinOctaves->value();
                m_pParam->m_patternScale = pSpinScale->value();
                m_pParam->m_bUseProvidedKeypoints = pCheck->isChecked();
                m_pParam->m_bDetect = pCheckDetect->isChecked();
                m_pParam->m_bCompute = pCheckCompute->isChecked();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvBRISKParam> m_pParam = nullptr;
};

class COcvWidgetBRISKFactory : public CWidgetFactory
{
    public:

        COcvWidgetBRISKFactory()
        {
            m_name = QObject::tr("BRISK").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetBRISK>(pParam);
        }
};
#endif // COCVWIDGETBRISK_HPP
