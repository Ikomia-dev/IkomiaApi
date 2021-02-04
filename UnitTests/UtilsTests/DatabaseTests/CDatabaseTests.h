#ifndef CDATABASETEST_H
#define CDATABASETEST_H

#include <QString>
#include <QtTest>
#include <QTreeView>
//#include <variant>
//#include <View/CContextMenu.hpp>
#include <QtSql>

class CDatabaseTests : public QObject
{
        Q_OBJECT

    public:
        CDatabaseTests();

    private:

        void testCreateDb();
        void testCreateTable();
        void testAdd();
        void testAllLeaf();
        void testAllNodes();
        void testAllLeafFrom();
        void testAllNodesFrom();
        void testSubTree();
        void testSubTreeComp();
        void testAllParent();
        void testRoot();
        void testNbLeaf();
        void testNbNodes();
        void testRemove();

    public slots:
        //void onCustomContextMenu(const QPoint &pos);

    private slots:
        /*
        void testDatabase();
        void testInit1();
        void testInit2();
        void testInit3();*/

    private:
        QSqlError m_error;
        std::string m_error_text;
        QString m_tableTitle = "intervalGraph";
        QTreeView* m_pTreeView = nullptr;
        //CMultiContextMenu<menu_sig> m_contextMenu;
};

#endif // CDATABASETEST_H
