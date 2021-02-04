#ifndef COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP
#define COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSelectiveSearchSegmentation.hpp"

class COcvWidgetSelectiveSearchSegmentation : public CProtocolTaskWidget
{
    public:

        COcvWidgetSelectiveSearchSegmentation(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
            initConnections();
        }
        COcvWidgetSelectiveSearchSegmentation(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSelectiveSearchSegmentationParam>(pParam);
            init();
            initConnections();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSelectiveSearchSegmentationParam>();

            auto pSpinSigma = addDoubleSpin(0, tr("Sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            auto pSpinBaseK = addDoubleSpin(1, tr("First K value"), m_pParam->m_baseK);
            auto pSpinStepK = addSpin(2, tr("Step for K value"), m_pParam->m_stepK);
            auto pSpinRectCount = addSpin(3, tr("Number of objects"), m_pParam->m_nbRects);

            auto pComboInit = addCombo(4, "Initialization method");
            pComboInit->addItem("Single", COcvSelectiveSearchSegmentationParam::SINGLE);
            pComboInit->addItem("Fast", COcvSelectiveSearchSegmentationParam::FAST);
            pComboInit->addItem("Quality", COcvSelectiveSearchSegmentationParam::QUALITY);
            pComboInit->setCurrentIndex(pComboInit->findData(m_pParam->m_initMethod));

            m_pCheckDefaultStrategy = new QCheckBox(tr("Default strategy"));
            m_pCheckDefaultStrategy->setChecked(m_pParam->m_bDefaultStrategy);

            m_pCheckColorStrategy = new QCheckBox(tr("Color strategy"));
            m_pCheckColorStrategy->setChecked(m_pParam->m_bColorStrategy);
            auto pSpinColorStrategy = new QDoubleSpinBox;
            pSpinColorStrategy->setDecimals(2);
            pSpinColorStrategy->setRange(0.0, 1.0);
            pSpinColorStrategy->setSingleStep(0.25);
            pSpinColorStrategy->setValue(m_pParam->m_colorWeight);

            m_pCheckFillStrategy = new QCheckBox(tr("Fill strategy"));
            m_pCheckFillStrategy->setChecked(m_pParam->m_bColorStrategy);
            auto pSpinFillStrategy = new QDoubleSpinBox;
            pSpinFillStrategy->setDecimals(2);
            pSpinFillStrategy->setRange(0.0, 1.0);
            pSpinFillStrategy->setSingleStep(0.25);
            pSpinFillStrategy->setValue(m_pParam->m_fillWeight);

            m_pCheckSizeStrategy = new QCheckBox(tr("Size strategy"));
            m_pCheckSizeStrategy->setChecked(m_pParam->m_bColorStrategy);
            auto pSpinSizeStrategy = new QDoubleSpinBox;
            pSpinSizeStrategy->setDecimals(2);
            pSpinSizeStrategy->setRange(0.0, 1.0);
            pSpinSizeStrategy->setSingleStep(0.25);
            pSpinSizeStrategy->setValue(m_pParam->m_sizeWeight);

            m_pCheckTextureStrategy = new QCheckBox(tr("Texture strategy"));
            m_pCheckTextureStrategy->setChecked(m_pParam->m_bColorStrategy);
            auto pSpinTextureStrategy = new QDoubleSpinBox;
            pSpinTextureStrategy->setDecimals(2);
            pSpinTextureStrategy->setRange(0.0, 1.0);
            pSpinTextureStrategy->setSingleStep(0.25);
            pSpinTextureStrategy->setValue(m_pParam->m_sizeWeight);

            auto pStrategyLayout = new QGridLayout;
            pStrategyLayout->addWidget(m_pCheckDefaultStrategy, 0, 0, 1, 2);
            pStrategyLayout->addWidget(m_pCheckColorStrategy, 1, 0);
            pStrategyLayout->addWidget(pSpinColorStrategy, 1, 1);
            pStrategyLayout->addWidget(m_pCheckFillStrategy, 2, 0);
            pStrategyLayout->addWidget(pSpinFillStrategy, 2, 1);
            pStrategyLayout->addWidget(m_pCheckSizeStrategy, 3, 0);
            pStrategyLayout->addWidget(pSpinSizeStrategy, 3, 1);
            pStrategyLayout->addWidget(m_pCheckTextureStrategy, 4, 0);
            pStrategyLayout->addWidget(pSpinTextureStrategy, 4, 1);

            auto pGroupBox = new QGroupBox(tr("Strategy: choose one or several of them"));
            pGroupBox->setLayout(pStrategyLayout);

            m_pLayout->addWidget(pGroupBox, 5, 0, 1, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigma = pSpinSigma->value();
                m_pParam->m_baseK = pSpinBaseK->value();
                m_pParam->m_stepK = pSpinStepK->value();
                m_pParam->m_nbRects = pSpinRectCount->value();
                m_pParam->m_initMethod = pComboInit->currentData().toInt();
                m_pParam->m_bDefaultStrategy = m_pCheckDefaultStrategy->isChecked();
                m_pParam->m_bColorStrategy = m_pCheckColorStrategy->isChecked();
                m_pParam->m_colorWeight = pSpinColorStrategy->value();
                m_pParam->m_bFillStrategy = m_pCheckFillStrategy->isChecked();
                m_pParam->m_fillWeight = pSpinFillStrategy->value();
                m_pParam->m_bSizeStrategy = m_pCheckSizeStrategy->isChecked();
                m_pParam->m_sizeWeight = pSpinSizeStrategy->value();
                m_pParam->m_bTextureStrategy = m_pCheckTextureStrategy->isChecked();
                m_pParam->m_textureWeight = pSpinTextureStrategy->value();
                emit doApplyProcess(m_pParam);
            });
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

    private:

        std::shared_ptr<COcvSelectiveSearchSegmentationParam> m_pParam = nullptr;
        QCheckBox* m_pCheckDefaultStrategy = nullptr;
        QCheckBox* m_pCheckColorStrategy = nullptr;
        QCheckBox* m_pCheckFillStrategy = nullptr;
        QCheckBox* m_pCheckSizeStrategy = nullptr;
        QCheckBox* m_pCheckTextureStrategy = nullptr;
};

class COcvWidgetSelectiveSearchSegmentationFactory : public CWidgetFactory
{
    public:

        COcvWidgetSelectiveSearchSegmentationFactory()
        {
            m_name = QObject::tr("Selective Search Segmentation").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSelectiveSearchSegmentation>(pParam);
        }
};

#endif // COCVWIDGETSELECTIVESEARCHSEGMENTATION_HPP
