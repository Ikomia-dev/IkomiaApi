#include <QTest>
#include "CToolsTests.h"
#include "UtilsTools.hpp"

CToolsTests::CToolsTests(QObject *parent) : QObject(parent)
{
}

void CToolsTests::variousFunctions()
{
    //toVariantList
    QList<int> intList;
    QVariantList list1 = Utils::toVariantList(intList);
    QVERIFY(list1.size() == 0);

    int nbElts = 10;
    for(int i=0; i<nbElts; ++i)
        intList.push_back(i);

    QVariantList list2 = Utils::toVariantList(intList);
    QVERIFY(list2.size() == nbElts);
    for(int i=0; i<nbElts; ++i)
    {
        QVERIFY(list2[i].isValid());
        QVERIFY(list2[i].toInt() == i);
    }
}

void CToolsTests::stringFunctions()
{
    //Tokenize
    std::vector<std::string> tokens;
    std::string str = "";
    Utils::String::tokenize(str, tokens, ".");
    QVERIFY(tokens.size() == 0);

    str = "toto_tata_titi";
    Utils::String::tokenize(str, tokens, ".");
    QVERIFY(tokens.size() == 1);
    tokens.clear();

    Utils::String::tokenize(str, tokens, "_");
    QVERIFY(tokens.size() == 3);
    QVERIFY(tokens[0] == "toto");
    QVERIFY(tokens[1] == "tata");
    QVERIFY(tokens[2] == "titi");

    //DBFormat: convert string to valid string for database
    str = "valid_string!";
    auto str1 = Utils::String::dbFormat(str);
    QVERIFY(str1 == str);

    str = "l'invalid_string!";
    str1 = Utils::String::dbFormat(str);
    QVERIFY(str1 != str);
    QVERIFY(str1 == "l''invalid_string!");
}

void CToolsTests::databaseFunctions()
{
    //Connection
    //Test must failed
    QVERIFY_EXCEPTION_THROWN(Utils::Database::connect("/folder_not_exists", ""), CException);
    //Test must succeed
    auto db = Utils::Database::connect("mydatabase.db", "myconnection");
    QVERIFY(db.isValid());
    QVERIFY(db.isOpen());

    //Query size
    int size = 77;
    db = createTestDB(size);

    QSqlQuery q(db);
    if(!q.exec("SELECT * FROM tests;"))
        throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

    int returnSize = Utils::Database::getQuerySize(q);
    QVERIFY(returnSize == size);
}

void CToolsTests::geometryFunctions()
{
    QPointF p1(0, 0);
    QPointF p2(10, 10);
    float dist = Utils::Geometry::sqrDistance(p1, p2);
    QVERIFY(dist == 200);

    p2 = QPoint(-10, 10);
    dist = Utils::Geometry::sqrDistance(p1, p2);
    QVERIFY(dist == 200);

    p1 = QPoint(-10, 10);
    dist = Utils::Geometry::sqrDistance(p1, p2);
    QVERIFY(dist == 0);
}

void CToolsTests::fileFunctions()
{
    const std::string pathTest = "/home/user/images/image.png";

    std::string ext = Utils::File::extension(pathTest);
    QVERIFY(ext == ".png");

    std::string nameNoExt = Utils::File::getFileNameWithoutExtension(pathTest);
    QVERIFY(nameNoExt == "image");

    std::string name = Utils::File::getFileName(pathTest);
    QVERIFY(name == "image.png");

    std::string parentPath = Utils::File::getParentPath(pathTest);
    QVERIFY(parentPath == "/home/user/images");

    std::string pathPattern = "/home/user/images/frame_%04d.png";
    std::string path = Utils::File::getPathFromPattern(pathPattern, 0);
    QVERIFY(path != pathPattern && path == "/home/user/images/frame_0000.png");
    pathPattern = "/home/user/images/frame_%4d.png";
    path = Utils::File::getPathFromPattern(pathPattern, 11);
    QVERIFY(path != pathPattern && path == "/home/user/images/frame_0011.png");

    pathPattern = "/run/media/ludo/data/Ludo/Work/Ikomia/Videos/Datasets/MPI-Sintel-testing/test/clean/ambush_1/frame_%04d.png";
    bool bFind = Utils::File::isFileSequenceExist(pathPattern);
    QVERIFY(bFind);
    pathPattern = "/run/media/ludo/data/Ludo/Work/Ikomia/Videos/Datasets/MPI-Sintel-testing/test/clean/ambush_1/image_%04d.png";
    bFind = Utils::File::isFileSequenceExist(pathPattern);
    QVERIFY(!bFind);
    pathPattern = "/run/media/ludo/data/Ludo/Work/Ikomia/Videos/Datasets/MPI-Sintel-testing/test/clean/ambush_1/frame%04d.jpg";
    bFind = Utils::File::isFileSequenceExist(pathPattern);
    QVERIFY(!bFind);
}

QSqlDatabase CToolsTests::createTestDB(int rows)
{
    auto db =  Utils::Database::connect(":memory:", "UnitTestConnection");
    assert(db.isValid());

    QStringList tables = db.tables(QSql::Tables);
    QSqlQuery q(db);

    if(tables.contains("tests") == true)
    {
        if(!q.exec("DROP TABLE tests;"))
            throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
    }

    if(!q.exec("CREATE TABLE tests (id INTEGER PRIMARY KEY, name TEXT, result INTEGER);"))
        throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

    QVariantList names, results;
    q.prepare("INSERT INTO tests (name, result) VALUES (?, ?);");

    for(int i=0; i<rows; ++i)
    {
        names << "test_n°_" + QString::number(i);
        results << 1;
    }
    q.addBindValue(names);
    q.addBindValue(results);

    if(!q.execBatch())
        throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

    return db;
}

QTEST_GUILESS_MAIN(CToolsTests)
