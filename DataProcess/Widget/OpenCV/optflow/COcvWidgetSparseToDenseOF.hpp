#ifndef COCVWIDGETSPARSETODENSEOF_HPP
#define COCVWIDGETSPARSETODENSEOF_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/optflow/COcvSparseToDenseOF.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSparseToDenseOF : public CProtocolTaskWidget
{
    public:

        COcvWidgetSparseToDenseOF(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSparseToDenseOF(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSparseToDenseOFParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSparseToDenseOFParam>();

            auto pCheck = addCheck(0, tr("Use OpenCL"), m_pParam->m_bUseOCL);
            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_bUseOCL = pCheck->isChecked();
                emit doApplyProcess(m_pParam);
            });

            
        }

    private:

        std::shared_ptr<COcvSparseToDenseOFParam> m_pParam = nullptr;
};

class COcvWidgetSparseToDenseOFFactory : public CWidgetFactory
{
    public:

        COcvWidgetSparseToDenseOFFactory()
        {
            m_name = QObject::tr("SparseToDense Flow").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSparseToDenseOF>(pParam);
        }
};
#endif // COCVWIDGETSPARSETODENSEOF_HPP
