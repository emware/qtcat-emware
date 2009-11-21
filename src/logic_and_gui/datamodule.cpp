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

//#include <QWidget>
#include <QString>
#include <QSqlQuery>
#include <QFileInfo>

#include "defsandtools.h"
#include "dbnode.h"
#include "iconmanager.h"
#include "catalogclass.h"
#include "configmanager.h"
#include "catalogreports.h"
#include "datamodule.h"

QCatDataModuleClass::QCatDataModuleClass()
{
	im = NULL;
	cat = NULL;
	rpt = NULL;
	dbn = NULL;
	dbConnName = "";
}

QCatDataModuleClass::QCatDataModuleClass(const QString fileName)
{
	bool fileExist = false;
    QFileInfo fi(fileName);
    
    fileExist = fi.exists();
    printQS(QString::number(fileExist));
	bool createNewDB = false;
	if (!fileExist)
	{
		QCatDataModuleClass::createDB(fileName, true);
		createNewDB = true;
	}
	
	dbConnName = fileName;
    
	db = QSqlDatabase::addDatabase(DBTYPE, dbConnName);
	db.setDatabaseName(fileName);
	if (!db.open())
	{
		errorCode = 1; // DB can not be opened
		errMsg = QObject::tr("DB %1 can not be opened.").arg(fileName);
		return;
	}
	dbn = new dbNodeClass(db);
	cat = new CatalogClass(db);
	rpt = new CatalogReports(db);
	im = new IconManager(db);
	if (createNewDB)
		im->fillDefaultIcons();
}

QCatDataModuleClass::~ QCatDataModuleClass()
{
	delete im;
	delete cat;
	delete rpt;
	delete dbn;
	db.close();
    QSqlDatabase::removeDatabase(dbConnName);
}

void QCatDataModuleClass::openDb(const QString fileName)
{
    delete im;
    delete cat;
    delete rpt;
    delete dbn;
	
	if (!dbConnName.isEmpty())
		QSqlDatabase::removeDatabase(dbConnName);

    dbConnName = fileName;
	//db = QSqlDatabase::addDatabase(DBTYPE, dbConnName);
	db = QSqlDatabase::addDatabase(DBTYPE);
	
	bool createNewDB = false;
	if (!QFileInfo(fileName).exists())
	{
        createDB(fileName, true);
		createNewDB = true;
	}
    db.setDatabaseName(fileName);
    if (!db.open())
    {
        errorCode = 1; // DB can not be opened
        errMsg = QObject::tr("DB %1 can not be opened.").arg(fileName);
        return;
    }
    dbn = new dbNodeClass(db);
    cat = new CatalogClass(db);
    rpt = new CatalogReports(db);
    im = new IconManager(db);
	if (createNewDB)
    {
		im->fillDefaultIcons();
        delete im;
        // Re-read global icons
        im = new IconManager(db);
    }
        

}

bool QCatDataModuleClass::isOk()
{
	return (errorCode == 0);	
}

bool QCatDataModuleClass::createDB(const QString fileName, bool overwrite)
{
	bool fileExist = QFileInfo(fileName).exists();
	if (!overwrite && fileExist)
		return false;
     
	QSqlQuery *wq;
    
	QString dropIconsTable = "DROP TABLE IF EXISTS Icons";
	QString crIconsTable = "CREATE TABLE Icons (id INTEGER PRIMARY KEY AUTOINCREMENT, catId INTEGER(2), name TEXT(255), fileName TEXT(255), iconData BLOB)";    
    
	QString dropNodeTypes = "DROP TABLE IF EXISTS nodeTypes";
	QString crNodeTypes = "CREATE TABLE NodeTypes (id INTEGER(4), nodeName TEXT(255), nodeIcon INT(4), PRIMARY KEY(id))";
    
	QString dropCatalogsTable = "DROP TABLE IF EXISTS Catalogs";
	QString crCatalogsTable = "CREATE TABLE Catalogs(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT(255), description TEXT(512), iconId INT, catSize INT, numFiles INT, numDirs INT, numGroups INT, numCD INT, numDVD INT, numNetDrives INT, numFlash INT, numHDD INT, numFDD INT, flags INT, readPass TEXT, writePass TEXT, datetime INT)";
    
	QString dropDiskNumbersTable = "DROP TABLE IF EXISTS DiskNumbers";
	QString crDiskNumbersTable = "CREATE TABLE DiskNumbers(id INTERGER, seq INTEGER)";
    
//	QSqlDatabase _db;
     //db = QSqlDatabase::addDatabase(DBTYPE, "AA");
	db.setDatabaseName(fileName);
	printQS(fileName);
	db.open();
    
	wq = new QSqlQuery(db);
	if (!wq->exec(dropIconsTable))
	{
		delete wq;
		return false;
	}
	if (!wq->exec(crIconsTable))
	{
		delete wq;
		return false;
	}
    if (!wq->exec(dropNodeTypes))
	{
		delete wq;
		return false;
	}
    if (!wq->exec(crNodeTypes))
	{
		delete wq;
		return false;
	}
	if (!wq->exec(dropCatalogsTable))
	{
		delete wq;
		return false;
	}
	if (!wq->exec(crCatalogsTable))
	{
		delete wq;
		return false;
	}
	if (!wq->exec(dropDiskNumbersTable))
	{
		delete wq;
		return false;
	}
	if (!wq->exec(crDiskNumbersTable))
	{
		delete wq;
		return false;
	}
	delete wq;
    
   // IconManager im(_db);
    //im.fillDefaultIcons();
    
	db.close();
	return true;
}


CatTreeItem::CatTreeItem(CatTreeItem * it) : QTreeWidgetItem(it)
{
}


CatTreeItem::~CatTreeItem()
{
}


bool CatTreeItem::contains(CatTreeItem * item)
{
	//printQS("---\nthis = " + text(0) + "\n find = " + item->text(0) + "\n---");
	if (this == item)
		return false;
	else
    	return findItem(this, item);
}

bool CatTreeItem::findItem(CatTreeItem * start, CatTreeItem * what)
{
    int childrenNum = start->childCount();
	for (int i = 0; i < childrenNum; i++)
	{
		CatTreeItem *_child = static_cast <CatTreeItem *> (start->child(i));
		if (_child == what)
				return true;
		else{
			bool res = false;
			if (_child->childCount() > 0)
				res = findItem(_child, what);
			if (res)
				return true;
		}
	}
	return false;
}

