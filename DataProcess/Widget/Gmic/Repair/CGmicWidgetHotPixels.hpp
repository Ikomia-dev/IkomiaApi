#ifndef CGMICWIDGETHOTPIXELS_HPP
#define CGMICWIDGETHOTPIXELS_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Repair/CGmicHotPixels.hpp"

class CGmicWidgetHotPixels : public CProtocolTaskWidget
{
    public:

        CGmicWidgetHotPixels(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetHotPixels(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicHotPixelsParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicHotPixelsParam>();

            auto pSpinMaskSize = addSpin(0, tr("Mask size"), m_pParam->m_maskSize);
            auto pSpinThreshold = addSpin(1, tr("Threshold(%)"), m_pParam->m_threshold);

            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_maskSize = pSpinMaskSize->value();
                m_pParam->m_threshold = pSpinThreshold->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicHotPixelsParam> m_pParam = nullptr;
};

class CGmicWidgetHotPixelsFactory : public CWidgetFactory
{
    public:

        CGmicWidgetHotPixelsFactory()
        {
            m_name = QObject::tr("Hot pixels").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetHotPixels>(pParam);
        }
};

#endif // CGMICWIDGETHOTPIXELS_HPP
