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

#ifndef CTREEDBMANAGER_H
#define CTREEDBMANAGER_H

#include <QtSql>
#include <QString>
#include <QStandardItem>
#include <memory>
#include <cassert>
#include "CException.h"
#include "UtilsTools.hpp"

/// @cond INTERNAL

template<class modelType = QStandardItemModel, class itemType = QStandardItem*>
class CTreeDbManager
{
    public:

        using itemMap = QMap<int, itemType>;

        // Useful functions
        void                    makeCurrentDb()
        {
            m_db = QSqlDatabase::database(m_connectionName);
            if(!m_db.isValid())
                throw CException(DatabaseExCode::INVALID_QUERY, m_db.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
        }
        void                    createDb(QString dbType, QString dbName)
        {
            if(!m_db.isValid())
                m_db = QSqlDatabase::addDatabase(dbType, m_connectionName);

            if(!m_db.isValid())
                throw CException(DatabaseExCode::INVALID_QUERY, "Database creation error.", __func__, __FILE__, __LINE__);

            if(m_db.isOpen())
                m_db.close();

            m_db.setDatabaseName(dbName);

            if(!m_db.open())
                throw CException(DatabaseExCode::INVALID_QUERY, m_db.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            m_bHasTransaction = m_db.driver()->hasFeature(QSqlDriver::Transactions);
        }
        void                    closeDb()
        {
            if(m_db.isValid())
                m_db.close();
        }
        void                    createTree(QString treeName)
        {
            QSqlQuery q(m_db);

            m_currentTreeName = treeName;
            // Test if the table treeName exists or not
            auto tables = m_db.tables();
            if(tables.contains(treeName))
            {
                if(!q.exec(QString("delete from %1").arg(treeName)))
                    throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
            }
            else
            {
                // Create table treeName
                if (!q.exec(QString("create table %1 (id integer primary key, typeid integer, name text not null, left integer, right integer, parentid integer, level integer);").arg(treeName)))
                    throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

                //Create index on 'left' field of 'treeName' table to speed up global query
                if(!q.exec(QString("CREATE INDEX leftIndex ON %1 (left);").arg(treeName)))
                    throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
            }
        }

        void                    setTreeRoot(QString rootName, size_t id = 0, size_t typeId = 0)
        {
            QSqlQuery q(m_db);
            if (!q.exec(QString("insert into %1 (left, right, level, name, id, typeid) values (1, 2, 1, '%2', %3, %4);").arg(m_currentTreeName).arg(rootName).arg(id).arg(typeId)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
        }
        void                    setModel(QAbstractItemModel* pModel)
        {
            assert(pModel != nullptr);
            m_pModel = static_cast<modelType*>(pModel);
        }
        void                    setCurrentTreeName(QString treeName)
        {
            m_currentTreeName = treeName;
        }
        void                    activatePragma()
        {
            QSqlQuery q(m_db);
            if(!q.exec("PRAGMA synchronous = OFF"))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            q.exec("PRAGMA journal_mode = MEMORY");
            if(!q.exec("PRAGMA journal_mode = MEMORY"))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            return;
        }

        void                    fillModelFromDatabase()
        {
            assert(m_pModel != nullptr);

            QSqlQuery q(m_db);
            if(!q.exec(QString("SELECT name, level, id, typeid, parentid FROM %1 ORDER BY left;").arg(m_currentTreeName)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            fillModel(q);
            return;
        }
        void                    fillDatabaseFromModel(QString treeName)
        {
            assert(m_pModel != nullptr);
            assert(m_pModel->getRoot() != nullptr);
            assert(m_pModel->getRoot()->getChild(0) != nullptr);

            createTree(treeName);
            // Only 1 child per model since multi models are managed by CMultiModel class
            auto root = m_pModel->getRoot()->getChild(0);
            setTreeRoot(QString::fromStdString(root->getName()), root->getId(), root->getTypeId());
            fillDatabase(root);
            return;
        }

        int                     addItem(QString name, size_t typeId, size_t id, size_t parentId)
        {
            QSqlQuery q(m_db);
            name = QString::fromStdString(Utils::String::dbFormat(name.toStdString()));

            if(!q.exec(QString("select right, level from %1 where id = %2;").arg(m_currentTreeName).arg(parentId)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if(!q.next())
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            int val= q.value(0).toInt();
            int level = q.value(1).toInt();

            if (!q.exec(QString("update %1 set right=right+2 where right>=%2;").arg(m_currentTreeName).arg(val)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if (!q.exec(QString("update %1 set left=left+2 where left>=%2;").arg(m_currentTreeName).arg(val)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if (!q.exec(QString("insert into %1 (id, typeid, name, left, right, parentid, level) values (%2, %3, '%4', %5, %6, %7, %8);")
                        .arg(m_currentTreeName)
                        .arg(id)
                        .arg(typeId)
                        .arg(name)
                        .arg(val)
                        .arg(val+1)
                        .arg(parentId)
                        .arg(level+1)))
            {
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
            }
            return q.lastInsertId().toInt();
        }
        int                     addItem(QString name, size_t typeId, size_t id, QString parentName)
        {
            QSqlQuery q(m_db);
            name = QString::fromStdString(Utils::String::dbFormat(name.toStdString()));
            parentName = QString::fromStdString(Utils::String::dbFormat(parentName.toStdString()));

            if(!q.exec(QString("select id, right, level from %1 where name = '%2';").arg(m_currentTreeName).arg(parentName)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if(!q.next())
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            int parentId = q.value(0).toInt();
            int val= q.value(1).toInt();
            int level = q.value(2).toInt();

            if (!q.exec(QString("update %1 set right=right+2 where right>=%2;").arg(m_currentTreeName).arg(val)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if (!q.exec(QString("update %1 set left=left+2 where left>=%2;").arg(m_currentTreeName).arg(val)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if (!q.exec(QString("insert into %1 (id, typeid, name, left, right, parentid, level) values (%2, %3, '%4', %5, %6, %7, %8);")
                        .arg(m_currentTreeName)
                        .arg(id).arg(typeId)
                        .arg(name)
                        .arg(val)
                        .arg(val+1)
                        .arg(parentId)
                        .arg(level+1)))
            {
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);
            }
            return q.lastInsertId().toInt();
        }

        QSqlError               removeItem(size_t id)
        {
            QSqlQuery q(m_db), qAllLeaf(m_db), qAllNodes(m_db);
            m_error = getAllLeaf(qAllLeaf);
            auto leafs = getQueryValueFromIndex(qAllLeaf, 1);
            m_error= getAllNodes(qAllNodes);
            auto nodes = getQueryValueFromIndex(qAllNodes, 1);

            if(leafs.contains(QString::number(id)))
                removeLeaf(id);
            else if(nodes.contains(QString::number(id)))
                removeNode(id);

            return QSqlError();
        }
        QSqlError               removeItem(QString name)
        {
            QSqlQuery q(m_db), qAllLeaf(m_db), qAllNodes(m_db);

            if(!q.exec(QString("select id from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto val = q.value(0).toString();

            getAllLeaf(qAllLeaf);
            auto leafs = getQueryValueFromIndex(qAllLeaf, 1);
            getAllNodes(qAllNodes);
            auto nodes = getQueryValueFromIndex(qAllNodes, 1);

            if(leafs.contains(val))
                removeLeaf(q.value(0).toInt());
            else if(nodes.contains(val))
                removeNode(q.value(0).toInt());

            return QSqlError();
        }

        QString                 getConnectionName() const
        {
            return m_connectionName;
        }
        // Graph Database manipulations
        QStringList             getQueryValueFromIndex(QSqlQuery& q, size_t index)
        {
            QStringList list;
            while(q.next())
            {
                list.append(q.value(index).toString());
            }
            return list;
        }
        QSqlError               getAllLeaf(QSqlQuery& q)
        {
            if(!q.exec(QString("select * from %1 where right - left = 1;").arg(m_currentTreeName)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getAllLeafFrom(QSqlQuery& q, QString leafName)
        {
            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(leafName)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if(!q.next())
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where right - left = 1 and left > %2 and right < %3;").arg(m_currentTreeName).arg(left).arg(right)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            return QSqlError();
        }
        QSqlError               getAllLeafFrom(QSqlQuery& q, size_t id)
        {
            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            if(!q.next())
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where right - left = 1 and left > %2 and right < %3;").arg(m_currentTreeName).arg(left).arg(right)))
                throw CException(DatabaseExCode::INVALID_QUERY, q.lastError().text().toStdString(), __func__, __FILE__, __LINE__);

            return QSqlError();
        }
        QSqlError               getAllNodes(QSqlQuery& q)
        {
            if(!q.exec(QString("select * from %1 where right - left > 1;").arg(m_currentTreeName)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getAllNodesFrom(QSqlQuery& q, QString nodeName)
        {
            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(nodeName)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where right - left > 1 and left > %2 and right < %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getAllNodesFrom(QSqlQuery& q, size_t id)
        {
            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where right - left > 1 and left > %2 and right < %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getAllParentFrom(QSqlQuery& q, QString name)
        {
            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left < %2 and right > %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getAllParentFrom(QSqlQuery& q, size_t id)
        {
            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left < %2 and right > %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               getRoot(QSqlQuery& q)
        {
            if(!q.exec(QString("select * from %1 where left = 1;").arg(m_currentTreeName)))
                return q.lastError();

            return QSqlError();

        }
        int                     getNbLeaf()
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select count(*) as leafNumber from %1 where left = right - 1;").arg(m_currentTreeName)))
                return 0;

            if(!q.next())
                return 0;

            return q.value("leafNumber").toInt();
        }
        int                     getNbNode()
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select count(*) as nodeNumber from %1 where left <> right - 1;").arg(m_currentTreeName)))
                return 0;

            if(!q.next())
                return 0;

            return q.value("nodeNumber").toInt();
        }
        QString                 getName(size_t id)
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select name from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return QString();

            if(!q.next())
                return QString();

            return q.value(0).toString();
        }

        QSqlError               extractSubTreeFrom(QSqlQuery& q, QString name)
        {
            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left >= %2 and right <= %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               extractSubTreeFrom(QSqlQuery& q, size_t id)
        {
            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left >= %2 and right <= %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               extractComplementarySubTreeFrom(QSqlQuery& q, QString name)
        {
            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left < %2 or right > %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               extractComplementarySubTreeFrom(QSqlQuery& q, size_t id)
        {
            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();

            if(!q.exec(QString("select * from %1 where left < %2 or right > %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            return QSqlError();
        }

        QSqlError               removeLeaf(size_t id)
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select left from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();

            if(!q.exec(QString("delete from %1 where left = %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set left = left - 2 where left >= %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set right = right - 2 where right >= %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               removeLeaf(QString name)
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select left from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();

            if(!q.exec(QString("delete from %1 where left = %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set left = left - 2 where left >= %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set right = right - 2 where right >= %2;").arg(m_currentTreeName).arg(left)))
                return q.lastError();

            return QSqlError();
        }

        QSqlError               removeNode(size_t id)
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select left, right from %1 where id = %2;").arg(m_currentTreeName).arg(id)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();
            auto diff = right - left + 1;

            if(!q.exec(QString("delete from %1 where left >= %2 and right <= %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            if(!q.exec(QString("update %1 set right = right - %2 where right >= %3;").arg(m_currentTreeName).arg(diff).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set left = left - %2 where left >= %3;").arg(m_currentTreeName).arg(diff).arg(left)))
                return q.lastError();

            return QSqlError();
        }
        QSqlError               removeNode(QString name)
        {
            QSqlQuery q(m_db);

            if(!q.exec(QString("select left, right from %1 where name = '%2';").arg(m_currentTreeName).arg(name)))
                return q.lastError();

            if(!q.next())
                return q.lastError();

            auto left = q.value(0).toInt();
            auto right = q.value(1).toInt();
            auto diff = right - left + 1;

            if(!q.exec(QString("delete from %1 where left >= %2 and right <= %3;").arg(m_currentTreeName).arg(left).arg(right)))
                return q.lastError();

            if(!q.exec(QString("update %1 set right = right - %2 where right >= %3;").arg(m_currentTreeName).arg(diff).arg(left)))
                return q.lastError();

            if(!q.exec(QString("update %1 set left = left - %2 where left >= %3;").arg(m_currentTreeName).arg(diff).arg(left)))
                return q.lastError();

            return QSqlError();
        }

    protected:

        // Pure virtual methods that must be reimplemented
        virtual void            fillModel(QSqlQuery query) = 0;
        virtual void            fillDatabase(itemType root) = 0;

        // Template methods
        template<typename T, typename Loader>
        auto                    loadCustomTreeItem(QSqlQuery q)
        {
            assert(m_pModel != nullptr);
            Loader loader;
            loader.setDbManager(this);
            loader.setModel(m_pModel);
            loader.setIndex(m_prevIndex);
            std::shared_ptr<T> item = loader.load(q);
            return item;
        }

        template<typename T, typename Saver>
        auto                    saveCustomTreeItem(T item, int treeItemId)
        {
            assert(item != nullptr);
            Saver saver;
            saver.setDbManager(this);
            saver.save(item, treeItemId);
        }

        // Functions for standard item
        virtual QStandardItem*  loadStandardTreeItem(QSqlQuery q)
        {
            QString nodeName = q.record().value("nodeName").toString();
            return new QStandardItem(nodeName);
        }
        void                    addStandardTreeItem(QSqlQuery q)
        {
            int nodeLevel = q.record().value("nodeLevel").toInt();
            int nodeId = q.record().value("nodeId").toInt();
            int parentId = q.record().value("parentId").toInt();

            // Create standard node
            auto nodeItem = loadStandardTreeItem(q);

            // Liste des éléments de niveau immédiatement inférieur
            auto listPreviousLevel = m_map.value(nodeLevel - 1);

            if (!listPreviousLevel.isEmpty())
            {
                auto parent = listPreviousLevel.value(parentId);
                assert(parent != nullptr);
                parent->appendRow(nodeItem);
                // Ajout de l'élément à la liste des éléments de même niveau
                itemMap listCurrentLevel = m_map.value(nodeLevel);
                listCurrentLevel.insert(nodeId, nodeItem);
                m_map.insert(nodeLevel, listCurrentLevel);
            }
            else
            {
                // Cet élément est le premier dans son niveau : on l'ajoute directement à la racine
                m_pModel->appendRow(nodeItem);
                listPreviousLevel.insert(nodeId, nodeItem);
                m_map.insert(nodeLevel, listPreviousLevel);
            }
        }

    protected:

        QSqlDatabase        m_db;
        QString             m_connectionName = QSqlDatabase::defaultConnection;
        QSqlError           m_error;
        std::string         m_error_text;
        modelType*          m_pModel = nullptr;
        QMap<int, itemMap>  m_map;
        QString             m_currentTreeName = "project";
        QModelIndex         m_prevIndex;
        bool                m_bHasTransaction = false;
};

/// @endcond

#endif // CTREEDBMANAGER_H
