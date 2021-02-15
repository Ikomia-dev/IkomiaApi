/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
