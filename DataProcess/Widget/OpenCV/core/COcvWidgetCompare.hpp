#ifndef COCVWIDGETCOMPARE_HPP
#define COCVWIDGETCOMPARE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvCompare.hpp"

class COcvWidgetCompare : public CProtocolTaskWidget
{
    public:

        COcvWidgetCompare(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetCompare(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvCompareParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvCompareParam>();

            auto pComboOp = addCombo(1, tr("Operation"));
            pComboOp->addItem(tr("Equal"), cv::CMP_EQ);
            pComboOp->addItem(tr("Greater"), cv::CMP_GT);
            pComboOp->addItem(tr("Greater or equal"), cv::CMP_GE);
            pComboOp->addItem(tr("Less"), cv::CMP_LT);
            pComboOp->addItem(tr("Less or equal"), cv::CMP_LE);
            pComboOp->addItem(tr("Unequal"), cv::CMP_NE);
            pComboOp->setCurrentIndex(pComboOp->findData(m_pParam->m_operation));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_operation = pComboOp->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvCompareParam> m_pParam = nullptr;
};

class COcvWidgetCompareFactory : public CWidgetFactory
{
    public:

        COcvWidgetCompareFactory()
        {
            m_name = QObject::tr("Compare").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetCompare>(pParam);
        }
};

#endif // COCVWIDGETCOMPARE_HPP
