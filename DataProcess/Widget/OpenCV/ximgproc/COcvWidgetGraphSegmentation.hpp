#ifndef COCVWIDGETGRAPHSEGMENTATION_HPP
#define COCVWIDGETGRAPHSEGMENTATION_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/OpenCV/ximgproc/COcvGraphSegmentation.hpp"

class COcvWidgetGraphSegmentation : public CProtocolTaskWidget
{
    public:

        COcvWidgetGraphSegmentation(QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            init();
        }
        COcvWidgetGraphSegmentation(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR) : CProtocolTaskWidget(parent)
        {
            m_pParam = std::dynamic_pointer_cast<COcvGraphSegmentationParam>(pParam);
            init();
        }

    protected:

        virtual void init()
        {
            if(m_pParam == nullptr)
                m_pParam = std::make_shared<COcvGraphSegmentationParam>();

            auto pSpinSigma = addDoubleSpin(0, tr("Sigma"), m_pParam->m_sigma, 0.0, DBL_MAX, 0.1, 2);
            auto pSpinK = addDoubleSpin(1, tr("K"), m_pParam->m_k);
            auto pSpinMinSize = addSpin(2, tr("Minimum size"), m_pParam->m_minSize);

            

            connect(m_pApplyBtn, &QPushButton::clicked, [=]
            {
                m_pParam->m_sigma = pSpinSigma->value();
                m_pParam->m_k = pSpinK->value();
                m_pParam->m_minSize = pSpinMinSize->value();
                emit doApplyProcess(m_pParam);
            });
        }

    private:

        std::shared_ptr<COcvGraphSegmentationParam> m_pParam = nullptr;
};

class COcvWidgetGraphSegmentationFactory : public CWidgetFactory
{
    public:

        COcvWidgetGraphSegmentationFactory()
        {
            m_name = QObject::tr("Graph Segmentation").toStdString();
        }

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<COcvWidgetGraphSegmentation>(pParam);
        }
};

#endif // COCVWIDGETGRAPHSEGMENTATION_HPP
