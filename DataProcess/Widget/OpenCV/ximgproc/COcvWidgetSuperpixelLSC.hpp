#ifndef COCVWIDGETSUPERPIXELLSC_HPP
#define COCVWIDGETSUPERPIXELLSC_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvSuperpixelLSC.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSuperpixelLSC : public CProtocolTaskWidget
{
    public:

        COcvWidgetSuperpixelLSC(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSuperpixelLSC(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSuperpixelLSCParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSuperpixelLSCParam>();

            auto pSpinSize = addSpin(0, tr("Region size"), m_pParam->m_regions_size);
            auto pSpinRatio = addDoubleSpin(1, tr("Ratio"), m_pParam->m_ratio, 0, 1, 0.001, 3);
            auto pSpinIter = addSpin(2, tr("Iterations"), m_pParam->m_num_iterations);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_regions_size = pSpinSize->value();
                m_pParam->m_ratio = pSpinRatio->value();
                m_pParam->m_num_iterations = pSpinIter->value();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSuperpixelLSCParam> m_pParam = nullptr;
};

class COcvWidgetSuperpixelLSCFactory : public CWidgetFactory
{
    public:

        COcvWidgetSuperpixelLSCFactory()
        {
            m_name = QObject::tr("SuperpixelLSC").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSuperpixelLSC>(pParam);
        }
};
#endif // COCVWIDGETSUPERPIXELLSC_HPP
