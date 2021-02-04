#ifndef COCVWIDGETMULTIPLY_HPP
#define COCVWIDGETMULTIPLY_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMultiply.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetMultiply : public CProtocolTaskWidget
{
    public:

        COcvWidgetMultiply(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMultiply(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMultiplyParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMultiplyParam>();

            auto pSpinScale = addDoubleSpin(0, tr("Scale"), m_pParam->m_scale, 0, DBL_MAX, 1, 4);
            auto pComboDepth = addCombo(1, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_scale = pSpinScale->value();
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvMultiplyParam> m_pParam = nullptr;
};

class COcvWidgetMultiplyFactory : public CWidgetFactory
{
    public:

        COcvWidgetMultiplyFactory()
        {
            m_name = QObject::tr("Multiply").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMultiply>(pParam);
        }
};
#endif // COCVWIDGETMULTIPLY_HPP
