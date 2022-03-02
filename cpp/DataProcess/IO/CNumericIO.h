// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef CNUMERICIO_H
#define CNUMERICIO_H

#include "DataProcessGlobal.hpp"
#include "Workflow/CWorkflowTaskIO.h"
#include "Main/CoreTools.hpp"

/** @file CNumericIO.hpp */

/**
 * @enum  NumericOutputType
 * @brief Output display type for numeric values
 */
enum class NumericOutputType
{
    NONE = 0,           /**< No display */
    TABLE = 2,          /**< Table display */
    PLOT = 4,           /**< Plot display */
    NUMERIC_FILE = 8    /**< Save to file */
};

/**
 * @enum  PlotType
 * @brief Plot type to display numeric values
 */
enum class PlotType
{
    CURVE,      /**< Curve */
    BAR,        /**< Bar diagram */
    MULTIBAR,   /**< Multi-bar diagram */
    HISTOGRAM,  /**< Histogram */
    PIE         /**< Pie chart */
};

class CNumericIOBase: public CWorkflowTaskIO
{
    public:

        using StringVector = std::vector<std::string>;
        using VectorOfStringVector = std::vector<std::vector<std::string>>;

        CNumericIOBase(): CWorkflowTaskIO(IODataType::NUMERIC_VALUES, "CNumericIO")
        {
        }
        CNumericIOBase(const std::string& name): CWorkflowTaskIO(IODataType::NUMERIC_VALUES, name)
        {
        }
        CNumericIOBase(const CNumericIOBase& io) : CWorkflowTaskIO(io)
        {
            m_outputType = io.m_outputType;
            m_plotType = io.m_plotType;
        }
        CNumericIOBase(const CNumericIOBase&& io) : CWorkflowTaskIO(io)
        {
            m_outputType = std::move(io.m_outputType);
            m_plotType = std::move(io.m_plotType);
        }

        virtual ~CNumericIOBase(){}

        virtual CNumericIOBase&  operator=(const CNumericIOBase& io)
        {
            CWorkflowTaskIO::operator=(io);
            m_outputType = io.m_outputType;
            m_plotType = io.m_plotType;
            return *this;
        }
        virtual CNumericIOBase&  operator=(const CNumericIOBase&& io)
        {
            CWorkflowTaskIO::operator=(io);
            m_outputType = std::move(io.m_outputType);
            m_plotType = std::move(io.m_plotType);
            return *this;
        }

        /**
         * @brief Set the output display type used to visualize numeric values.
         * @param type : see ::NumericOutputType for details.
         */
        inline void                 setOutputType(NumericOutputType type)
        {
            m_outputType = type;
        }
        /**
         * @brief Set the plot type used to visualize numeric values.
         * @details This property is used only if the output display type is set to ::NumericOutputType::PLOT. Use setOutputType method to set the output display type.
         * @param type : see ::PlotType for details.
         */
        inline void                 setPlotType(PlotType type)
        {
            m_plotType = type;
        }

        /**
         * @brief Gets current output display type.
         * @return Output display type. See ::NumericOutputType for details.
         */
        inline NumericOutputType    getOutputType() const
        {
            return m_outputType;
        }
        /**
         * @brief Gets current plot type.
         * @return Plot type. See ::PlotType for details.
         */
        inline PlotType             getPlotType() const
        {
            return m_plotType;
        }
        /**
         * @brief Gets associated label list at index i.
         * @param i: index of the desired label list.
         * @return Vector of labels.
         */
        inline StringVector         getValueLabelList(size_t index) const
        {
            if(index < m_valueLabels.size())
                return m_valueLabels[index];
            else
                return std::vector<std::string>();
        }
        /**
         * @brief Gets associated header label for list at index i.
         * @param i: index of the desired list.
         * @return Header label.
         */
        inline std::string          getHeaderLabel(size_t index) const
        {
            if(index < m_headerLabels.size())
                return m_headerLabels[index];
            else
                return std::string();
        }
        /**
         * @brief Get all associated label lists.
         * @return Vector of vector of labels.
         */
        inline VectorOfStringVector getAllValueLabels() const
        {
            return m_valueLabels;
        }
        /**
         * @brief Get all associated header labels.
         * @return Vector of labels.
         */
        inline StringVector         getAllHeaderLabels() const
        {
            return m_headerLabels;
        }

        virtual VectorOfStringVector getAllValuesAsString() const = 0;

        virtual void                save(const std::string &path) override
        {
            Q_UNUSED(path);
        }

    protected:

        NumericOutputType                       m_outputType = NumericOutputType::TABLE;
        PlotType                                m_plotType = PlotType::CURVE;
        std::vector<std::string>                m_headerLabels;
        std::vector<std::vector<std::string>>   m_valueLabels;
};

/**
 * @ingroup groupCore
 * @brief The CNumericIO class defines an input or output for a workflow task dedicated to numeric values management.
 * @details This class is designed to handle numeric values as input or output of a workflow task. A CNumericIO<T> instance can be added as input or output to a CWorkflowTask or derived object.
 * It consists on a list of values (generic type), a list of associated labels and a display type (::NumericOutputType).
 * For the specific case of plot display, a plot type property is available (::PlotType).
 */
template<class Type>
class CNumericIO : public CNumericIOBase
{
    public:

        using NumericValues = std::vector<Type>;
        using VectorOfNumericValues = std::vector<std::vector<Type>>;
        using NumericIOPtr = std::shared_ptr<CNumericIO>;

        /**
         * @brief Default constructor
         */
        CNumericIO() : CNumericIOBase()
        {
            m_description = QObject::tr("Numerical values structured as table data (headers, labels and values).\n"
                                        "Can be displayed as table or plot.").toStdString();
            m_saveFormat = DataFileFormat::CSV;
        }

        CNumericIO(const std::string& name) : CNumericIOBase(name)
        {
            m_description = QObject::tr("Numerical values structured as table data (headers, labels and values).\n"
                                        "Can be displayed as table or plot.").toStdString();
            m_saveFormat = DataFileFormat::CSV;
        }
        /**
         * @brief Copy constructor
         */
        CNumericIO(const CNumericIO& io) : CNumericIOBase(io)
        {
            m_values = io.m_values;
            m_valueLabels = io.m_valueLabels;
            m_headerLabels = io.m_headerLabels;
        }
        /**
         * @brief Universal reference copy constructor
         */
        CNumericIO(const CNumericIO&& io) : CNumericIOBase(io)
        {
            m_values = std::move(io.m_values);
            m_valueLabels = std::move(io.m_valueLabels);
            m_headerLabels = std::move(io.m_headerLabels);
        }

        virtual ~CNumericIO(){}

        /**
         * @brief Assignement operator
         */
        virtual CNumericIO&  operator=(const CNumericIO& io)
        {
            CNumericIOBase::operator=(io);
            m_values = io.m_values;
            m_valueLabels = io.m_valueLabels;
            m_headerLabels = io.m_headerLabels;
            return *this;
        }
        /**
         * @brief Universal reference assignement operator
         */
        virtual CNumericIO&  operator=(const CNumericIO&& io)
        {
            CNumericIOBase::operator=(io);
            m_values = std::move(io.m_values);
            m_valueLabels = std::move(io.m_valueLabels);
            m_headerLabels = std::move(io.m_headerLabels);
            return *this;
        }

        /**
         * @brief Checks if some numeric values or labels are available.
         * @return True if numeric value list or label list are not empty, False otherwise.
         */
        bool                        isDataAvailable() const override
        {
            return (m_values.size() > 0 || m_valueLabels.size() > 0);
        }

        /**
         * @brief Clears numeric value list and label list.
         */
        virtual void                clearData() override
        {
            m_values.clear();
            m_valueLabels.clear();
            m_headerLabels.clear();
        }

        /**
         * @brief Appends value list of type Type.
         * @param values: generic values vector.
         */
        inline void                 addValueList(const std::vector<Type>& values)
        {
            m_values.push_back(values);
        }
        /**
         * @brief Appends value list of type Type.
         * @param values: generic values vector.
         * @param header: associated header label.
         */
        inline void                 addValueList(const std::vector<Type>& values, const std::string& headerLabel)
        {
            m_values.push_back(values);
            m_headerLabels.push_back(headerLabel);
        }
        /**
         * @brief Appends value list of type Type.
         * @param values: generic values vector.
         * @param labels: associated label for each value.
         */
        inline void                 addValueList(const std::vector<Type>& values, const std::vector<std::string>& labels)
        {
            if(values.size() != labels.size())
                throw CException(CoreExCode::INVALID_SIZE, "Value and label list must have the same size", __func__, __FILE__, __LINE__);

            m_values.push_back(values);
            m_valueLabels.push_back(labels);
        }
        /**
         * @brief Appends value list of type Type.
         * @param values: generic values vector.
         * @param header: associated header label.
         * @param labels: associated label for each value.
         */
        inline void                 addValueList(const std::vector<Type>& values, const std::string& headerLabel, const std::vector<std::string>& labels)
        {
            if(values.size() != labels.size())
                throw CException(CoreExCode::INVALID_SIZE, "Value and label list must have the same size", __func__, __FILE__, __LINE__);

            m_values.push_back(values);
            m_headerLabels.push_back(headerLabel);
            m_valueLabels.push_back(labels);
        }

        /**
         * @brief Gets value list at index i.
         * @param i: index of the desired value list.
         * @return Vector of values (generic type).
         */
        inline NumericValues        getValueList(size_t index) const
        {
            if(index < m_values.size())
                return m_values[index];
            else
                return std::vector<Type>();
        }
        /**
         * @brief Get all value lists.
         * @return Vector of vector of values (generic type).
         */
        inline VectorOfNumericValues getAllValues() const
        {
            return m_values;
        }
        inline VectorOfStringVector getAllValuesAsString() const override
        {
            VectorOfStringVector strValues;
            for(size_t i=0; i<m_values.size(); ++i)
            {
                StringVector valueList;
                for(size_t j=0; j<m_values[i].size(); ++j)
                    valueList.push_back(Utils::to_string(m_values[i][j]));

                strValues.push_back(valueList);
            }
            return strValues;
        }

        /**
         * @brief Performs a deep copy of the object.
         * @return CNumericIO smart pointer (std::shared_ptr).
         */
        NumericIOPtr                clone()
        {
            return std::static_pointer_cast<CNumericIO>(cloneImp());
        }

        void                        save() override
        {
            std::string path = m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
            save(path);
        }
        void                        save(const std::string& path) override
        {
            auto extension = Utils::File::extension(path);
            if(extension == ".csv")
                saveCSV(path);
            else if(extension == ".xml")
                saveXML(path);
        }

    private:

        virtual WorkflowTaskIOPtr   cloneImp() const override
        {
            return std::shared_ptr<CNumericIO>(new CNumericIO(*this));
        }

        void                        saveCSV(const std::string &path) const
        {
            std::ofstream file;
            file.open(path, std::ios::out | std::ios::trunc);

            //Write header labels
            if(m_headerLabels.size() > 0)
            {
                if(m_valueLabels.size() > 0)
                    file << ";";

                for(size_t i=0; i<m_headerLabels.size(); ++i)
                    file << m_headerLabels[i] + ";";
                file << "\n";
            }

            //Count rows
            size_t nbRow = 0;
            for(size_t i=0; i<m_values.size(); ++i)
                nbRow = std::max(nbRow, m_values[i].size());

            //Write values and associated labels (if there are some)
            if(m_values.size() == m_valueLabels.size())
            {
                for(size_t i=0; i<nbRow; ++i)
                {
                    for(size_t j=0; j<m_values.size(); ++j)
                    {
                        file << m_valueLabels[j][i] + ";";
                        file << Utils::to_string(m_values[j][i]) + ";";
                    }
                    file << "\n";
                }
            }
            else if(m_valueLabels.size() == 1)
            {
                for(size_t i=0; i<nbRow; ++i)
                {
                    file << m_valueLabels[0][i] + ";";
                    for(size_t j=0; j<m_values.size(); ++j)
                        file << Utils::to_string(m_values[j][i]) + ";";

                    file << "\n";
                }
            }
            else
            {
                for(size_t i=0; i<nbRow; ++i)
                {
                    for(size_t j=0; j<m_values.size(); ++j)
                        file << Utils::to_string(m_values[j][i]) + ";";

                    file << "\n";
                }
            }
            file.close();
        }
        void                        saveXML(const std::string &path) const
        {
            Q_UNUSED(path);
            throw CException(CoreExCode::NOT_IMPLEMENTED, "Feature not available yet", __func__, __FILE__, __LINE__);
        }

    private:

        VectorOfNumericValues       m_values;
};

// Partial specialization
template <>
DATAPROCESSSHARED_EXPORT CNumericIOBase::VectorOfStringVector CNumericIO<std::string>::getAllValuesAsString() const;

template <>
DATAPROCESSSHARED_EXPORT void CNumericIO<std::string>::saveCSV(const std::string &path) const;


class DATAPROCESSSHARED_EXPORT CNumericIOFactory: public CWorkflowTaskIOFactory
{
public:

    CNumericIOFactory()
    {
        m_name = "CNumericIO";
    }

    virtual WorkflowTaskIOPtr   create(IODataType dataType)
    {
        Q_UNUSED(dataType);
        return std::make_shared<CNumericIO<double>>();
    }
};

#endif // CNUMERICIO_H
