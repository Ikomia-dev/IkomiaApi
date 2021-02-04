#ifndef CGMICWIDGETCOLORPRESETS_HPP
#define CGMICWIDGETCOLORPRESETS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Colors/CGmicColorPresets.hpp"

class CGmicWidgetColorPresets : public CProtocolTaskWidget
{
    public:

        CGmicWidgetColorPresets(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetColorPresets(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicColorPresetsParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicColorPresetsParam>();

            auto pComboCategory = addCombo(0, tr("Category"));
            for(size_t i=0; i<m_pParam->m_categories.size(); ++i)
                pComboCategory->addItem(QString::fromStdString(m_pParam->m_categories[i]));

            pComboCategory->setCurrentIndex(m_pParam->m_category);

            auto pComboPreset = addCombo(1, tr("Presets"));
            for(size_t i=0; i<m_pParam->m_presets[0].size(); ++i)
                pComboPreset->addItem(QString::fromStdString(m_pParam->m_presets[0][i]));

            pComboPreset->setCurrentIndex(m_pParam->m_preset);

            auto pSpinThumbSize = addSpin(2, tr("Thumbnail size"), m_pParam->m_thumbSize, 0, 1024);
            auto pSpinStrength = addDoubleSpin(3, tr("Strength(%)"), m_pParam->m_strenght, 0, 100, 1.0);
            auto pSpinBrightness = addDoubleSpin(4, tr("Brightness(%)"), m_pParam->m_brightness, -100, 100, 1.0);
            auto pSpinContrast = addDoubleSpin(5, tr("Contrast(%)"), m_pParam->m_contrast, -100, 100, 1.0);
            auto pSpinGamma = addDoubleSpin(6, tr("Gamma(%)"), m_pParam->m_gamma, -100, 100, 1.0);
            auto pSpinHue = addDoubleSpin(7, tr("Hue(%)"), m_pParam->m_hue, -100, 100, 1.0);
            auto pSpinSaturation = addDoubleSpin(8, tr("Saturation(%)"), m_pParam->m_saturation, -100, 100, 1.0);

            auto pComboNormColors = addCombo(9, tr("Normalize colors"));
            pComboNormColors->addItem(tr("None"), CGmicColorPresetsParam::NONE);
            pComboNormColors->addItem(tr("Pre-normalyze"), CGmicColorPresetsParam::PRE_NORMALIZE);
            pComboNormColors->addItem(tr("Post-normalyze"), CGmicColorPresetsParam::POST_NORMALIZE);
            pComboNormColors->addItem(tr("Both"), CGmicColorPresetsParam::BOTH);

            connect(pComboCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index)
            {
                 pComboPreset->clear();

                 for(size_t i=0; i<m_pParam->m_presets[index].size(); ++i)
                     pComboPreset->addItem(QString::fromStdString(m_pParam->m_presets[index][i]));

                 pComboPreset->setCurrentIndex(1);
            });

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_category = pComboCategory->currentIndex();
                m_pParam->m_preset = pComboPreset->currentIndex();
                m_pParam->m_thumbSize = pSpinThumbSize->value();
                m_pParam->m_strenght = pSpinStrength->value();
                m_pParam->m_brightness = pSpinBrightness->value();
                m_pParam->m_contrast = pSpinContrast->value();
                m_pParam->m_gamma = pSpinGamma->value();
                m_pParam->m_hue = pSpinHue->value();
                m_pParam->m_saturation = pSpinSaturation->value();
                m_pParam->m_normalizeColors = pComboNormColors->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicColorPresetsParam> m_pParam = nullptr;
};

class CGmicWidgetColorPresetsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetColorPresetsFactory()
        {
            m_name = QObject::tr("Color presets").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetColorPresets>(pParam);
        }
};

#endif // CGMICWIDGETCOLORPRESETS_HPP
