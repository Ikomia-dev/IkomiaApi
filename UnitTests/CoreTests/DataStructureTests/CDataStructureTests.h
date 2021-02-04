#ifndef CDATASTRUCTURETESTS_H
#define CDATASTRUCTURETESTS_H

#include <QObject>
#include "Main/CoreDefine.hpp"

class CDataStructureTests: public QObject
{
    Q_OBJECT

    public:

        explicit CDataStructureTests(QObject* parent = nullptr);

    private:

        std::string sampleImgPath();

        Dimensions  dimensions(const SubsetBounds& bounds);

    private slots:

        void        ndArrayConstructors();
        void        ndArrayAccessors();
        void        ndArraySubArrayCreation();
        void        ndArrayUpDimension();
        void        ndArrayDownDimension();
        void        ndArrayInsertDimension();

        void        datasetConstructors();
        void        datasetAccessors();
        void        datasetIndexConversion();
        void        dataConversion();

        void        loadSingleImage();
        void        loadMultiImages();
        void        loadZStackImages();
        void        loadNdImages();
};

#endif // CDATASTRUCTURETESTS_H
