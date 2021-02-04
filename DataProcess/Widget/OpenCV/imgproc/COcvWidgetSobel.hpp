#ifndef COCVWIDGETSOBEL_HPP
#define COCVWIDGETSOBEL_HPP
#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/imgproc/COcvSobel.hpp"
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>

class COcvWidgetSobel : public CProtocolTaskWidget
{
    public:

        COcvWidgetSobel(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }

        COcvWidgetSobel(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvSobelParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvSobelParam>();

            /*auto pSpinDepth = addSpin(0, tr("Depth"), m_pParam->m_ddepth);
            pSpinDepth->setMinimum(-1);*/
            auto pSpinX = addSpin(0, tr("x-derivative order"), m_pParam->m_dx);
            pSpinX->setRange(0,3);

            auto pSpinY = addSpin(1, tr("y-derivative order"), m_pParam->m_dy);
            pSpinY->setRange(0,3);

            auto pSpinSize = addSpin(2, tr("Kernel size"), m_pParam->m_ksize);
            pSpinSize->setMinimum(1);

            auto pDblSpinScale = addDoubleSpin(3, tr("Scale"), m_pParam->m_scale);
            auto pDblSpinDelta = addDoubleSpin(4, tr("Delta"), m_pParam->m_delta);

            auto pComboBorder = addCombo(5, "Border type");
            pComboBorder->addItem("Default", cv::BORDER_DEFAULT);
            pComboBorder->addItem("Replicate", cv::BORDER_REPLICATE);
            pComboBorder->setCurrentIndex(pComboBorder->findData(m_pParam->m_border));

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_dx = pSpinX->value();
                m_pParam->m_dy = pSpinY->value();
                m_pParam->m_ksize = pSpinSize->value();
                m_pParam->m_scale = pDblSpinScale->value();
                m_pParam->m_delta = pDblSpinDelta->value();
                m_pParam->m_border = pComboBorder->currentData().toInt();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvSobelParam> m_pParam = nullptr;
};

class COcvWidgetSobelFactory : public CWidgetFactory
{
    public:

        COcvWidgetSobelFactory()
        {
            m_name = QObject::tr("Sobel").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetSobel>(pParam);
        }
};
#endif // COCVWIDGETSOBEL_HPP
