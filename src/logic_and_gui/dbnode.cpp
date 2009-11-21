/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nikiforov@bsc.bg   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "defsandtools.h"
#include "dbnode.h"

// dbNodeClass implementation
dbNodeClass::dbNodeClass(const QSqlDatabase &db)
{
	this->db = db;   
}

dbNodeClass::~dbNodeClass()
{
 	//db.close();   
}

void dbNodeClass::initGivenData(dbNode &data)
{
    data.id 			= 0;
    data.catId 			= 0;      
    data.parent 		= 0;
	data.nodeType 		= 0;
	data.nodeName 		= "";
    data.nodeSize 		= 0;
    data.dateTime 		= 0;
	data.otherNum 		= 0;
	data.iconId 		= 0;
    data.nodeDesc 		= "";
    data.nodeAttr 		= 0;
    data.nodeOwner		= "";
    data.nodeGrp		= "";
    data.nodeFlags 		= 0;
    data.nodeMimeType	= "";
    data.diskNo			= 0;
}
/*
int dbNodeClass::isExistRootElement(int catId)
{
    QSqlQuery *q = NULL;
    int result = -1;
    q = new QSqlQuery(db);
    if (!q)
        return result;
    QString sql = "SELECT id, catId, itemType FROM Nodes WHERE catId = '" + QString::number(catId) + "' and itemType = " + QString::number(ROOT_ELEMENT);
    
    q->exec(sql);
    //printQS(q->lastQuery());
    if (q->isActive())
    {
        q->first();
        if (q->isValid())        
        {
            result = q->value(0).toInt();
        }
    }
    delete q;
    return result;
}
*/
int dbNodeClass::isExistGroup(int catId, int parent, const QString &grpName)
{
    QSqlQuery *q = NULL;
    int result = -1;
    q = new QSqlQuery(db);
    if (!q)
        return result;

	QString sql = "SELECT id, parent, name, itemType FROM Catalog_" + QString::number(catId)+ " WHERE parent = '" + QString::number(parent) 
			+ "' and name = ? and itemType = " + QString::number(GROUP_ELEMENT);
    
	q->prepare(sql);
	q->addBindValue(grpName);
    q->exec();
    //printQS(q->lastQuery());
    if (q->isActive())
    {
        q->first();
        if (q->isValid())        
        {
            result = q->value(0).toInt();
            printQS("result = " + QString::number(result));
        }
    }
    delete q;
    return result;
}

int dbNodeClass::isExistDisk(int catId, int parent, const QString &diskName)
{
    QSqlQuery *q = NULL;
    int result = -1;
    q = new QSqlQuery(db);
    if (!q)
        return result;
    
	QString sql = "SELECT id, parent, name, itemType FROM Catalog_" + QString::number(catId)+ " WHERE parent = '" + QString::number(parent) 
			+ "' and name = ? and itemType = " + QString::number(DISK_ELEMENT);
    
	q->prepare(sql);
	q->addBindValue(diskName);
    q->exec();
    //printQS(q->lastQuery());
    if (q->isActive())
    {
        q->first();
        if (q->isValid())        
        {
            result = q->value(0).toInt();
        }
    }
    delete q;
    return result;
}

void dbNodeClass::prepareQueryForInsert(QSqlQuery * q, const dbNode & data)
{
	QString sql;
    if (data.nodeType == FILE_ELEMENT) 
    	sql = "INSERT INTO Files_" + QString::number(data.catId) +
			" (parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimetype, diskNo) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    else
        sql = "INSERT INTO Catalog_" + QString::number(data.catId) +
                " (parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimetype, diskNo) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        
	q->prepare(sql);
	q->addBindValue(data.parent);
	q->addBindValue(data.nodeType);
	q->addBindValue(data.nodeName);
	q->addBindValue(data.nodeSize);
	q->addBindValue(data.dateTime);
	q->addBindValue(data.otherNum);
	q->addBindValue(data.iconId);
	q->addBindValue(data.nodeDesc);
	q->addBindValue(data.nodeAttr);
	q->addBindValue(data.nodeOwner);
	q->addBindValue(data.nodeGrp);
	q->addBindValue(data.nodeFlags);
	q->addBindValue(data.nodeMimeType);
	q->addBindValue(data.diskNo);
	
}

void dbNodeClass::prepareQueryForUpdate(QSqlQuery * q, const dbNode & data)
{
    QString sql;
    if (data.nodeType == FILE_ELEMENT)
        sql = "UPDATE Files_" + QString::number(data.catId) + " SET "
                "parent = ?, itemType = ?, name = ?, size = ?, dateTime = ?, " 
                "otherNum = ?, iconId = ?, description = ?, attributes = ?, owner = ?, " 
                "grp = ?, flag = ?, mimeType = ?, diskNo = ? " 
                "WHERE id = '"		+  QString::number(data.id) + "'"; 
    else
		sql = "UPDATE Catalog_" + QString::number(data.catId) + " SET "
			"parent = ?, itemType = ?, name = ?, size = ?, dateTime = ?, " 
			"otherNum = ?, iconId = ?, description = ?, attributes = ?, owner = ?, " 
			"grp = ?, flag = ?, mimeType = ?, diskNo = ? " 
			"WHERE id = '"		+  QString::number(data.id) + "'"; 
	q->prepare(sql);
	q->addBindValue(data.parent);
	q->addBindValue(data.nodeType);
	q->addBindValue(data.nodeName);
	q->addBindValue(data.nodeSize);
	q->addBindValue(data.dateTime);
	q->addBindValue(data.otherNum);
	q->addBindValue(data.iconId);
	q->addBindValue(data.nodeDesc);
	q->addBindValue(data.nodeAttr);
	q->addBindValue(data.nodeOwner);
	q->addBindValue(data.nodeGrp);
	q->addBindValue(data.nodeFlags);
	q->addBindValue(data.nodeMimeType);
	q->addBindValue(data.diskNo);
}

void dbNodeClass::makeDeleteSql(const dbNode &data, QString &sql)
{
    if (data.nodeType == FILE_ELEMENT)
    	sql = "DELETE FROM Files_" + QString::number(data.catId) + " WHERE id = '" + QString::number(data.id) + "'"; 
    else
        sql = "DELETE FROM Catalog_" + QString::number(data.catId) + " WHERE id = '" + QString::number(data.id) + "'"; 
}

int dbNodeClass::getNodeType(int catId, int _id, bool filesTable)
{
    int result = -1;   
    if (_id == 0)
    	return ROOT_ELEMENT;
    if (filesTable)
        return FILE_ELEMENT;
    
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return result;
    QString sql = "SELECT itemType FROM Catalog_" + QString::number(catId) + " WHERE id = '" + QString::number(_id) + "'";
	//printQS(sql);
    if(!q->exec(sql))
    {
        delete q;
        return result;
    }else{
        q->first();
        if (q->isValid())
        {
            result = q->value(0).toInt();   
        }
    }
    delete q;
    return result;
}

int dbNodeClass::getNodeId(int catId, int parent, const QString &name)
{
    int result = -1;
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return result;
    QString sql = "SELECT id FROM Catalog_" + QString::number(catId) + " WHERE parent='%1' and name = ?";
    sql = sql.arg(parent);
	q->prepare(sql);
	q->addBindValue(name);
    if (!q->exec())
    {
    	delete q;
        return result;
    }else{
    	q->first();
        if (q->isValid())
        {
            result =  q->value(0).toInt();    
        }
    }
    delete q;
    return result;

}

int dbNodeClass::getNextDiskNo(int catId)
{
	int result = -1;
	QString sql = "SELECT id, seq FROM DiskNumbers WHERE id = " + QString::number(catId);
	//printQS("getNextDiskNo -> " + sql);
	QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return result;
	if (!q->exec(sql))
	{
		delete q;
		return result;
	}else{
		q->first();
		if (q->isValid())
		{
			result = q->value(1).toInt();
			result++;
		}
	}
	delete q;
	return result;
}

bool dbNodeClass::setNextDiskNo(int catId, int diskNo)
{
	QString sql = "UPDATE DiskNumbers SET seq = '" + QString::number(diskNo) + "' WHERE id = " + QString::number(catId);
	return runSql(sql);

}


int dbNodeClass::createGroup(dbNode &data)
{
    QSqlQuery *q = NULL;
    int result = -1;
    int parentType = getNodeType(data.catId, data.parent, false);
    int grpId = 0;
    if (checkRelationship(parentType, GROUP_ELEMENT))
    {
        // Group can be created;
        q = new QSqlQuery(db);
        if (!q)
            return false;
        if ((grpId = isExistGroup(data.catId, data.parent, data.nodeName)) != -1)
        {
            //printQS("Group for catalog " + QString::number(data.catId) + " exists with id = " + QString::number(grpId));
            delete q;
            return result;
        }
        QString sql;
        data.dateTime = QDateTime::currentDateTime().toTime_t();
        data.nodeType = GROUP_ELEMENT;
		data.diskNo = 0;
		data.otherNum = 0;
		prepareQueryForInsert(q, data);
        //makeInsertSql(data, sql);
        
        q->exec();
		//printQS(q->lastQuery());
		//printQS(q->executedQuery());
		if (q->isActive())
			{   
				result = true;
			}
        delete q;
        
    }
    return result;   
}

int dbNodeClass::createDisk(dbNode &data)
{
    QSqlQuery *q = NULL;
    int result = -1;
    int parentType = getNodeType(data.catId, data.parent, false);
    int diskId = 0;
    if (checkRelationship(parentType, DISK_ELEMENT))
    {
        // Disk can be created;
        //printQS("relationship passed");
        q = new QSqlQuery(db);
        if (!q)
            return false;
        if ((diskId = isExistDisk(data.catId, data.parent, data.nodeName)) != -1)
        {
            //printQS("Disk for catalog " + QString::number(data.catId) + " exists with id = " + QString::number(diskId));
            delete q;
            return result;
        }
        QString sql;
        data.dateTime = QDateTime::currentDateTime().toTime_t();
        data.nodeType = DISK_ELEMENT;
		data.diskNo = getNextDiskNo(data.catId);
		
		prepareQueryForInsert(q, data);
		//makeInsertSql(data, sql);
        
        q->exec();
        if (q->isActive())
        {   
            result = true;
        }
        delete q;
		setNextDiskNo(data.catId, data.diskNo);
        
    }
    return result;   
    
}

int dbNodeClass::createNode(dbNode &data)
{
    //TODO: get id 
    QSqlQuery *q = NULL;
    int result = -1;
    int parentType = getNodeType(data.catId, data.parent, false
                                );
    if (checkRelationship(parentType, data.nodeType))
    {
        // Node can be created;
        q = new QSqlQuery(db);
        if (!q)
            return false;
        QString sql;
		prepareQueryForInsert(q, data);
        
        if (!q->exec())
			printQS("Fail on " + data.nodeName);
        if (q->isActive())
        {   
            result = true;
        }
        delete q;
        
    }
    return result;   
}

bool dbNodeClass::checkRelationship(int parentType, int childType)
{
    bool result = false;
    switch(childType)
    {
        case GROUP_ELEMENT:
            if ((parentType == ROOT_ELEMENT) || (parentType == GROUP_ELEMENT))
                result = true;
            break;
        case DISK_ELEMENT:
            if ((parentType == ROOT_ELEMENT) || (parentType == GROUP_ELEMENT))
                result = true;
            break;
        case FOLDER_ELEMENT:
            if ((parentType == DISK_ELEMENT) || (parentType == FOLDER_ELEMENT))
                result = true;
            break;
        case FILE_ELEMENT:
        case ARCHIVE_ELEMENT:
            if ((parentType == DISK_ELEMENT) || (parentType == FOLDER_ELEMENT))
                result = true;
            break;
        default:
            result = false;
    }
    return result;
}

bool dbNodeClass::updateNode(const dbNode &data)
{
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return false;
	prepareQueryForUpdate(q, data);
	if (q->exec())
    {
        delete q;
        return true;   
    }
    delete q;
    return false;
    
}

bool dbNodeClass::updateGroup(const dbNode &data)
{
    return updateNode(data);
}

bool dbNodeClass::updateDisk(const dbNode &data)
{
    return updateNode(data);
}

bool dbNodeClass::renameNode(int _catId, int _id, const QString &name)
{
    QString sql = "UPDATE Catalog_" + QString::number(_catId) + " SET name = ? WHERE id = '" + QString::number(_id) + "'";
    
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return false;
    q->prepare(sql);
    q->addBindValue(name);
    if (q->exec())
    {
        delete q;
        return true;
    }
    delete q;
    return false;
    
}

bool dbNodeClass::renameFile(int _catId, int _id, const QString &name)
{
    QString sql = "UPDATE Files_" + QString::number(_catId) + " SET name = ? WHERE id = '" + QString::number(_id) + "'";
    
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return false;
    q->prepare(sql);
    q->addBindValue(name);
    if (q->exec())
    {
        delete q;
        return true;
    }
    delete q;
    return false;
    
}

bool dbNodeClass::deleteNode(const dbNode &data)
{
    QString sql;
    makeDeleteSql(data, sql);
	return runSql(sql);
}

bool dbNodeClass::deleteDisk(const dbNode &data)
{
    QString sql = "DELETE FROM Catalog_" + QString::number(data.catId) + " WHERE DiskNo = '" + QString::number(data.diskNo) + "'";
	bool catRes = runSql(sql);
    sql = "DELETE FROM Files_" + QString::number(data.catId) + " WHERE DiskNo = '" + QString::number(data.diskNo) + "'";
    bool filesRes = runSql(sql);
    
    return (catRes && filesRes);
}

int dbNodeClass::getSuitableParentForScan(int catId, int id)
{
	dbNode data;
	if (readNode(catId, id, data))
	{
		int nodeType = data.nodeType;
		switch (nodeType)
		{
			case ROOT_ELEMENT:
			case GROUP_ELEMENT:
				return data.id;
				break;
			case DISK_ELEMENT:
			case FOLDER_ELEMENT:
				return getSuitableParentForScan(catId, data.parent);
		}
	}else{
		return -1;
	}
}

bool dbNodeClass::readNode(int catId, int id, dbNode &data)
{
    QString sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Catalog_" + QString::number(catId) + " WHERE id = " + QString::number(id);
        
	bool result = false;
	QSqlQuery *q = new QSqlQuery(db);
	if (!q)
		return false;
	if (q->exec(sql))
	{
		q->first();
		data.id 			= q->value(0).toInt();
		data.catId 			= catId;      
		data.parent 		= q->value(1).toInt();
		data.nodeType 		= q->value(2).toInt();
		data.nodeName 		= q->value(3).toString();
		data.nodeSize 		= q->value(4).toLongLong();
		data.dateTime 		= q->value(5).toInt();
		data.otherNum 		= q->value(6).toInt();
		data.iconId 		= q->value(7).toInt();
		data.nodeDesc 		= q->value(8).toString();
		data.nodeAttr 		= q->value(9).toInt();
		data.nodeOwner 		= q->value(10).toString();
		data.nodeGrp 		= q->value(11).toString();
		data.nodeFlags 		= q->value(12).toInt();
		data.nodeMimeType 	= q->value(13).toString();
		data.diskNo			= q->value(14).toInt();
		result = true;
	}
	delete q;
	return result;
	
}

bool dbNodeClass::readFile(int catId, int id, dbNode &data)
{
    QString sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Files_" + QString::number(catId) + " WHERE id = " + QString::number(id);
        
    bool result = false;
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return false;
    if (q->exec(sql))
    {
        q->first();
        data.id 			= q->value(0).toInt();
        data.catId 			= catId;      
        data.parent 		= q->value(1).toInt();
        data.nodeType 		= q->value(2).toInt();
        data.nodeName 		= q->value(3).toString();
        data.nodeSize 		= q->value(4).toLongLong();
        data.dateTime 		= q->value(5).toInt();
        data.otherNum 		= q->value(6).toInt();
        data.iconId 		= q->value(7).toInt();
        data.nodeDesc 		= q->value(8).toString();
        data.nodeAttr 		= q->value(9).toInt();
        data.nodeOwner 		= q->value(10).toString();
        data.nodeGrp 		= q->value(11).toString();
        data.nodeFlags 		= q->value(12).toInt();
        data.nodeMimeType 	= q->value(13).toString();
        data.diskNo			= q->value(14).toInt();
        result = true;
    }
    delete q;
    return result;
	
}

void dbNodeClass::readAllChildsForParent(int catId, int parent, QList<dbNode *> &data)
{
	QString sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Catalog_" + QString::number(catId) + " WHERE parent = '" + QString::number(parent) + "'";
	//printQS(sql);
	QSqlQuery *q = new QSqlQuery(db);
	if (q->exec(sql))
	{
		while (q->next())
		{
			dbNode *node = new dbNode;
			node->id 			= q->value(0).toInt();
			node->catId 		= catId;      
			node->parent 		= q->value(1).toInt();
			node->nodeType 		= q->value(2).toInt();
			node->nodeName 		= q->value(3).toString();
			node->nodeSize 		= q->value(4).toLongLong();
			node->dateTime 		= q->value(5).toInt();
			node->otherNum 		= q->value(6).toInt();
			node->iconId 		= q->value(7).toInt();
			node->nodeDesc 		= q->value(8).toString();
			node->nodeAttr 		= q->value(9).toInt();
			node->nodeOwner 	= q->value(10).toString();
			node->nodeGrp 		= q->value(11).toString();
			node->nodeFlags 	= q->value(12).toInt();
			node->nodeMimeType 	= q->value(13).toString();
			node->diskNo		= q->value(14).toInt();
			data.append(node);
		}
	}
	
    sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Files_" + QString::number(catId) + " WHERE parent = '" + QString::number(parent) + "'";
	//printQS(sql);
    if (q->exec(sql))
    {
        while (q->next())
        {
            dbNode *node = new dbNode;
            node->id 			= q->value(0).toInt();
            node->catId 		= catId;      
            node->parent 		= q->value(1).toInt();
            node->nodeType 		= q->value(2).toInt();
            node->nodeName 		= q->value(3).toString();
            node->nodeSize 		= q->value(4).toLongLong();
            node->dateTime 		= q->value(5).toInt();
            node->otherNum 		= q->value(6).toInt();
            node->iconId 		= q->value(7).toInt();
            node->nodeDesc 		= q->value(8).toString();
            node->nodeAttr 		= q->value(9).toInt();
            node->nodeOwner 	= q->value(10).toString();
            node->nodeGrp 		= q->value(11).toString();
            node->nodeFlags 	= q->value(12).toInt();
            node->nodeMimeType 	= q->value(13).toString();
            node->diskNo		= q->value(14).toInt();
            data.append(node);
        }
    }
    delete q;
    
}

QList<dbNode *> *dbNodeClass::readAllChildsForParent(int catId, int parent)
{
	QList<dbNode *> *pNodes = new QList<dbNode *>;
	QString sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Catalog_" + QString::number(catId) + " WHERE parent = '" + QString::number(parent) + "' ORDER BY name ASC";
	//printQS(sql);
    QSqlQuery *q = new QSqlQuery(db);
	if (q->exec(sql))
	{
		while (q->next())
		{
			dbNode *node = new dbNode;
			node->id 			= q->value(0).toInt();
			node->catId 		= catId;      
			node->parent 		= q->value(1).toInt();
			node->nodeType 		= q->value(2).toInt();
			node->nodeName 		= q->value(3).toString();
			node->nodeSize 		= q->value(4).toLongLong();
			node->dateTime 		= q->value(5).toInt();
			node->otherNum 		= q->value(6).toInt();
			node->iconId 		= q->value(7).toInt();
			node->nodeDesc 		= q->value(8).toString();
			node->nodeAttr 		= q->value(9).toInt();
			node->nodeOwner 	= q->value(10).toString();
			node->nodeGrp 		= q->value(11).toString();
			node->nodeFlags 	= q->value(12).toInt();
			node->nodeMimeType 	= q->value(13).toString();
			node->diskNo		= q->value(14).toInt();
			pNodes->append(node);
		}
	}
    sql = "SELECT id, parent, itemType, name, size, dateTime, otherNum, iconId, description, attributes, owner, grp, flag, mimeType, diskNo FROM Files_" + QString::number(catId) + " WHERE parent = '" + QString::number(parent) + "' ORDER BY name ASC";
	//printQS(sql);
    if (q->exec(sql))
    {
        while (q->next())
        {
            dbNode *node = new dbNode;
            node->id 			= q->value(0).toInt();
            node->catId 		= catId;      
            node->parent 		= q->value(1).toInt();
            node->nodeType 		= q->value(2).toInt();
            node->nodeName 		= q->value(3).toString();
            node->nodeSize 		= q->value(4).toLongLong();
            node->dateTime 		= q->value(5).toInt();
            node->otherNum 		= q->value(6).toInt();
            node->iconId 		= q->value(7).toInt();
            node->nodeDesc 		= q->value(8).toString();
            node->nodeAttr 		= q->value(9).toInt();
            node->nodeOwner 	= q->value(10).toString();
            node->nodeGrp 		= q->value(11).toString();
            node->nodeFlags 	= q->value(12).toInt();
            node->nodeMimeType 	= q->value(13).toString();
            node->diskNo		= q->value(14).toInt();
            pNodes->append(node);
        }
    }
    delete q;
    
	return pNodes;
}

void dbNodeClass::clearNodeList(QList<dbNode *> &pList)
{
    while (!pList.isEmpty())
        delete pList.takeFirst();
    
}

bool dbNodeClass::runSql(const QString &sql)
{
    QSqlQuery *q = new QSqlQuery(db);
    if (!q)
        return false;
    if (q->exec(sql))
    {
        delete q;
        return true;
    }
    delete q;
    return false;
    
}

bool dbNodeClass::updateNodeDescription(int _catId, int _id, const QString &newDescription)
{
    QString sql = "UPDATE Catalog_" + QString::number(_catId) + " SET description = '" + newDescription + "' WHERE id = '" + QString::number(_id) + "'";
	return runSql(sql);
}

bool dbNodeClass::updateNodeIcon(int _catId, int _id, int _iconId)
{
    QString sql = "UPDATE Catalog_" + QString::number(_catId) + " SET iconId = '" + QString::number(_iconId) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);
    
}

bool dbNodeClass::updateNodeIconAndDescription(int _catId, int _id, int _iconId, const QString &newDescription)
{
    QString sql = "UPDATE Catalog_" + QString::number(_catId) + " SET description = '" + newDescription + "', iconId = '" + QString::number(_iconId) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);

}

bool dbNodeClass::updateNodeParent(int _catId, int _id, int _newParent)
{
    QString sql = "UPDATE Catalog_" + QString::number(_catId) + " SET parent = '" + QString::number(_newParent) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);
}

bool dbNodeClass::updateFileDescription(int _catId, int _id, const QString &newDescription)
{
    QString sql = "UPDATE Files_" + QString::number(_catId) + " SET description = '" + newDescription + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);
}

bool dbNodeClass::updateFileIcon(int _catId, int _id, int _iconId)
{
    QString sql = "UPDATE Files_" + QString::number(_catId) + " SET iconId = '" + QString::number(_iconId) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);
    
}

bool dbNodeClass::updateFileIconAndDescription(int _catId, int _id, int _iconId, const QString &newDescription)
{
    QString sql = "UPDATE File_" + QString::number(_catId) + " SET description = '" + newDescription + "', iconId = '" + QString::number(_iconId) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);

}

bool dbNodeClass::updateFileParent(int _catId, int _id, int _newParent)
{
    QString sql = "UPDATE File_" + QString::number(_catId) + " SET parent = '" + QString::number(_newParent) + "' WHERE id = '" + QString::number(_id) + "'";
    return runSql(sql);
}


