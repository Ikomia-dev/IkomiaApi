#ifndef COCVWIDGETINRANGE_HPP
#define COCVWIDGETINRANGE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvInRange.hpp"

class COcvWidgetInRange : public CProtocolTaskWidget
{
    public:

        COcvWidgetInRange(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetInRange(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvInRangeParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvInRangeParam>();

            auto pCheckLowerScalar = addCheck(0, tr("Lower scalar input"), m_pParam->m_bLowerScalar);
            auto pSpinLowerScalar = addDoubleSpin(1, tr("Lower scalar value"), m_pParam->m_lower[0]);
            auto pCheckUpperScalar = addCheck(2, tr("Upper scalar input"), m_pParam->m_bUpperScalar);
            auto pSpinUpperScalar = addDoubleSpin(3, tr("Upper scalar value"), m_pParam->m_upper[0]);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bLowerScalar = pCheckLowerScalar->isChecked();
                m_pParam->m_lower= cv::Scalar::all(pSpinLowerScalar->value());
                m_pParam->m_bUpperScalar = pCheckUpperScalar->isChecked();
                m_pParam->m_upper= cv::Scalar::all(pSpinUpperScalar->value());
                emit doApplyProcess(m_pParam);
            } );
        }

    private:

        std::shared_ptr<COcvInRangeParam> m_pParam = nullptr;
};

class COcvWidgetInRangeFactory : public CWidgetFactory
{
    public:

        COcvWidgetInRangeFactory()
        {
            m_name = QObject::tr("InRange").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetInRange>(pParam);
        }
};

#endif // COCVWIDGETINRANGE_HPP
