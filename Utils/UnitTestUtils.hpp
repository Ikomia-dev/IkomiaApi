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
