#ifndef COCVWIDGETGFTT_HPP
#define COCVWIDGETGFTT_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/features2d/COcvGFTT.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetGFTT : public CProtocolTaskWidget
{
    public:

        COcvWidgetGFTT(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetGFTT(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGFTTParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGFTTParam>();

            auto pSpinCorners = addSpin(0, tr("Max corners"), m_pParam->m_maxCorners);
            pSpinCorners->setRange(0, INT_MAX);
            pSpinCorners->setValue(m_pParam->m_maxCorners);
            auto pSpinQuality = addDoubleSpin(1, tr("Quality level"), m_pParam->m_qualityLevel);
            pSpinQuality->setSingleStep(0.01);
            auto pSpinDist = addDoubleSpin(2, tr("Min distance"), m_pParam->m_minDistance);
            auto pSpinBlockSize = addSpin(3, tr("Block size"), m_pParam->m_blockSize);
            auto pCheckHarris = addCheck(4, tr("Use Harris"), m_pParam->m_bUseHarrisDetector);
            auto pSpinCoeff = addDoubleSpin(5, tr("Harris coefficient"), m_pParam->m_k);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_maxCorners = pSpinCorners->value();
                m_pParam->m_qualityLevel = pSpinQuality->value();
                m_pParam->m_minDistance = pSpinDist->value();
                m_pParam->m_blockSize = pSpinBlockSize->value();
                m_pParam->m_bUseHarrisDetector = pCheckHarris->isChecked();
                m_pParam->m_k = pSpinCoeff->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvGFTTParam> m_pParam = nullptr;
};

class COcvWidgetGFTTFactory : public CWidgetFactory
{
    public:

        COcvWidgetGFTTFactory()
        {
            m_name = QObject::tr("GFTTDetector").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGFTT>(pParam);
        }
};
#endif // COCVWIDGETGFTT_HPP
