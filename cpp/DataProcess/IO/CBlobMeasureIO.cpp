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

#include "CBlobMeasureIO.h"
#include "CException.h"
#include "Main/CoreTools.hpp"

//--------------------------------//
//----- class CObjectMeasure -----//
//--------------------------------//
CObjectMeasure::CObjectMeasure()
{
}

CObjectMeasure::CObjectMeasure(const CMeasure &measure, double value, size_t graphicsId, const std::string& label)
{
    m_measure = measure;
    m_values.push_back(value);
    m_graphicsId = graphicsId;
    m_label = label;
}

CObjectMeasure::CObjectMeasure(const CMeasure &measure, const std::vector<double> &values, size_t graphicsId, const std::string &label)
{
    m_measure = measure;
    m_values = values;
    m_graphicsId = graphicsId;
    m_label = label;
}

CObjectMeasure::CObjectMeasure(const CMeasure &measure, std::initializer_list<double> values, size_t graphicsId, const std::string &label)
{
    m_measure = measure;
    m_values.insert(m_values.end(), values);
    m_graphicsId = graphicsId;
    m_label = label;
}

CMeasure CObjectMeasure::getMeasureInfo() const
{
    return m_measure;
}

std::vector<double> CObjectMeasure::getValues() const
{
    return m_values;
}

void CObjectMeasure::setValues(const std::vector<double> &values)
{
    m_values = values;
}

//--------------------------------//
//----- class CBlobMeasureIO -----//
//--------------------------------//
CBlobMeasureIO::CBlobMeasureIO() : CWorkflowTaskIO(IODataType::BLOB_VALUES, "CBlobMeasureIO")
{
    m_description = QObject::tr("Predefined measures computed from connected components (Surface, perimeter...).").toStdString();
    m_saveFormat = DataFileFormat::CSV;
}

CBlobMeasureIO::CBlobMeasureIO(const std::string &name) : CWorkflowTaskIO(IODataType::BLOB_VALUES, name)
{
    m_description = QObject::tr("Predefined measures computed from connected components (Surface, perimeter...).").toStdString();
    m_saveFormat = DataFileFormat::CSV;
}

CBlobMeasureIO::CBlobMeasureIO(const CBlobMeasureIO& io) : CWorkflowTaskIO(io)
{
    m_measures = io.m_measures;
}

CBlobMeasureIO::CBlobMeasureIO(const CBlobMeasureIO&& io) : CWorkflowTaskIO(io)
{
    m_measures = std::move(io.m_measures);
}

CBlobMeasureIO::~CBlobMeasureIO()
{
}

CBlobMeasureIO &CBlobMeasureIO::operator=(const CBlobMeasureIO& io)
{
    CWorkflowTaskIO::operator=(io);
    m_measures = io.m_measures;
    return *this;
}

CBlobMeasureIO &CBlobMeasureIO::operator=(const CBlobMeasureIO&& io)
{
    CWorkflowTaskIO::operator=(io);
    m_measures = std::move(io.m_measures);
    return *this;
}

void CBlobMeasureIO::setObjectMeasure(size_t index, const CObjectMeasure &measure)
{
    if(index >= m_measures.size())
        addObjectMeasure(measure);
    else
    {
        ObjectMeasures measures;
        measures.push_back(measure);
        m_measures[index] = measures;
    }
}

ObjectsMeasures CBlobMeasureIO::getMeasures() const
{
    return m_measures;
}

ObjectMeasures CBlobMeasureIO::getBlobMeasures(size_t index) const
{
    if(index >= m_measures.size())
        throw CException(CoreExCode::INDEX_OVERFLOW, "Blob index out of range");

    return m_measures[index];
}

int CBlobMeasureIO::getBlobMeasureIndex(size_t index, std::string name)
{
    ObjectMeasures measures = getBlobMeasures(index);
    for(size_t i=0; i<measures.size(); ++i)
    {
        if(measures[i].m_measure.m_name == name)
            return i;
    }
    return -1;
}

int CBlobMeasureIO::getBlobMeasureIndex(size_t index, int id)
{
    return getBlobMeasureIndex(index, CMeasure::getName(id));
}

bool CBlobMeasureIO::isDataAvailable() const
{
    return m_measures.size() > 0;
}

void CBlobMeasureIO::addObjectMeasure(const CObjectMeasure &measure)
{
    ObjectMeasures measures;
    measures.push_back(measure);
    m_measures.push_back(measures);
}

void CBlobMeasureIO::addObjectMeasures(const std::vector<CObjectMeasure> &measures)
{
    m_measures.push_back(measures);
}

void CBlobMeasureIO::clearData()
{
    m_measures.clear();
}

void CBlobMeasureIO::save()
{
    std::string path = m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
    save(path);
}

void CBlobMeasureIO::save(const std::string &path)
{
    auto extension = Utils::File::extension(path);
    if(extension == ".csv")
        saveCSV(path);
    else if(extension == ".xml")
        saveXML(path);
}

std::set<std::string> CBlobMeasureIO::getMeasuresNames() const
{
    std::set<std::string> names;
    //Iterate throw objects (ie lines)
    for(size_t i=0; i<m_measures.size(); ++i)
    {
        //Iterate throw object measures (ie columns)
        for(size_t j=0; j<m_measures[i].size(); ++j)
            names.insert(m_measures[i][j].m_measure.m_name);
    }
    return names;
}

void CBlobMeasureIO::saveCSV(const std::string &path) const
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::trunc);

    auto names = getMeasuresNames();
    file << "Object;Id;Category;";

    for(auto it=names.begin(); it!=names.end(); ++it)
        file << *it + ";";
    file << "\n";

    //Iterate throw objects (ie lines)
    for(size_t i=0; i<m_measures.size(); ++i)
    {
        std::map<std::string, std::string>  mapNameValues;
        file << std::to_string(i) + ";";
        file << std::to_string(m_measures[i][0].m_graphicsId) + ";";
        file << m_measures[i][0].m_label + ";";

        //Iterate throw object measures (ie columns)
        for(size_t j=0; j<m_measures[i].size(); ++j)
        {
            std::string strValues;
            auto measure = m_measures[i][j];

            for(size_t k=0; k<measure.m_values.size(); ++k)
            {
                strValues += std::to_string(measure.m_values[k]);
                if(k != measure.m_values.size() - 1)
                    strValues += "-";
            }
            mapNameValues.insert(std::make_pair(m_measures[i][j].m_measure.m_name, strValues));
        }

        for(auto it=names.begin(); it!=names.end(); ++it)
        {
            auto itMeasure = mapNameValues.find(*it);
            if(itMeasure == mapNameValues.end())
                file << ";";
            else
                file << itMeasure->second + ";";
        }
        file << "\n";
    }
    file.close();
}

void CBlobMeasureIO::saveXML(const std::string &path) const
{
    Q_UNUSED(path);
    throw CException(CoreExCode::NOT_IMPLEMENTED, "Feature not available yet", __func__, __FILE__, __LINE__);
}

std::shared_ptr<CBlobMeasureIO> CBlobMeasureIO::clone() const
{
    return std::static_pointer_cast<CBlobMeasureIO>(cloneImp());
}

std::shared_ptr<CWorkflowTaskIO> CBlobMeasureIO::cloneImp() const
{
    return std::shared_ptr<CBlobMeasureIO>(new CBlobMeasureIO(*this));
}
