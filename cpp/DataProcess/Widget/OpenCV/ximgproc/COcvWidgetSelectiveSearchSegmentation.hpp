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

#ifndef COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP
#define COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSelectiveSearchSegmentation.hpp"

class COcvWidgetSelectiveSearchSegmentation : public CWorkflowTaskWidget
{
    public:

        COcvWidgetSelectiveSearchSegmentation(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
            initConnections();
        }
        COcvWidgetSelectiveSearchSegmentation(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSelectiveSearchSegmentationParam>(pParam);
            init();
            initConnections();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSelectiveSearchSegmentationParam>();

            m_pSpinSigma = addDoubleSpin(0, tr("Sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            m_pSpinBaseK = addDoubleSpin(1, tr("First K value"), m_pParam->m_baseK);
            m_pSpinStepK = addSpin(2, tr("Step for K value"), m_pParam->m_stepK);
            m_pSpinRectCount = addSpin(3, tr("Number of objects"), m_pParam->m_nbRects);

            m_pComboInit = addCombo(4, "Initialization method");
            m_pComboInit->addItem("Single", COcvSelectiveSearchSegmentationParam::SINGLE);
            m_pComboInit->addItem("Fast", COcvSelectiveSearchSegmentationParam::FAST);
            m_pComboInit->addItem("Quality", COcvSelectiveSearchSegmentationParam::QUALITY);
            m_pComboInit->setCurrentIndex(m_pComboInit->findData(m_pParam->m_initMethod));

            m_pCheckDefaultStrategy = new QCheckBox(tr("Default strategy"));
            m_pCheckDefaultStrategy->setChecked(m_pParam->m_bDefaultStrategy);

            m_pCheckColorStrategy = new QCheckBox(tr("Color strategy"));
            m_pCheckColorStrategy->setChecked(m_pParam->m_bColorStrategy);
            m_pSpinColorStrategy = new QDoubleSpinBox;
            m_pSpinColorStrategy->setDecimals(2);
            m_pSpinColorStrategy->setRange(0.0, 1.0);
            m_pSpinColorStrategy->setSingleStep(0.25);
            m_pSpinColorStrategy->setValue(m_pParam->m_colorWeight);

            m_pCheckFillStrategy = new QCheckBox(tr("Fill strategy"));
            m_pCheckFillStrategy->setChecked(m_pParam->m_bColorStrategy);
            m_pSpinFillStrategy = new QDoubleSpinBox;
            m_pSpinFillStrategy->setDecimals(2);
            m_pSpinFillStrategy->setRange(0.0, 1.0);
            m_pSpinFillStrategy->setSingleStep(0.25);
            m_pSpinFillStrategy->setValue(m_pParam->m_fillWeight);

            m_pCheckSizeStrategy = new QCheckBox(tr("Size strategy"));
            m_pCheckSizeStrategy->setChecked(m_pParam->m_bColorStrategy);
            m_pSpinSizeStrategy = new QDoubleSpinBox;
            m_pSpinSizeStrategy->setDecimals(2);
            m_pSpinSizeStrategy->setRange(0.0, 1.0);
            m_pSpinSizeStrategy->setSingleStep(0.25);
            m_pSpinSizeStrategy->setValue(m_pParam->m_sizeWeight);

            m_pCheckTextureStrategy = new QCheckBox(tr("Texture strategy"));
            m_pCheckTextureStrategy->setChecked(m_pParam->m_bColorStrategy);
            m_pSpinTextureStrategy = new QDoubleSpinBox;
            m_pSpinTextureStrategy->setDecimals(2);
            m_pSpinTextureStrategy->setRange(0.0, 1.0);
            m_pSpinTextureStrategy->setSingleStep(0.25);
            m_pSpinTextureStrategy->setValue(m_pParam->m_sizeWeight);

            auto pStrategyLayout = new QGridLayout;
            pStrategyLayout->addWidget(m_pCheckDefaultStrategy, 0, 0, 1, 2);
            pStrategyLayout->addWidget(m_pCheckColorStrategy, 1, 0);
            pStrategyLayout->addWidget(m_pSpinColorStrategy, 1, 1);
            pStrategyLayout->addWidget(m_pCheckFillStrategy, 2, 0);
            pStrategyLayout->addWidget(m_pSpinFillStrategy, 2, 1);
            pStrategyLayout->addWidget(m_pCheckSizeStrategy, 3, 0);
            pStrategyLayout->addWidget(m_pSpinSizeStrategy, 3, 1);
            pStrategyLayout->addWidget(m_pCheckTextureStrategy, 4, 0);
            pStrategyLayout->addWidget(m_pSpinTextureStrategy, 4, 1);

            auto pGroupBox = new QGroupBox(tr("Strategy: choose one or several of them"));
            pGroupBox->setLayout(pStrategyLayout);

            m_pLayout->addWidget(pGroupBox, 5, 0, 1, 2);
        }

        void    initConnections()
        {
            connect(m_pCheckDefaultStrategy, &QCheckBox::clicked, [&]
            {
                if(m_pCheckDefaultStrategy->isChecked())
                {
                    m_pCheckColorStrategy->setChecked(false);
                    m_pCheckFillStrategy->setChecked(false);
                    m_pCheckSizeStrategy->setChecked(false);
                    m_pCheckTextureStrategy->setChecked(false);
                }
                else
                    m_pCheckColorStrategy->setChecked(true);
            });

            connect(m_pCheckColorStrategy, &QCheckBox::clicked, [&]
            {
                if(m_pCheckColorStrategy->isChecked())
                    m_pCheckDefaultStrategy->setChecked(false);
                else
                {
                    if(m_pCheckFillStrategy->isChecked() == false &&
                       m_pCheckSizeStrategy->isChecked() == false &&
                       m_pCheckTextureStrategy->isChecked() == false)
                    {
                        m_pCheckDefaultStrategy->setChecked(true);
                    }
                }
            });
            connect(m_pCheckFillStrategy, &QCheckBox::clicked, [&]
            {
                if(m_pCheckFillStrategy->isChecked())
                    m_pCheckDefaultStrategy->setChecked(false);
                else
                {
                    if(m_pCheckColorStrategy->isChecked() == false &&
                       m_pCheckSizeStrategy->isChecked() == false &&
                       m_pCheckTextureStrategy->isChecked() == false)
                    {
                        m_pCheckDefaultStrategy->setChecked(true);
                    }
                }
            });
            connect(m_pCheckSizeStrategy, &QCheckBox::clicked, [&]
            {
                if(m_pCheckSizeStrategy->isChecked())
                    m_pCheckDefaultStrategy->setChecked(false);
                else
                {
                    if(m_pCheckColorStrategy->isChecked() == false &&
                       m_pCheckFillStrategy->isChecked() == false &&
                       m_pCheckTextureStrategy->isChecked() == false)
                    {
                        m_pCheckDefaultStrategy->setChecked(true);
                    }
                }
            });
            connect(m_pCheckTextureStrategy, &QCheckBox::clicked, [&]
            {
                if(m_pCheckSizeStrategy->isChecked())
                    m_pCheckDefaultStrategy->setChecked(false);
                else
                {
                    if(m_pCheckColorStrategy->isChecked() == false &&
                       m_pCheckFillStrategy->isChecked() == false &&
                       m_pCheckSizeStrategy->isChecked() == false)
                    {
                        m_pCheckDefaultStrategy->setChecked(true);
                    }
                }
            });
        }

        void onApply() override
        {
            m_pParam->m_sigma = m_pSpinSigma->value();
            m_pParam->m_baseK = m_pSpinBaseK->value();
            m_pParam->m_stepK = m_pSpinStepK->value();
            m_pParam->m_nbRects = m_pSpinRectCount->value();
            m_pParam->m_initMethod = m_pComboInit->currentData().toInt();
            m_pParam->m_bDefaultStrategy = m_pCheckDefaultStrategy->isChecked();
            m_pParam->m_bColorStrategy = m_pCheckColorStrategy->isChecked();
            m_pParam->m_colorWeight = m_pSpinColorStrategy->value();
            m_pParam->m_bFillStrategy = m_pCheckFillStrategy->isChecked();
            m_pParam->m_fillWeight = m_pSpinFillStrategy->value();
            m_pParam->m_bSizeStrategy = m_pCheckSizeStrategy->isChecked();
            m_pParam->m_sizeWeight = m_pSpinSizeStrategy->value();
            m_pParam->m_bTextureStrategy = m_pCheckTextureStrategy->isChecked();
            m_pParam->m_textureWeight = m_pSpinTextureStrategy->value();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<COcvSelectiveSearchSegmentationParam> m_pParam = nullptr;
        QCheckBox*      m_pCheckDefaultStrategy = nullptr;
        QCheckBox*      m_pCheckColorStrategy = nullptr;
        QCheckBox*      m_pCheckFillStrategy = nullptr;
        QCheckBox*      m_pCheckSizeStrategy = nullptr;
        QCheckBox*      m_pCheckTextureStrategy = nullptr;
        QDoubleSpinBox* m_pSpinSigma = nullptr;
        QDoubleSpinBox* m_pSpinBaseK = nullptr;
        QDoubleSpinBox* m_pSpinColorStrategy = nullptr;
        QDoubleSpinBox* m_pSpinFillStrategy = nullptr;
        QDoubleSpinBox* m_pSpinSizeStrategy = nullptr;
        QDoubleSpinBox* m_pSpinTextureStrategy = nullptr;
        QSpinBox*       m_pSpinStepK = nullptr;
        QSpinBox*       m_pSpinRectCount = nullptr;
        QComboBox*      m_pComboInit = nullptr;
};

class COcvWidgetSelectiveSearchSegmentationFactory : public CWidgetFactory
{
    public:

        COcvWidgetSelectiveSearchSegmentationFactory()
        {
            m_name = "ocv_selective_search_segmentation";
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSelectiveSearchSegmentation>(pParam);
        }
};

#endif // COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP
