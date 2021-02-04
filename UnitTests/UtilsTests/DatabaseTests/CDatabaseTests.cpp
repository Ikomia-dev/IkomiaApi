#include "CDatabaseTests.h"
#include <QAbstractItemModel>
#include <QTreeView>
#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QtWidgets>
#include <QFileDialog>

CDatabaseTests::CDatabaseTests()
{
}
/*
void CDatabaseTest::testCreateDb()
{
    m_error = m_db.createDb("QSQLITE", ":memory:");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());
}

void CDatabaseTest::testCreateTable()
{
    m_error = m_db.createTree(m_tableTitle);
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    m_error = m_db.setTreeRoot("Root");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    m_db.setCurrentTreeName(m_tableTitle);
}

void CDatabaseTest::testAdd()
{
    size_t ind = 1;

    for(int i=1; i<3; ++i)
    {
        size_t indProject = ind++;
        m_error = m_db.addItem("Project"+QString::number(i), static_cast<size_t>(CStandardDbManager::TreeItemType::PROJECT), indProject, 0);
        m_error_text = m_error.text().toUtf8().constData();
        QVERIFY2(m_error.isValid() == false, m_error_text.c_str());
        for(int j=1; j<3; ++j)
        {
            size_t indFolder = ind++;
            m_error = m_db.addItem("Folder"+QString::number(j), static_cast<size_t>(CStandardDbManager::TreeItemType::FOLDER), indFolder, indProject);
            m_error_text = m_error.text().toUtf8().constData();
            QVERIFY2(m_error.isValid() == false, m_error_text.c_str());
            for(int k=1; k<3; ++k)
            {
                size_t indImage = ind++;
                m_error = m_db.addItem("Image"+QString::number(k), static_cast<size_t>(CStandardDbManager::TreeItemType::IMAGE), indImage, indFolder);
                m_error_text = m_error.text().toUtf8().constData();
                QVERIFY2(m_error.isValid() == false, m_error_text.c_str());
            }
        }
    }
}

void CDatabaseTest::testAllLeaf()
{
    QSqlQuery q;
    m_error = m_db.getAllLeaf(q);
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto listLeaf = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(listLeaf.isEmpty() == false);
    for(auto& it : listLeaf) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "All leaf :  " << listLeaf;
}

void CDatabaseTest::testAllNodes()
{
    QSqlQuery q;
    m_error = m_db.getAllNodes(q);
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto listNode = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(listNode.isEmpty() == false);
    for(auto& it : listNode) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "All nodes : " << listNode;
}

void CDatabaseTest::testAllLeafFrom()
{
    QSqlQuery q;
    m_error = m_db.getAllLeafFrom(q, "Project1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto listLeafFrom = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(listLeafFrom.isEmpty() == false);
    for(auto& it : listLeafFrom) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "All leaf from Project1 : "<< listLeafFrom;
}

void CDatabaseTest::testAllNodesFrom()
{
    QSqlQuery q;
    m_error = m_db.getAllNodesFrom(q, "Project1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto listNodeFrom = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(listNodeFrom.isEmpty() == false);
    for(auto& it : listNodeFrom) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "All nodes from Project1 : " << listNodeFrom;
}

void CDatabaseTest::testSubTree()
{
    QSqlQuery q;
    m_error = m_db.extractSubTreeFrom(q, "Project1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto subTreeFrom = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(subTreeFrom.isEmpty() == false);
    for(auto& it : subTreeFrom) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "SubTree Project1 : " << subTreeFrom;
}

void CDatabaseTest::testSubTreeComp()
{
    QSqlQuery q;
    m_error = m_db.extractComplementarySubTreeFrom(q, "Project1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto subTreeCompFrom = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(subTreeCompFrom.isEmpty() == false);
    for(auto& it : subTreeCompFrom) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "SubTreeComp Project1 : " << subTreeCompFrom;

}

void CDatabaseTest::testAllParent()
{
    QSqlQuery q;
    m_error = m_db.getAllParentFrom(q, "Project1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto allParentFrom = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(allParentFrom.isEmpty() == false);
    for(auto& it : allParentFrom) {
       it = m_db.getName(it.toInt());
    }
    qDebug() << "AllParent Project1 : " << allParentFrom;
}

void CDatabaseTest::testRoot()
{
    QSqlQuery q;
    m_error = m_db.getRoot(q);
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    auto root = m_db.getQueryValueFromIndex(q, 1);
    QVERIFY(root.isEmpty() == false);
    qDebug() << "Root : " << m_db.getName(root.at(0).toInt());
}

void CDatabaseTest::testNbLeaf()
{
    auto nbLeaf = m_db.getNbLeaf();
    QVERIFY(nbLeaf > 0);
    qDebug() << "Nb Leaf : " << nbLeaf;
}

void CDatabaseTest::testNbNodes()
{
    auto nbNodes = m_db.getNbNode();
    QVERIFY(nbNodes > 0);
    qDebug() << "Nb nodes : " << nbNodes;
}

void CDatabaseTest::testRemove()
{
    m_error = m_db.removeItem("Folder1");
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());
    testAllLeaf();
    testAllNodes();
}

void CDatabaseTest::testDatabase()
{
    testCreateDb();
    testCreateTable();
    testAdd();

    //
    testRoot();
    testNbLeaf();
    testNbNodes();
    testAllLeaf();
    testAllLeafFrom();
    testAllNodes();
    testAllNodesFrom();
    testAllParent();
    testSubTree();
    testSubTreeComp();
    testRemove();

    m_db.closeDb();
}

void CDatabaseTest::testInit1()
{
    QString projectName = "Untitled1";
    m_db.initDb();
    m_error = m_db.createTree(projectName);
    m_error = m_db.setTreeRoot("All projects");
    m_db.setCurrentTreeName(projectName);
    size_t ind = 1;

    for(int i=1; i<3; ++i)
    {
        size_t indProject = ind++;
        m_error = m_db.addItem("Project"+QString::number(i), static_cast<size_t>(CStandardDbManager::TreeItemType::PROJECT), indProject, 0);
        for(int j=1; j<3; ++j)
        {
            size_t indFolder = ind++;
            m_error = m_db.addItem("Folder"+QString::number(j), static_cast<size_t>(CStandardDbManager::TreeItemType::FOLDER), indFolder, indProject);
            for(int k=1; k<3; ++k)
            {
                size_t indImage = ind++;
                m_error = m_db.addItem("Image"+QString::number(k), static_cast<size_t>(CStandardDbManager::TreeItemType::IMAGE), indImage, indFolder);
            }
        }
    }

    auto model = new QStandardItemModel;
    m_db.setModel(model);
    m_error = m_db.fillModelFromDatabase();
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    QDialog dlg;
    QTreeView* pView = new QTreeView(&dlg);
    pView->setModel(model);
    dlg.exec();
    m_db.closeDb();
}

void CDatabaseTest::testInit2()
{
    QString projectName = "Untitled2";
    m_dbProject.initDb();
    m_error = m_dbProject.createTree(projectName);
    m_error = m_dbProject.setTreeRoot("All projects");
    m_dbProject.setCurrentTreeName(projectName);

    size_t ind = 1;

    for(int i=1; i<3; ++i)
    {
        size_t indProject = ind++;
        m_error = m_dbProject.addItem("Project"+QString::number(i), static_cast<size_t>(CProjectDatabase::TreeItemType::PROJECT), indProject, 0);
        for(int j=1; j<3; ++j)
        {
            size_t indFolder = ind++;
            m_error = m_dbProject.addItem("Folder"+QString::number(j), static_cast<size_t>(CProjectDatabase::TreeItemType::FOLDER), indFolder, indProject);
            for(int k=1; k<3; ++k)
            {
                size_t indImage = ind++;
                m_error = m_dbProject.addItem("Image"+QString::number(k), static_cast<size_t>(CProjectDatabase::TreeItemType::IMAGE), indImage, indFolder);
            }
        }
    }

    auto model = new CProjectModel;
    m_dbProject.setModel(model);
    m_error = m_dbProject.fillModelFromDatabase();
    m_error_text = m_error.text().toUtf8().constData();
    QVERIFY2(m_error.isValid() == false, m_error_text.c_str());

    QDialog dlg;
    QTreeView* pView = new QTreeView(&dlg);
    pView->setModel(model);
    dlg.exec();
    m_dbProject.closeDb();
}

void CDatabaseTest::testInit3()
{
    auto AllModel = new CMultiProjectModel;

    QDialog dlg;
    QGridLayout* pLayout = new QGridLayout(&dlg);
    m_pTreeView = new QTreeView;

    QPushButton* pBtnLoad = new QPushButton;
    pBtnLoad->setText("Load");
    connect(pBtnLoad, &QPushButton::clicked, [AllModel, this]{
        QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                 tr("Open Project"), "home/yom/Documents", tr("db Files (*.db)"));
        if(fileName.isEmpty())
            return;

        auto projectModel = new CProjectModel;
        CProjectDbManager project("Project");
        project.load(fileName);
        project.setCurrentTreeName("Project");
        project.setModel(projectModel);
        project.fillModelFromDatabase();

        AllModel->addModel(projectModel);
        m_pTreeView->expandAll();
    });

    QPushButton* pBtnSave = new QPushButton;
    pBtnSave->setText("Save");
    connect(pBtnSave, &QPushButton::clicked, [AllModel, this]{
        auto modelList = AllModel->getAllModels();
        for(auto& it : modelList)
        {
            QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                     tr("Save Project"), "home/yom/Documents", tr("db Files (*.db)"));
            CProjectDbManager project("Project");
            project.save(fileName);
            project.setModel(it);
            project.fillDatabaseFromModel("Project");
        }
    });

    QPushButton* pBtnNew = new QPushButton;
    pBtnNew->setText("New");
    connect(pBtnNew, &QPushButton::clicked, [AllModel, this]{
        CProjectDbManager project("Project");
        project.setProjectFileName(":memory:");
        project.initDb();
        auto projectModel = new CProjectModel;
        project.setModel(projectModel);
        project.fillModelFromDatabase();

        AllModel->addModel(projectModel);
        m_pTreeView->expandAll();
    });

    QPushButton* pBtnClear = new QPushButton;
    pBtnClear->setText("Clear");
    connect(pBtnClear, &QPushButton::clicked, [AllModel, this]{
        AllModel->clear();
    });

    m_pTreeView->setHeaderHidden(true);
    m_pTreeView->setModel(AllModel);
    m_pTreeView->setExpandsOnDoubleClick(false);
    m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_pTreeView, &QTreeView::customContextMenuRequested, this, &CDatabaseTest::onCustomContextMenu);

    pLayout->addWidget(m_pTreeView);
    pLayout->addWidget(pBtnNew);
    pLayout->addWidget(pBtnLoad);
    pLayout->addWidget(pBtnSave);
    pLayout->addWidget(pBtnClear);

    m_contextMenu.addAction(TreeItemType::PROJECT,
                                tr("Add folder"),
                                [AllModel, this](QModelIndex& index){AllModel->addItem(index, "New Folder");} );
    m_contextMenu.addAction(TreeItemType::PROJECT,
                                tr("Remove folder"),
                                [AllModel, this](QModelIndex& index){AllModel->removeItem(index);} );

    dlg.exec();
}

void CDatabaseTest::onCustomContextMenu(const QPoint& pos)
{
    QModelIndex index = m_pTreeView->indexAt(pos);
    if(!index.isValid())
        return;

    m_contextMenu.displayMenu(TreeItemType::PROJECT, m_pTreeView->mapToGlobal(pos), index);
}*/

QTEST_GUILESS_MAIN(CDatabaseTests)

#include "moc_CDatabaseTests.cpp"
