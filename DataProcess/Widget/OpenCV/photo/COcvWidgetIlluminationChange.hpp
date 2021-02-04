#ifndef COCVWIDGETILLUMINATIONCHANGE_HPP
#define COCVWIDGETILLUMINATIONCHANGE_HPP
#include "Process/OpenCV/photo/COcvIlluminationChange.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetIlluminationChange : public CProtocolTaskWidget
{
    public:

        COcvWidgetIlluminationChange(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetIlluminationChange(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvIlluminationChangeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvIlluminationChangeParam>();

            auto pSpinAlpha = addDoubleSpin(0, tr("Alpha"), m_pParam->m_alpha, 0, 2, 0.1);
            auto pSpinBeta = addDoubleSpin(1, tr("Beta"), m_pParam->m_beta, 0, 2, 0.1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_alpha = pSpinAlpha->value();
                m_pParam->m_beta = pSpinBeta->value();
                emit doApplyProcess(m_pParam); } );

            
            
        }

    private:

        std::shared_ptr<COcvIlluminationChangeParam> m_pParam = nullptr;
};

class COcvWidgetIlluminationChangeFactory : public CWidgetFactory
{
    public:

        COcvWidgetIlluminationChangeFactory()
        {
            m_name = QObject::tr("IlluminationChange").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetIlluminationChange>(pParam);
        }
};
#endif // COCVWIDGETILLUMINATIONCHANGE_HPP
