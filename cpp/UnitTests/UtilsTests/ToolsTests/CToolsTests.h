#ifndef CTOOLSTESTS_H
#define CTOOLSTESTS_H

#include <QObject>
#include <QSqlDatabase>

class CToolsTests : public QObject
{
    Q_OBJECT

    public:
        explicit CToolsTests(QObject *parent = nullptr);

    private slots:

        void            variousFunctions();
        void            stringFunctions();
        void            databaseFunctions();
        void            geometryFunctions();
        void            fileFunctions();

    private:

        QSqlDatabase    createTestDB(int rows);
};

#endif // CTOOLSTESTS_H
