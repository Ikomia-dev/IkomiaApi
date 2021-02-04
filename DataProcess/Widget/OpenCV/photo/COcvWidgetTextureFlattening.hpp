#ifndef COCVWIDGETTEXTUREFLATTENING_HPP
#define COCVWIDGETTEXTUREFLATTENING_HPP
#include "../../../Process/OpenCV/photo/COcvTextureFlattening.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetTextureFlattening : public CProtocolTaskWidget
{
    public:

        COcvWidgetTextureFlattening(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetTextureFlattening(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvTextureFlatteningParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvTextureFlatteningParam>();

            auto pLabelLow = new QLabel(tr("Low threshold"));
            auto pLabelHigh = new QLabel(tr("High threshold"));
            auto pLabelKer = new QLabel(tr("Kernel size"));

            auto pSpinLow = new QDoubleSpinBox;
            auto pSpinHigh = new QDoubleSpinBox;
            auto pSpinKer = new QDoubleSpinBox;

            pSpinLow->setValue(m_pParam->m_lowThresh);
            pSpinLow->setRange(0, 100);
            pSpinLow->setSingleStep(1);

            pSpinHigh->setValue(m_pParam->m_highThresh);
            pSpinHigh->setRange(0, 100);
            pSpinHigh->setSingleStep(1);

            pSpinKer->setValue(m_pParam->m_kernelSize);
            pSpinKer->setRange(0, 100);
            pSpinKer->setSingleStep(1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_lowThresh = pSpinLow->value();
                m_pParam->m_highThresh = pSpinHigh->value();
                m_pParam->m_kernelSize = pSpinKer->value();
                emit doApplyProcess(m_pParam); } );

            
            m_pLayout->addWidget(pLabelLow, 0, 0);
            m_pLayout->addWidget(pSpinLow, 0, 1);

            m_pLayout->addWidget(pLabelHigh, 1, 0);
            m_pLayout->addWidget(pSpinHigh, 1, 1);

            m_pLayout->addWidget(pLabelKer, 2, 0);
            m_pLayout->addWidget(pSpinKer, 2, 1);

            
        }

    private:

        std::shared_ptr<COcvTextureFlatteningParam> m_pParam = nullptr;
};

class COcvWidgetTextureFlatteningFactory : public CWidgetFactory
{
    public:

        COcvWidgetTextureFlatteningFactory()
        {
            m_name = QObject::tr("Texture flattening").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetTextureFlattening>(pParam);
        }
};

#endif // COCVWIDGETTEXTUREFLATTENING_HPP
