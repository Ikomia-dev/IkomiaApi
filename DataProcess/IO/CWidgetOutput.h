#ifndef CWIDGETOUTPUT_H
#define CWIDGETOUTPUT_H

#include <QWidget>
#include "DataProcessGlobal.hpp"
#include "Protocol/CProtocolTaskIO.h"

/**
 * @ingroup groupCore
 * @brief The CWidgetOutput class defines a widget output for a workflow task.
 * @details This class is designed to handle widget as output of a workflow task.
 * A widget must be a derived object from QWidget od the Qt framework.
 * In Python,it can be either a QWidget based instance from PyQt5 or PySide2 bindings.
 */
class DATAPROCESSSHARED_EXPORT CWidgetOutput : public CProtocolTaskIO
{
    public:

        /**
        * @brief Default constructor
        */
        CWidgetOutput();
        /**
         * @brief Constructs a CWidgetOutput instance with the data type.
         * @details By default, the widget data type is ::Data::WIDGET.
         * @param data: data type.
         */
        CWidgetOutput(IODataType data);

        virtual ~CWidgetOutput();

        /**
         * @brief Sets the widget pointer. This class does not take ownership of the pointer.
         * @param pWidget: Qt QWidget based instance.
         * Python: PyQt5 or PySide2.
         */
        void        setWidget(QWidget* pWidget);

        /**
         * @brief Gets the current widget pointer.
         * @return QWidget based pointer.
         */
        QWidget*    getWidget() const;

        /**
         * @brief Check if the output contains a valid widget (not null).
         * @return True if the widget pointer is not null, False otherwise.
         */
        bool        isDataAvailable() const override;

        /**
         * @brief Clears the output data: sets the widget pointer to null.
         */
        void        clearData() override;

        void        save() override;
        void        save(const std::string& path) override;

        /**
         * @brief Makes a deep copy and returns it.
         * @return New instance of CWidgetOutput as shared pointer.
         */
        std::shared_ptr<CWidgetOutput> clone() const;

    private:

        virtual std::shared_ptr<CProtocolTaskIO> cloneImp() const override;

    protected:

        QWidget*    m_pWidget = nullptr;
};

class DATAPROCESSSHARED_EXPORT CWidgetOutputFactory: public CProtocolTaskIOFactory
{
    public:

        CWidgetOutputFactory()
        {
            m_name = "CWidgetOutput";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            return std::make_shared<CWidgetOutput>(dataType);
        }
};

#endif // CWIDGETOUTPUT_H
