#ifndef CWIDGETPLOTPROCESS_H
#define CWIDGETPLOTPROCESS_H

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CPlotMerge.hpp"
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>

class CWidgetPlotMerge : public CProtocolTaskWidget
{
    public:

        CWidgetPlotMerge(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        CWidgetPlotMerge(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<CPlotMergeParam>(pParam);
            init();
        }

    protected:

        virtual void    init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<CPlotMergeParam>();

            auto pSpin = new QSpinBox;
            auto pLabel = new QLabel(tr("Number of inputs"));

            pSpin->setValue(m_pParam->m_inputCount);

            m_pLayout->addWidget(pLabel, 0, 0);
            m_pLayout->addWidget(pSpin, 0, 1);
            

             connect(m_pApplyBtn, &QPushButton::clicked, [this, pSpin]{
                 m_pParam->m_inputCount = pSpin->value();
                 emit doApplyProcess(m_pParam); } );
        }

    private:

        std::shared_ptr<CPlotMergeParam>  m_pParam = nullptr;
};

class CWidgetPlotMergeFactory : public CWidgetFactory
{
    public:

        CWidgetPlotMergeFactory()
        {
            m_name = QObject::tr("Plots merge").toStdString();
        }
        ~CWidgetPlotMergeFactory() {}

        virtual ProtocolTaskWidgetPtr create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_unique<CWidgetPlotMerge>(pParam);
        }
};

#endif // CWIDGETPLOTPROCESS_H
