#ifndef COCVWIDGETOBJECTNESSBING_HPP
#define COCVWIDGETOBJECTNESSBING_HPP
#include "Process/OpenCV/saliency/COcvObjectnessBING.hpp"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>
#include <QGridLayout>

class COcvWidgetObjectnessBING : public CProtocolTaskWidget
{
    public:

        COcvWidgetObjectnessBING(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetObjectnessBING(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvObjectnessBINGParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvObjectnessBINGParam>();

            auto pSpinBB = addSpin(0, tr("N best bounding boxes"), m_pParam->m_nbBBox);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_nbBBox = pSpinBB->value();
                emit doApplyProcess(m_pParam); } );

            
        }

    private:

        std::shared_ptr<COcvObjectnessBINGParam> m_pParam = nullptr;
};

class COcvWidgetObjectnessBINGFactory : public CWidgetFactory
{
    public:

        COcvWidgetObjectnessBINGFactory()
        {
            m_name = QObject::tr("ObjectnessBING").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetObjectnessBING>(pParam);
        }
};
#endif // COCVWIDGETOBJECTNESSBING_HPP
