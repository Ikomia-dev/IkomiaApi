#ifndef COCVWIDGETCOLORCHANGE_HPP
#define COCVWIDGETCOLORCHANGE_HPP
#include "Process/OpenCV/photo/COcvColorchange.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetColorchange : public CProtocolTaskWidget
{
    public:

        COcvWidgetColorchange(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetColorchange(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvColorchangeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvColorchangeParam>();

            auto pSpinRed = addDoubleSpin(0, tr("Red factor"), m_pParam->m_red_mul, 0.5, 2.5, 0.1);
            auto pSpinGreen = addDoubleSpin(1, tr("Green factor"), m_pParam->m_green_mul, 0.5, 2.5, 0.1);
            auto pSpinBlue = addDoubleSpin(2, tr("Blue factor"), m_pParam->m_blue_mul, 0.5, 2.5, 0.1);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_red_mul = pSpinRed->value();
                m_pParam->m_green_mul = pSpinGreen->value();
                m_pParam->m_blue_mul = pSpinBlue->value();
                emit doApplyProcess(m_pParam); } );

            
        }

    private:

        std::shared_ptr<COcvColorchangeParam> m_pParam = nullptr;
};

class COcvWidgetColorchangeFactory : public CWidgetFactory
{
    public:

        COcvWidgetColorchangeFactory()
        {
            m_name = QObject::tr("Colorchange").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetColorchange>(pParam);
        }
};
#endif // COCVWIDGETCOLORCHANGE_HPP
