#ifndef COCVWIDGETADD_HPP
#define COCVWIDGETADD_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/core/COcvAdd.hpp"
#include <QDoubleSpinBox>
#include <QLabel>

class COcvWidgetAdd : public CProtocolTaskWidget
{
    public:

        COcvWidgetAdd(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetAdd(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvAddParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvAddParam>();

            auto pComboDepth = addCombo(1, tr("Depth"));
            pComboDepth->addItem("Default", -1);
            pComboDepth->addItem("CV_8U", CV_8U);
            pComboDepth->addItem("CV_16U", CV_16U);
            pComboDepth->addItem("CV_32F", CV_32F);
            pComboDepth->addItem("CV_64F", CV_64F);
            pComboDepth->setCurrentIndex(pComboDepth->findData(m_pParam->m_dtype));

            connect(m_pApplyBtn, &QPushButton::clicked, [=]{
                m_pParam->m_dtype = pComboDepth->currentData().toInt();
                emit doApplyProcess(m_pParam);
            } );

            
            
        }

    private:

        std::shared_ptr<COcvAddParam> m_pParam = nullptr;
};

class COcvWidgetAddFactory : public CWidgetFactory
{
    public:

        COcvWidgetAddFactory()
        {
            m_name = QObject::tr("Add").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetAdd>(pParam);
        }
};

#endif // COCVWIDGETADD_HPP
