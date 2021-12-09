# include "CNumericIO.h".h"

template <>
std::vector<std::vector<std::string> > CNumericIO<std::string>::getAllValuesAsString() const
{
    return m_values;
}

template <>
void CNumericIO<std::string>::saveCSV(const std::string &path) const
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
                file << m_values[j][i] + ";";
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
                file << m_values[j][i] + ";";

            file << "\n";
        }
    }
    else
    {
        for(size_t i=0; i<nbRow; ++i)
        {
            for(size_t j=0; j<m_values.size(); ++j)
                file << m_values[j][i] + ";";

            file << "\n";
        }
    }
    file.close();
}
