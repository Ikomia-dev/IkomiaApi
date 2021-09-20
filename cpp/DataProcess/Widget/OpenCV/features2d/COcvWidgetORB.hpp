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

#ifndef COCVWIDGETORB_HPP
#define COCVWIDGETORB_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvORB.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetORB : public CWorkflowTaskWidget
{
    public:

        COcvWidgetORB(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        COcvWidgetORB(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvORBParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvORBParam>();

            m_pSpinFeatures = addSpin(0, tr("Features"), m_pParam->m_nfeatures);
            m_pSpinFeatures->setRange(0, INT_MAX);
            m_pSpinFeatures->setValue(m_pParam->m_nfeatures);

            m_pSpinScale = addDoubleSpin(1, tr("Scale factor"), m_pParam->m_scaleFactor);
            m_pSpinScale->setSingleStep(0.1);

            m_pSpinLevels = addSpin(2, tr("Levels"), m_pParam->m_nlevels);
            m_pSpinEdgeThresh = addSpin(3, tr("Edge threshold"), m_pParam->m_edgeThreshold);
            m_pSpinFirstLevel = addSpin(2, tr("First level"), m_pParam->m_firstLevel);
            m_pSpinWTA = addSpin(4, tr("WTA_K"), m_pParam->m_WTA_K);

            m_pCombo = addCombo(5, tr("Score Type"));
            m_pCombo->addItem("HARRIS_SCORE", cv::ORB::HARRIS_SCORE);
            m_pCombo->addItem("FAST_SCORE", cv::ORB::FAST_SCORE);
            m_pCombo->setCurrentIndex(m_pCombo->findData(m_pParam->m_scoreType));

            m_pSpinPatch = addSpin(6, tr("Patch size"), m_pParam->m_patchSize);
            m_pSpinFastThresh = addSpin(7, tr("Fast threshold"), m_pParam->m_fastThreshold);
            m_pCheck = addCheck(8, tr("Use provided keypoints"), m_pParam->m_bUseProvidedKeypoints);
            m_pCheckDetect = addCheck(9, tr("Detect"), m_pParam->m_bDetect);
            m_pCheckDetect->setEnabled(false);
            m_pCheckCompute = addCheck(10, tr("Compute"), m_pParam->m_bCompute);

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
            m_pParam->m_nfeatures = m_pSpinFeatures->value();
            m_pParam->m_scaleFactor = m_pSpinScale->value();
            m_pParam->m_nlevels = m_pSpinLevels->value();
            m_pParam->m_edgeThreshold = m_pSpinEdgeThresh->value();
            m_pParam->m_firstLevel = m_pSpinFirstLevel->value();
            m_pParam->m_WTA_K = m_pSpinWTA->value();
            m_pParam->m_scoreType = m_pCombo->currentData().toInt();
            m_pParam->m_patchSize = m_pSpinPatch->value();
            m_pParam->m_fastThreshold = m_pSpinFastThresh->value();
            m_pParam->m_bUseProvidedKeypoints = m_pCheck->isChecked();
            m_pParam->m_bDetect = m_pCheckDetect->isChecked();
            m_pParam->m_bCompute = m_pCheckCompute->isChecked();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvORBParam> m_pParam = nullptr;
        QSpinBox*       m_pSpinFeatures = nullptr;
        QSpinBox*       m_pSpinLevels = nullptr;
        QSpinBox*       m_pSpinEdgeThresh = nullptr;
        QSpinBox*       m_pSpinFirstLevel = nullptr;
        QSpinBox*       m_pSpinWTA = nullptr;
        QSpinBox*       m_pSpinPatch = nullptr;
        QSpinBox*       m_pSpinFastThresh = nullptr;
        QDoubleSpinBox* m_pSpinScale = nullptr;
        QComboBox*      m_pCombo = nullptr;
        QCheckBox*      m_pCheck = nullptr;
        QCheckBox*      m_pCheckDetect = nullptr;
        QCheckBox*      m_pCheckCompute = nullptr;
};

class COcvWidgetORBFactory : public CWidgetFactory
{
    public:

        COcvWidgetORBFactory()
        {
            m_name = "ocv_orb";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetORB>(pParam);
        }
};
#endif // COCVWIDGETORB_HPP
