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
#ifndef DATAMODULE_H
#define DATAMODULE_H

/**
	@author Victor Nike <victor.nike@gmail.com>
 */

#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QSqlDatabase>

class dbNodeClass;
class CatalogReports;
class IconManager;
class CatalogClass;
class ConfigManager;

class QCatDataModuleClass{
	public:
        QCatDataModuleClass();
		QCatDataModuleClass(const QString fileName);
		~QCatDataModuleClass();
		bool isOk();
        bool createDB(const QString fileName, bool overwrite);
        //static bool createDB(const QString fileName, bool overwrite);
        void openDb(const QString fileName);
		
	// Public Members
	public:
		dbNodeClass *dbn;
		CatalogClass *cat;
		CatalogReports *rpt;
		ConfigManager *settings;
		IconManager *im;
		QSqlDatabase db;
		QString dbConnName;
	private:
		int errorCode;
		QString errMsg;
};

class CatTreeItem : public QTreeWidgetItem{
    public:
        CatTreeItem(CatTreeItem *it = 0);
        virtual ~CatTreeItem();
        bool contains(CatTreeItem *item);
    public:
        // Catalog related
        int itemType;
        int indexOrCatId; // index if ROOT_ELEMENT, else catID number;
        
        int id;
        int parentId;
        int iconId;
        int fullType;
		bool childsLoaded;

    private:
        
        bool findItem(CatTreeItem *start, CatTreeItem *what);
        
};

class IconListItem : public QListWidgetItem {
    
    public:
        IconListItem(QListWidget *parent = 0, int type = Type) : QListWidgetItem(parent, type) {};
        //IconListItem(IconListItem *it) : QListWidgetItem(it) {};
    
    public:
        int iconId;
        QString iconFileName;
    
};

#endif
