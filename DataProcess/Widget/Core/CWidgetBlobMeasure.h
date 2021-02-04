#ifndef CWIDGETBLOBMEASURE_HPP
#define CWIDGETBLOBMEASURE_HPP

#include "Core/CWidgetFactory.hpp"
#include "Process/Core/CBlobMeasure.h"
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>

class CWidgetBlobMeasure : public CProtocolTaskWidget
{
    public:

        CWidgetBlobMeasure(QWidget *parent = Q_NULLPTR);
        CWidgetBlobMeasure(std::shared_ptr<CProtocolTaskParam> pParam, QWidget *parent = Q_NULLPTR);

    public slots:

        void            onItemChanged(QListWidgetItem* pItem);

    protected:

        virtual void    init();
        void            initConnections();

        void            fillMeasuresListView();

        int             getMeasureIndex(const std::string &name) const;

    private:

        std::shared_ptr<CBlobMeasureParam>  m_pParam = nullptr;
        QListWidget*                        m_pListView = nullptr;
        std::vector<CMeasure>               m_measures;
};

class CWidgetBlobMeasureFactory : public CWidgetFactory
{
    public:

        CWidgetBlobMeasureFactory()
        {
            m_name = QObject::tr("Objects measurements").toStdString();
        }
        ~CWidgetBlobMeasureFactory() {}

        virtual ProtocolTaskWidgetPtr   create(std::shared_ptr<CProtocolTaskParam> pParam)
        {
            return std::make_shared<CWidgetBlobMeasure>(pParam);
        }
};

#endif // CWIDGETBLOBMEASURE_HPP
