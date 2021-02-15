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

#ifndef UNITTESTUTILS_HPP
#define UNITTESTUTILS_HPP

#include <QDir>

///@cond INTERNAL

namespace Ikomia
{
    namespace UnitTest
    {
        inline std::string getDataPath()
        {
            //Detection du lanceur de la compilation : QtCreator ou Jenkins
            QDir path = QDir::current();
            QDir parentPath = path;
            parentPath.cdUp();

            while(parentPath.dirName() != "Developpement" && parentPath.dirName() != "workspace")
            {
                parentPath.cdUp();
                path.cdUp();
            }

            if(parentPath.dirName() == "Developpement")
                return parentPath.absolutePath().toStdString() + "/Ikomia/UnitTests/Data";
            else
                return path.absolutePath().toStdString() + "/UnitTests/Data";
        }
    }
}

///@endcond

#endif // UNITTESTUTILS_HPP
