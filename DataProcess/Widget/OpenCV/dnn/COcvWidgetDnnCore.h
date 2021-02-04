#ifndef COCVWIDGETDNNCORE_H
#define COCVWIDGETDNNCORE_H

#include "Protocol/CProtocolTaskWidget.h"
#include "Process/OpenCV/dnn/COcvDnnProcess.h"

class DATAPROCESSSHARED_EXPORT COcvWidgetDnnCore : public CProtocolTaskWidget
{
    public:

        COcvWidgetDnnCore(QWidget *parent = Q_NULLPTR);
        COcvWidgetDnnCore(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        virtual void    init() override;

    private:

        void            initConnections();
        void            fillComboTarget(cv::dnn::Backend backend);
        QString         getTargetName(const cv::dnn::Target& target) const;

    protected:

        QComboBox*                          m_pComboBackend = nullptr;
        QComboBox*                          m_pComboTarget = nullptr;
        std::shared_ptr<COcvDnnCoreParam>   m_pParam = nullptr;
};

#endif // COCVWIDGETDNNCORE_H
