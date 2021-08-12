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

#ifndef CGMICWIDGETCOLORPRESETS_HPP
#define CGMICWIDGETCOLORPRESETS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicColorPresets.hpp"

class CGmicWidgetColorPresets : public CWorkflowTaskWidget
{
    public:

        CGmicWidgetColorPresets(QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetColorPresets(std::shared_ptr<CWorkflowTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CWorkflowTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicColorPresetsParam>(pParam);
            init();
        }

    protected:

        void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicColorPresetsParam>();

            m_pComboCategory = addCombo(0, tr("Category"));
            for(size_t i=0; i<m_pParam->m_categories.size(); ++i)
                m_pComboCategory->addItem(QString::fromStdString(m_pParam->m_categories[i]));

            m_pComboCategory->setCurrentIndex(m_pParam->m_category);

            m_pComboPreset = addCombo(1, tr("Presets"));
            for(size_t i=0; i<m_pParam->m_presets[0].size(); ++i)
                m_pComboPreset->addItem(QString::fromStdString(m_pParam->m_presets[0][i]));

            m_pComboPreset->setCurrentIndex(m_pParam->m_preset);

            m_pSpinThumbSize = addSpin(2, tr("Thumbnail size"), m_pParam->m_thumbSize, 0, 1024);
            m_pSpinStrength = addDoubleSpin(3, tr("Strength(%)"), m_pParam->m_strenght, 0, 100, 1.0);
            m_pSpinBrightness = addDoubleSpin(4, tr("Brightness(%)"), m_pParam->m_brightness, -100, 100, 1.0);
            m_pSpinContrast = addDoubleSpin(5, tr("Contrast(%)"), m_pParam->m_contrast, -100, 100, 1.0);
            m_pSpinGamma = addDoubleSpin(6, tr("Gamma(%)"), m_pParam->m_gamma, -100, 100, 1.0);
            m_pSpinHue = addDoubleSpin(7, tr("Hue(%)"), m_pParam->m_hue, -100, 100, 1.0);
            m_pSpinSaturation = addDoubleSpin(8, tr("Saturation(%)"), m_pParam->m_saturation, -100, 100, 1.0);

            m_pComboNormColors = addCombo(9, tr("Normalize colors"));
            m_pComboNormColors->addItem(tr("None"), CGmicColorPresetsParam::NONE);
            m_pComboNormColors->addItem(tr("Pre-normalyze"), CGmicColorPresetsParam::PRE_NORMALIZE);
            m_pComboNormColors->addItem(tr("Post-normalyze"), CGmicColorPresetsParam::POST_NORMALIZE);
            m_pComboNormColors->addItem(tr("Both"), CGmicColorPresetsParam::BOTH);

            connect(m_pComboCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index)
            {
                 m_pComboPreset->clear();

                 for(size_t i=0; i<m_pParam->m_presets[index].size(); ++i)
                     m_pComboPreset->addItem(QString::fromStdString(m_pParam->m_presets[index][i]));

                 m_pComboPreset->setCurrentIndex(1);
            });
        }

        void onApply() override
        {
            m_pParam->m_category = m_pComboCategory->currentIndex();
            m_pParam->m_preset = m_pComboPreset->currentIndex();
            m_pParam->m_thumbSize = m_pSpinThumbSize->value();
            m_pParam->m_strenght = m_pSpinStrength->value();
            m_pParam->m_brightness = m_pSpinBrightness->value();
            m_pParam->m_contrast = m_pSpinContrast->value();
            m_pParam->m_gamma = m_pSpinGamma->value();
            m_pParam->m_hue = m_pSpinHue->value();
            m_pParam->m_saturation = m_pSpinSaturation->value();
            m_pParam->m_normalizeColors = m_pComboNormColors->currentData().toInt();
            emit doApplyProcess(m_pParam);
        }

    private:

        std::shared_ptr<CGmicColorPresetsParam> m_pParam = nullptr;
        QDoubleSpinBox* m_pSpinStrength = nullptr;
        QDoubleSpinBox* m_pSpinBrightness = nullptr;
        QDoubleSpinBox* m_pSpinContrast = nullptr;
        QDoubleSpinBox* m_pSpinGamma = nullptr;
        QDoubleSpinBox* m_pSpinHue = nullptr;
        QDoubleSpinBox* m_pSpinSaturation = nullptr;
        QSpinBox*       m_pSpinThumbSize = nullptr;
        QComboBox*      m_pComboCategory = nullptr;
        QComboBox*      m_pComboPreset = nullptr;
        QComboBox*      m_pComboNormColors = nullptr;
};

class CGmicWidgetColorPresetsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetColorPresetsFactory()
        {
            m_name = QObject::tr("Color presets").toStdString();
        }

        virtual WorkflowTaskWidgetPtr   create(std::shared_ptr<CWorkflowTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetColorPresets>(pParam);
        }
};

#endif // CGMICWIDGETCOLORPRESETS_HPP
