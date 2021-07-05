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

#ifndef CMEASUREIO_H
#define CMEASUREIO_H

#include "DataProcessGlobal.hpp"
#include "Data/CMeasure.h"
#include "Workflow/CWorkflowTaskIO.h"

class DATAPROCESSSHARED_EXPORT CObjectMeasure
{
    public:

        CObjectMeasure();
        CObjectMeasure(const CMeasure& measure, double value, size_t graphicsId, const std::string& label);
        CObjectMeasure(const CMeasure& measure, const std::vector<double>& values, size_t graphicsId, const std::string& label);
        CObjectMeasure(const CMeasure& measure, std::initializer_list<double> values, size_t graphicsId, const std::string& label);

        CMeasure            getMeasureInfo() const;
        std::vector<double> getValues() const;

        void                setValues(const std::vector<double>& values);

    public:

        CMeasure            m_measure;
        std::vector<double> m_values;
        size_t              m_graphicsId = 0;
        std::string         m_label = "";
};

using ObjectMeasures = std::vector<CObjectMeasure>;
using ObjectsMeasures = std::vector<ObjectMeasures>;

class DATAPROCESSSHARED_EXPORT CMeasureIO : public CWorkflowTaskIO
{
    public:

        CMeasureIO();
        CMeasureIO(const CMeasureIO& io);
        CMeasureIO(const CMeasureIO&& io);

        virtual ~CMeasureIO();

        CMeasureIO&      operator=(const CMeasureIO& io);
        CMeasureIO&      operator=(const CMeasureIO&& io);

        void                    setObjectMeasure(size_t index, const CObjectMeasure& measure);

        ObjectsMeasures         getMeasures() const;

        bool                    isDataAvailable() const override;

        void                    addObjectMeasure(const CObjectMeasure& measure);
        void                    addObjectMeasures(const std::vector<CObjectMeasure>& measures);

        void                    clearData() override;

        void                    save() override;
        void                    save(const std::string &path) override;

        std::shared_ptr<CMeasureIO> clone() const;

    private:

        std::set<std::string>   getMeasuresNames() const;

        void                    saveCSV(const std::string &path) const;
        void                    saveXML(const std::string &path) const;

        virtual std::shared_ptr<CWorkflowTaskIO> cloneImp() const override;

    private:

        //List of measures for each blob
        ObjectsMeasures m_measures;
};

class DATAPROCESSSHARED_EXPORT CMeasureIOFactory: public CWorkflowTaskIOFactory
{
    public:

        CMeasureIOFactory()
        {
            m_name = "CMeasureIO";
        }

        virtual WorkflowTaskIOPtr   create(IODataType dataType)
        {
            Q_UNUSED(dataType);
            return std::make_shared<CMeasureIO>();
        }
};

#endif // CMEASUREIO_H
