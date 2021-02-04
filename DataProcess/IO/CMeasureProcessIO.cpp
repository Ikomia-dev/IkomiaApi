#include "CMeasureProcessIO.h"
#include "CException.h"
#include "Main/CoreTools.hpp"

//--------------------------------//
//----- class CObjectMeasure -----//
//--------------------------------//
CObjectMeasure::CObjectMeasure()
{
}

CObjectMeasure::CObjectMeasure(CMeasure measure, const double &value, size_t graphicsId, const std::string& label)
{
    m_measure = measure;
    m_values.push_back(value);
    m_graphicsId = graphicsId;
    m_label = label;
}

CObjectMeasure::CObjectMeasure(CMeasure measure, std::initializer_list<double> values, size_t graphicsId, const std::string &label)
{
    m_measure = measure;
    m_values.insert(m_values.end(), values);
    m_graphicsId = graphicsId;
    m_label = label;
}

//-----------------------------------//
//----- class CMeasureProcessIO -----//
//-----------------------------------//
CMeasureProcessIO::CMeasureProcessIO() : CProtocolTaskIO(IODataType::BLOB_VALUES)
{
    m_name = "MeasureProcessIO";
    m_description = QObject::tr("Predefined measures computed from connected components (Surface, perimeter...).").toStdString();
    m_saveFormat = DataFileFormat::CSV;
}

CMeasureProcessIO::CMeasureProcessIO(const CMeasureProcessIO& io) : CProtocolTaskIO(io)
{
    m_measures = io.m_measures;
}

CMeasureProcessIO::CMeasureProcessIO(const CMeasureProcessIO&& io) : CProtocolTaskIO(io)
{
    m_measures = std::move(io.m_measures);
}

CMeasureProcessIO::~CMeasureProcessIO()
{
}

CMeasureProcessIO &CMeasureProcessIO::operator=(const CMeasureProcessIO& io)
{
    CProtocolTaskIO::operator=(io);
    m_measures = io.m_measures;
    return *this;
}

CMeasureProcessIO &CMeasureProcessIO::operator=(const CMeasureProcessIO&& io)
{
    CProtocolTaskIO::operator=(io);
    m_measures = std::move(io.m_measures);
    return *this;
}

void CMeasureProcessIO::setObjectMeasure(size_t index, const CObjectMeasure &measure)
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

ObjectsMeasures CMeasureProcessIO::getMeasures() const
{
    return m_measures;
}

bool CMeasureProcessIO::isDataAvailable() const
{
    return m_measures.size() > 0;
}

void CMeasureProcessIO::addObjectMeasure(const CObjectMeasure &measure)
{
    ObjectMeasures measures;
    measures.push_back(measure);
    m_measures.push_back(measures);
}

void CMeasureProcessIO::addObjectMeasures(const std::vector<CObjectMeasure> &measures)
{
    m_measures.push_back(measures);
}

void CMeasureProcessIO::clearData()
{
    m_measures.clear();
}

void CMeasureProcessIO::save()
{
    std::string path = m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
    save(path);
}

void CMeasureProcessIO::save(const std::string &path)
{
    auto extension = Utils::File::extension(path);
    if(extension == ".csv")
        saveCSV(path);
    else if(extension == ".xml")
        saveXML(path);
}

std::set<std::string> CMeasureProcessIO::getMeasuresNames() const
{
    std::set<std::string> names;
    //Iterate throw objects (ie lines)
    for(size_t i=0; i<m_measures.size(); ++i)
    {
        //Iterate throw object measures (ie columns)
        for(size_t j=0; j<m_measures[i].size(); ++j)
            names.insert(m_measures[i][j].m_measure.getName());
    }
    return names;
}

void CMeasureProcessIO::saveCSV(const std::string &path) const
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
            mapNameValues.insert(std::make_pair(m_measures[i][j].m_measure.getName(), strValues));
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

void CMeasureProcessIO::saveXML(const std::string &path) const
{
    Q_UNUSED(path);
    throw CException(CoreExCode::NOT_IMPLEMENTED, "Feature not available yet", __func__, __FILE__, __LINE__);
}

std::shared_ptr<CMeasureProcessIO> CMeasureProcessIO::clone() const
{
    return std::static_pointer_cast<CMeasureProcessIO>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CMeasureProcessIO::cloneImp() const
{
    return std::shared_ptr<CMeasureProcessIO>(new CMeasureProcessIO(*this));
}
