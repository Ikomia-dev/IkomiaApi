#ifndef COCVWIDGETDENOISETVL1_HPP
#define COCVWIDGETDENOISETVL1_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/photo/COcvDenoiseTVL1.hpp"

class COcvWidgetDenoiseTVL1 : public CProtocolTaskWidget
{
    public:

        COcvWidgetDenoiseTVL1(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetDenoiseTVL1(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvDenoiseTVL1Param>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvDenoiseTVL1Param>();

            auto pSpinLambda = addDoubleSpin(0, tr("Lambda"), m_pParam->m_lambda);
            auto pSpinIter = addSpin(1, tr("Iterations"), m_pParam->m_niters);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_lambda = pSpinLambda->value();
                m_pParam->m_niters = pSpinIter->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvDenoiseTVL1Param> m_pParam = nullptr;
};

class COcvWidgetDenoiseTVL1Factory : public CWidgetFactory
{
    public:

        COcvWidgetDenoiseTVL1Factory()
        {
            m_name = QObject::tr("Denoise TV L1").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetDenoiseTVL1>(pParam);
        }
};
#endif // COCVWIDGETDENOISETVL1_HPP
