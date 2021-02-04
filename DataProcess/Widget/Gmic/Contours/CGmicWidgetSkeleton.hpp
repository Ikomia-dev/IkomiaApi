#ifndef CGMICWIDGETSKELETON_HPP
#define CGMICWIDGETSKELETON_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Gmic/Contours/CGmicSkeleton.hpp"

class CGmicWidgetSkeleton : public CProtocolTaskWidget
{
    public:

        CGmicWidgetSkeleton(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        CGmicWidgetSkeleton(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CGmicSkeletonParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CGmicSkeletonParam>();

            auto pComboMethod = addCombo(0, tr("Method"));
            pComboMethod->addItem(tr("Distance(Fast)"), CGmicSkeletonParam::DISTANCE);
            pComboMethod->addItem(tr("Thinning(Slow)"), CGmicSkeletonParam::THINNING);
            pComboMethod->setCurrentIndex(pComboMethod->findData(m_pParam->m_method));

            auto pSpinSmooth = addDoubleSpin(1, tr("Smoothness"), m_pParam->m_smoothness, 0, 10, 0.5);

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_method = pComboMethod->currentData().toInt();
                m_pParam->m_smoothness = pSpinSmooth->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<CGmicSkeletonParam> m_pParam = nullptr;
};

class CGmicWidgetSkeletonFactory : public CWidgetFactory
{
    public:

        CGmicWidgetSkeletonFactory()
        {
            m_name = QObject::tr("Skeleton").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CGmicWidgetSkeleton>(pParam);
        }
};

#endif // CGMICWIDGETSKELETON_HPP
