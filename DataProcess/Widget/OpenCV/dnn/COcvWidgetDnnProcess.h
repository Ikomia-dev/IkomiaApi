#ifndef COCVWIDGETDNNPROCESS_H
#define COCVWIDGETDNNPROCESS_H

#include "Process/OpenCV/dnn/COcvDnnProcess.h"
#include "Widget/OpenCV/dnn/COcvWidgetDnnCore.h"

class DATAPROCESSSHARED_EXPORT COcvWidgetDnnProcess : public COcvWidgetDnnCore
{    
    public:

        COcvWidgetDnnProcess(QWidget *parent = Q_NULLPTR);
        COcvWidgetDnnProcess(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    protected:

        virtual void    init() override;

        void            clearFiles();

    private slots:

        void            onFrameworkChanged(int index);
        void            onSelectModelFile();
        void            onSelectStructFile();
        void            onSelectLabelsFile();
        void            onApply();

    private:

        void            initConnections();
        void            initFramework(int framework);

    protected:

        QVBoxLayout*                            m_pVBoxLayout = nullptr;
        QComboBox*                              m_pComboFramework = nullptr;
        CBrowseFileWidget*                      m_pBrowseStructFile = nullptr;
        CBrowseFileWidget*                      m_pBrowseModelFile = nullptr;
        CBrowseFileWidget*                      m_pBrowseLabelFile = nullptr;
};

#endif // COCVWIDGETDNNPROCESS_H
