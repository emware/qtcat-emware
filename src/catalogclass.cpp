/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nike@gmail.com   *
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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <QIcon>
#include <QSqlError>

#include "catalogclass.h"
#include "defsandtools.h"

/**
 * CatalogClass start
 * 
 * */

CatalogClass::CatalogClass(const QSqlDatabase &db)
{
    this->db = db;
}

CatalogClass::~CatalogClass()
{

}

void CatalogClass::initGivenData(catalogNode &data)
{	
	data.catId = 0;
	data.catName = "";
	data.catDescription = "";
	data.iconId = 0;
	data.catSize = 0;
	data.numFiles = 0;
	data.numDirs = 0;
	data.numGroups = 0;
	data.numCD = 0;
	data.numDVD = 0;
	data.numNetDrives = 0;
	data.numFlash = 0;
	data.numHDD = 0;
	data.numFDD = 0;
	data.catFlags = 0; 
	data.catReadPass = "";
	data.catWritePass = "";	
	data.isReadProtected = false;
	data.isWriteProtected = false;
    data.isReadUnlocked = false;
    data.isWriteUnlocked = false;
	data.isOpened = false;
    data.dateTime = 0;

}

int CatalogClass::createCatalog(catalogNode &data)
{
	QSqlQuery *q = NULL;
	int result = -1; // catalog is not created;
	if (data.catName.isEmpty())
		return result;
	q = new QSqlQuery(db);
	if (!q)
		return result;
	if (isExist(data.catName))
	{
		delete q;
		return result;
	}
	QString sql = "INSERT INTO Catalogs(name, description, iconId, catSize, numFiles, numDirs, numGroups, numCD, numDVD, numNetDrives, numFlash, numHDD, numFDD, flags, readPass, writePass, datetime) VALUES('" +
			data.catName + "', '" + data.catDescription + "', '"  +
			QString::number(data.iconId) + "', '" + QString::number(data.catSize)  + "', '" + QString::number(data.numFiles)  + "', '" + QString::number(data.numDirs)  + "', '" + QString::number(data.numGroups) + "', '" +  
			QString::number(data.numCD)  + "', '" + QString::number(data.numDVD) + "', '" + QString::number(data.numNetDrives) + "', '" + QString::number(data.numFlash) + "', '" + QString::number(data.numHDD) + "', '" + 
			QString::number(data.numFDD) + "', '" + QString::number(data.catFlags) + "', '" + data.catReadPass + "', '" + data.catWritePass + "', '" + QString::number(data.dateTime) + "')";
            
            
	//printQS(sql);
	q->exec(sql);
	if (!q->isActive()){
		delete q;
		return result;   
	}
	if (readCatalog(data.catName, data))
		result = data.catId;
	else
		result = 0;
	//printQS("Cat id = " + QString::number(result));
	createCatalogTable(data.catId);
	delete q;
	return result;
        
}
bool CatalogClass::readCatalog(QString _catalogName, catalogNode &data)
{
	QSqlQuery *q = NULL;
	bool result = false;
	QString sql = "SELECT * FROM Catalogs WHERE name = '%1'";
	sql = sql.arg(_catalogName);
	printQS(sql);
	q = new QSqlQuery(db);
	if (!q)
		return result;
	q->exec(sql);
	if (q->isActive())
	{
		q->next();
		data.catId 			= q->value(0).toInt();
		data.catName		= q->value(1).toString();
		data.catDescription = q->value(2).toString();
		data.iconId 		= q->value(3).toInt();
		data.catSize	 	= q->value(4).toInt();
		data.numFiles 		= q->value(5).toInt();
		data.numDirs 		= q->value(6).toInt();
		data.numGroups 		= q->value(7).toInt();
		data.numCD 			= q->value(8).toInt();
		data.numDVD 		= q->value(9).toInt();
		data.numNetDrives	= q->value(10).toInt();
		data.numFlash 		= q->value(11).toInt();
		data.numHDD 		= q->value(12).toInt();
		data.numFDD 		= q->value(13).toInt();
		data.catFlags 		= q->value(14).toInt();
		data.catReadPass 	= q->value(15).toString();
		data.catWritePass	= q->value(16).toString();
        data.dateTime		= q->value(17).toInt();
        
        data.isReadProtected 	= (data.catFlags & READ_PROTECTED) ? true : false;
        data.isWriteProtected	= (data.catFlags & WRITE_PROTECTED) ? true : false;
        data.isOpened			= (data.catFlags & CATALOG_OPENED) ? true : false;
        if (data.isReadProtected)
            data.isOpened = false;
        
		result = true;
	}
	delete q;
	return result;
}

bool CatalogClass::readCatalog(int _id, catalogNode &data)
{
	QSqlQuery *q = NULL;
	bool result = false;
	QString sql = "SELECT * FROM Catalogs WHERE id = '%1'";
	sql = sql.arg(_id);
	q = new QSqlQuery(db);
	if (!q)
		return result;
    
	q->exec(sql);
	if (q->isActive())
	{
        q->next();
        data.catId 			= q->value(0).toInt();
        data.catName		= q->value(1).toString();
        data.catDescription = q->value(2).toString();
        data.iconId 		= q->value(3).toInt();
        data.catSize	 	= q->value(4).toInt();
        data.numFiles 		= q->value(5).toInt();
        data.numDirs 		= q->value(6).toInt();
        data.numGroups 		= q->value(7).toInt();
        data.numCD 			= q->value(8).toInt();
        data.numDVD 		= q->value(9).toInt();
        data.numNetDrives	= q->value(10).toInt();
        data.numFlash 		= q->value(11).toInt();
        data.numHDD 		= q->value(12).toInt();
        data.numFDD 		= q->value(13).toInt();
        data.catFlags 		= q->value(14).toInt();
        data.catReadPass 	= q->value(15).toString();
        data.catWritePass	= q->value(16).toString();
        data.dateTime		= q->value(17).toInt();
      
        data.isReadProtected 	= (data.catFlags & READ_PROTECTED) ? true : false;
        data.isWriteProtected	= (data.catFlags & WRITE_PROTECTED) ? true : false;
        data.isOpened			= (data.catFlags & CATALOG_OPENED) ? true : false;
        if (data.isReadProtected)
            data.isOpened = false;
        
        result = true;
	}
	delete q;
	return result;
    
}

bool CatalogClass::updateCatalog(catalogNode &data)
{
	QSqlQuery *q = NULL;
	/*
	if (data.readPasswordProtected & !canWrite)
	return false;*/
	q = new QSqlQuery(db);
	if (!q)
		return false;
    
	QString sql = "UPDATE Catalogs SET name='" + data.catName + "', description='" + data.catDescription + "', iconId='" + QString::number(data.iconId) + "', catSize='"+ QString::number(data.catSize) + "', numFiles='"+ QString::number(data.numFiles) + "', numDirs='"+ QString::number(data.numDirs) + "', numGroups='"+ QString::number(data.numGroups) + "', numCD='"+ QString::number(data.numCD) + "', numDVD='"+ QString::number(data.numDVD) + "', numNetDrives='"+ QString::number(data.numNetDrives) + "', numFlash='"+ QString::number(data.numFlash) + "', numHDD='"+ QString::number(data.numHDD) + "', numFDD='"+ QString::number(data.numFDD) + "', flags='"+ QString::number(data.catFlags) + "', readPass='" + data.catReadPass + "', writePass='" + data.catWritePass + "', dateTime = '" + QString::number(data.dateTime) + "' WHERE id = '"+ QString::number(data.catId) + "'";
    
	q->exec(sql);
	if (!q->isValid())
	{   
		delete q;
		return false;
	}
	delete q;
	return true;
       
}

bool CatalogClass::deleteCatalog(int _id)
{
	QSqlQuery *q = NULL;
	QString sqlFromCatalogs = "DELETE FROM Catalogs WHERE id = " + QString::number(_id);
    QString sqlDropTable1 = "DROP TABLE IF EXISTS Catalog_" + QString::number(_id);
    QString sqlDropTable2 = "DROP TABLE IF EXISTS Files_" + QString::number(_id);
    QString sqlDeleteIcons = "DELETE FROM Icons WHERE catId = " + QString::number(_id);
	QString sqlDeleteDiskNum = "DELETE FROM DiskNumbers WHERE id = " + QString::number(_id);
	QString sqlDropIndex1 = "DROP INDEX IF EXISTS Catalog_" + QString::number(_id) + "_Index_1";
	QString sqlDropIndex2 = "DROP INDEX IF EXISTS Catalog_" + QString::number(_id) + "_Index_2";
	QString sqlDropIndex3 = "DROP INDEX IF EXISTS Catalog_" + QString::number(_id) + "_Index_3";

    QString sqlDropIndex4 = "DROP INDEX IF EXISTS Files_" + QString::number(_id) + "_Index_1";
    QString sqlDropIndex5 = "DROP INDEX IF EXISTS Files_" + QString::number(_id) + "_Index_2";

	q = new QSqlQuery(db);
	if (!q)
		return false;
	q->exec(sqlFromCatalogs);
    // Delete from Catalog Table
	if (!q->isActive())
	{   
		delete q;
		return false;
	}
    // Delete Indexes
    q->exec(sqlDropIndex1);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    q->exec(sqlDropIndex2);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    q->exec(sqlDropIndex3);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    q->exec(sqlDropIndex4);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    q->exec(sqlDropIndex5);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
	// Delete tables
    q->exec(sqlDropTable1);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    q->exec(sqlDropTable2);
    if (!q->isActive())
    {   
        delete q;
        return false;
    }
    // Delete Icons
    q->exec(sqlDeleteIcons);
	if (!q->isActive())
	{   
		delete q;
		return false;
	}
    // Delete Disk Num
	q->exec(sqlDeleteDiskNum);
	if (!q->isActive())
	{   
		delete q;
		return false;
	}
	delete q;
	return true;
	
}

bool CatalogClass::authRead(catalogNode &data, QString userPass)
{
	if (data.catReadPass == userPass)
        data.isReadUnlocked = true;
    else
        data.isReadUnlocked = false;
    
    return data.isReadUnlocked;
}

bool CatalogClass::authWrite(catalogNode &data, QString userPass)
{
	if (data.catWritePass == userPass)
		data.isWriteUnlocked = true;
	else
        data.isWriteUnlocked = false;
    
    return data.isWriteUnlocked;
    
}

bool CatalogClass::isExist(QString _catalogName)
{
	QSqlQuery *q = NULL;
	bool result = false;
	q = new QSqlQuery(db);
	if (!q)
		return result;
	QString sql = "SELECT id, name FROM Catalogs Where name='%1'";
	sql = sql.arg(_catalogName);
            
	q->exec(sql);
	if (q->isActive())
	{
		q->first();
/*        if (q->isValid())
		printQS(QString("Catalog with name %1 exists").arg(_catalogName));*/
		result = q->isValid();
	}
	delete q;
	return result;
}

bool CatalogClass::createCatalogTable(int catId)
{
	QString sqlDrop = "DROP TABLE IF EXIST Catalog_" + QString::number(catId);
	
	QString sqlCreateCat = "CREATE TABLE Catalog_" + QString::number(catId) + "(id INTEGER PRIMARY KEY AUTOINCREMENT, parent INT, itemType INT, name TEXT, size INT(8), dateTime INT, otherNum INT, iconId INT, description TEXT, attributes INT, owner TEXT, grp TEXT, flag INT, mimeType TEXT, diskNo INT)";
    
    QString sqlIndex1Cat = "CREATE INDEX Catalog_" + QString::number(catId) + "_Index_1 ON Catalog_" + QString::number(catId) + "(itemType ASC)";
    QString sqlIndex2Cat = "CREATE INDEX Catalog_" + QString::number(catId) + "_Index_2 ON Catalog_" + QString::number(catId) + "(diskNo ASC)";
    QString sqlIndex3Cat = "CREATE INDEX Catalog_" + QString::number(catId) + "_Index_3 ON Catalog_" + QString::number(catId) + "(parent ASC)";
	
    QString sqlCreateFiles = "CREATE TABLE Files_" + QString::number(catId) + "(id INTEGER PRIMARY KEY AUTOINCREMENT, parent INT, itemType INT, name TEXT, size INT(8), dateTime INT, otherNum INT, iconId INT, description TEXT, attributes INT, owner TEXT, grp TEXT, flag INT, mimeType TEXT, diskNo INT)";
    
    QString sqlIndex1Files = "CREATE INDEX Files_" + QString::number(catId) + "_Index_1 ON Files_" + QString::number(catId) + "(diskNo ASC)";
    QString sqlIndex2Files = "CREATE INDEX Files_" + QString::number(catId) + "_Index_2 ON Files_" + QString::number(catId) + "(parent ASC)";
 
	QString sqlDiskNo = "INSERT INTO DiskNumbers (id, seq) VALUES ('" + QString::number(catId) + "', 0)";
	
	QSqlQuery *q = NULL;
	q = new QSqlQuery(db);
	if (!q)
		return false;
    
	q->exec(sqlDrop);
    q->exec(sqlCreateCat);
    q->exec(sqlIndex1Cat);
    q->exec(sqlIndex2Cat);
    q->exec(sqlIndex3Cat);
    
    q->exec(sqlCreateFiles);
    q->exec(sqlIndex1Files);
    q->exec(sqlIndex2Files);
	
	q->exec(sqlDiskNo);
	
	delete q;
	return true;

}


int CatalogClass::getCatalogId(const QSqlDatabase &_db, const QString &catName)
{
	int result = -1;
	QSqlQuery *q = NULL;
	q = new QSqlQuery(_db);
        
	QString sql = "SELECT id, name FROM Catalogs WHERE name = '" + catName + "'";
    
	q->exec(sql);
	if (q->isActive())
	{
		q->first();
		if (q->isValid())
			result = q->value(0).toInt();   
	}
	delete q;
	return result;
}

bool CatalogClass::renameCatalog(int _id, const QString &newName)
{
	bool result = false;
	QString sql = "UPDATE Catalogs SET name = '" + newName + "' WHERE id = '" + QString::number(_id) + "'";
	QSqlQuery *q = new QSqlQuery(db);
	if (q->exec(sql))
		result = true;
	delete q;
	return result;
}

bool CatalogClass::readAllCatalogs(QList<catalogNode *> &catalogItems)
{
	QString sql = "SELECT id, name, description, iconId, catSize, numFiles, numDirs, numGroups, numCD, numDVD, numNetDrives, numFlash, numHDD, numFDD, flags, readPass, writePass, dateTime FROM Catalogs";
	QSqlQuery *q = new QSqlQuery(db);
	
	if (!q->exec(sql))
	{
		delete q;
		return false;
		
	}else{
		while (q->next())
		{
			catalogNode *pCat	 	= new catalogNode;
			pCat->catId 			= q->value(0).toInt();
			pCat->catName			= q->value(1).toString();
			pCat->catDescription	= q->value(2).toString();
			pCat->iconId 			= q->value(3).toInt();
			pCat->catSize 			= q->value(4).toInt();
			pCat->numFiles 			= q->value(5).toInt();
			pCat->numDirs 			= q->value(6).toInt();
			pCat->numGroups 		= q->value(7).toInt();
			pCat->numCD 			= q->value(8).toInt();
			pCat->numDVD 			= q->value(9).toInt();
			pCat->numNetDrives		= q->value(10).toInt();
			pCat->numFlash 			= q->value(11).toInt();
			pCat->numHDD 			= q->value(12).toInt();
			pCat->numFDD 			= q->value(13).toInt();
			pCat->catFlags 			= q->value(14).toInt();
			pCat->catReadPass 		= q->value(15).toString();
			pCat->catWritePass 		= q->value(16).toString();
            pCat->dateTime			= q->value(17).toInt();
			
			pCat->isReadProtected	= (pCat->catFlags & READ_PROTECTED) ? true : false;
			pCat->isWriteProtected	= (pCat->catFlags & WRITE_PROTECTED) ? true : false;
			pCat->isOpened			= (pCat->catFlags & CATALOG_OPENED)	? true : false;
			if (pCat->isReadProtected)
				pCat->isOpened = false;
            
			pCat->isReadUnlocked 	= false;
			pCat->isWriteUnlocked	= false;

			catalogItems.append(pCat);
		}
		
	}
	delete q;
	return true;

}

void CatalogClass::readAllCatalogs()
{
	clearCatalogsList(items);
	readAllCatalogs(items);
}

void CatalogClass::clearCatalogsList()
{
    clearCatalogsList(items);
}

void CatalogClass::clearCatalogsList(QList<catalogNode *> &list)
{
	while (!list.isEmpty())
		delete list.takeFirst();
}

void CatalogClass::setPasswordFlags(catalogNode &data, bool rp, bool wp)
{
    if (data.catFlags & READ_PROTECTED)
	    data.catFlags = data.catFlags ^ READ_PROTECTED;
    if (data.catFlags & WRITE_PROTECTED)
    	data.catFlags = data.catFlags ^ WRITE_PROTECTED;
    if (rp)
        data.catFlags = data.catFlags | READ_PROTECTED;
    if (wp)
        data.catFlags = data.catFlags | WRITE_PROTECTED;
}

