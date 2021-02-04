#ifndef COCVWIDGETMEDIAN_HPP
#define COCVWIDGETMEDIAN_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvMedian.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>

class COcvWidgetMedian : public CProtocolTaskWidget
{
    public:

        COcvWidgetMedian(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetMedian(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvMedianParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvMedianParam>();

            auto pSpinSize = addSpin(0, tr("Filter size"), m_pParam->m_ksize, 1, INT_MAX, 2);
            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_ksize = pSpinSize->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvMedianParam> m_pParam = nullptr;
};

class COcvWidgetMedianFactory : public CWidgetFactory
{
    public:

        COcvWidgetMedianFactory()
        {
            m_name = QObject::tr("Median Filter").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetMedian>(pParam);
        }
};

#endif // COCVWIDGETMEDIAN_HPP
