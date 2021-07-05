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

#include "CMeasure.h"
#include "UtilsTools.hpp"
#include "Main/CoreTools.hpp"

std::array<CMeasure::Id,9> CMeasure::m_internalMeasures = { CMeasure::Id::SURFACE, CMeasure::Id::PERIMETER,
                                                            CMeasure::Id::CENTROID, CMeasure::Id::BBOX,
                                                            CMeasure::Id::ORIENTED_BBOX, CMeasure::Id::EQUIVALENT_DIAMETER,
                                                            CMeasure::Id::ELONGATION, CMeasure::Id::CIRCULARITY,
                                                            CMeasure::Id::SOLIDITY };

CMeasure::CMeasure()
{
}

CMeasure::CMeasure(int id)
{
    m_id = id;
    m_name = getName(id);
}

CMeasure::CMeasure(int id, const std::string &name)
{
    m_id = id;
    m_name = name;
}

CMeasure::CMeasure(int id, const std::string &name, const std::string &formula)
{
    m_id = id;
    m_name = name;
    m_formula = formula;
}

std::vector<CMeasure> CMeasure::getAvailableMeasures()
{
    std::vector<CMeasure> measures;
    for(size_t i=0; i<m_internalMeasures.size(); ++i)
    {
        CMeasure measure;
        measure.m_id = m_internalMeasures[i];
        measure.m_name = CMeasure::getName(measure.m_id);
        measures.push_back(measure);
    }

    auto db = Utils::Database::connect(Utils::Database::getMainPath(), Utils::Database::getMainConnectionName());
    if(db.isValid())
    {
        QSqlQuery q(db);
        if(!q.exec("SELECT * FROM customMeasureInfo;"))
            throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

        while(q.next())
        {
            CMeasure measure;
            measure.m_id = static_cast<Id>(q.value(0).toInt());
            measure.m_name = q.value(1).toString().toStdString();
            measure.m_formula = q.value(2).toString().toStdString();
            measures.push_back(measure);
        }
    }
    return measures;
}

std::string CMeasure::getName(int id)
{
    std::string name = "";
    switch (id)
    {
        case Id::SURFACE: name = QObject::tr("Surface").toStdString(); break;
        case Id::PERIMETER: name = QObject::tr("Perimeter").toStdString(); break;
        case Id::CENTROID: name = QObject::tr("Centroid").toStdString(); break;
        case Id::BBOX: name = QObject::tr("Bounding box").toStdString(); break;
        case Id::ORIENTED_BBOX: name = QObject::tr("Oriented bounding box").toStdString(); break;
        case Id::EQUIVALENT_DIAMETER: name = QObject::tr("Equivalent diameter").toStdString(); break;
        case Id::ELONGATION: name = QObject::tr("Elongation").toStdString(); break;
        case Id::CIRCULARITY: name = QObject::tr("Circularity").toStdString(); break;
        case Id::SOLIDITY: name = QObject::tr("Solidity").toStdString(); break;
        case Id::CUSTOM: name = QObject::tr("Custom measure").toStdString(); break;
    }
    return name;
}
