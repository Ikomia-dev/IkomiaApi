#ifndef COCVWIDGETMIN_HPP
#define COCVWIDGETMIN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvMin.hpp"

class COcvWidgetMin : public CProtocolTaskWidget
{
    public:

        COcvWidgetMin(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMin(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMinParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMinParam>();

            auto pCheckScalar = addCheck(0, tr("Scalar operation"), m_pParam->m_bScalar);
            auto pSpinScalar = addDoubleSpin(1, tr("Scalar value"), m_pParam->m_scalar[0]);
            
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bScalar = pCheckScalar->isChecked();
                m_pParam->m_scalar= cv::Scalar::all(pSpinScalar->value());
                emit doApplyProcess(m_pParam);
            } );
        }

    private:

        std::shared_ptr<COcvMinParam> m_pParam = nullptr;
};

class COcvWidgetMinFactory : public CWidgetFactory
{
    public:

        COcvWidgetMinFactory()
        {
            m_name = QObject::tr("Min").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMin>(pParam);
        }
};

#endif // COCVWIDGETMIN_HPP
