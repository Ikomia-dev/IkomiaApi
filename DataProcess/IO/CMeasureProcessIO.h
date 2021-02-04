#ifndef CMEASUREPROCESSIO_H
#define CMEASUREPROCESSIO_H

#include "DataProcessGlobal.hpp"
#include "Data/CMeasure.h"
#include "Protocol/CProtocolTaskIO.h"

class DATAPROCESSSHARED_EXPORT CObjectMeasure
{
    public:

        CObjectMeasure();
        CObjectMeasure(CMeasure measure, const double& value, size_t graphicsId, const std::string& label);
        CObjectMeasure(CMeasure measure, std::initializer_list<double> values, size_t graphicsId, const std::string& label);

    public:

        CMeasure            m_measure;
        std::vector<double> m_values;
        size_t              m_graphicsId = 0;
        std::string         m_label = "";
};

using ObjectMeasures = std::vector<CObjectMeasure>;
using ObjectsMeasures = std::vector<ObjectMeasures>;

class DATAPROCESSSHARED_EXPORT CMeasureProcessIO : public CProtocolTaskIO
{
    public:

        CMeasureProcessIO();
        CMeasureProcessIO(const CMeasureProcessIO& io);
        CMeasureProcessIO(const CMeasureProcessIO&& io);

        virtual ~CMeasureProcessIO();

        CMeasureProcessIO&      operator=(const CMeasureProcessIO& io);
        CMeasureProcessIO&      operator=(const CMeasureProcessIO&& io);

        void                    setObjectMeasure(size_t index, const CObjectMeasure& measure);

        ObjectsMeasures         getMeasures() const;

        bool                    isDataAvailable() const override;

        void                    addObjectMeasure(const CObjectMeasure& measure);
        void                    addObjectMeasures(const std::vector<CObjectMeasure>& measures);

        void                    clearData() override;

        void                    save() override;
        void                    save(const std::string &path) override;

        std::shared_ptr<CMeasureProcessIO> clone() const;

    private:

        std::set<std::string>   getMeasuresNames() const;

        void                    saveCSV(const std::string &path) const;
        void                    saveXML(const std::string &path) const;

        virtual std::shared_ptr<CProtocolTaskIO> cloneImp() const override;

    private:

        //List of measures for each blob
        ObjectsMeasures m_measures;
};

class DATAPROCESSSHARED_EXPORT CMeasureProcessIOFactory: public CProtocolTaskIOFactory
{
    public:

        CMeasureProcessIOFactory()
        {
            m_name = "MeasureProcessIO";
        }

        virtual ProtocolTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CMeasureProcessIO>();
        }
};

#endif // CMEASUREPROCESSIO_H
