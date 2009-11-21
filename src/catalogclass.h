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
#ifndef CATALOGCLASS_H
#define CATALOGCLASS_H

/**
	@author Victor Nike <victor.nike@gmail.com>
 */

#include <QList>
#include <QSqlDatabase>
#include <QString>


struct catalogNode;

class CatalogClass{
	public:
        // Constructors
		//CatalogClass();
		CatalogClass(const QSqlDatabase &db);
		//CatalogClass(QString dbConnectionName);
		//void init();
        
        // Destructor
		~CatalogClass();
        
        // Catalogs table related funcions
		void initGivenData(catalogNode &data);
        int createCatalog(catalogNode &data);
        bool readCatalog(int _id, catalogNode &data);
        bool readCatalog(QString _catalogName, catalogNode &data);
        bool updateCatalog(catalogNode &data);
		bool isExist(QString _catalogName);
		
		bool renameCatalog(int _id, const QString &newName);
		bool deleteCatalog(int _id);
        
        bool authRead(catalogNode &data, QString userPass);
        bool authWrite(catalogNode &data, QString userPass);
		bool createCatalogTable(int catId);
        
        void readAllCatalogs();
		void clearCatalogsList();
		static int getCatalogId(const QSqlDatabase &_db, const QString &catName);
        
        void setPasswordFlags(catalogNode &data, bool rp, bool wp);
        
        QList<catalogNode *> items;
        
	private:
        bool readAllCatalogs(QList<catalogNode *> &catalogItems);
        void clearCatalogsList(QList<catalogNode *> &list);
		
        QSqlDatabase db;
		QString dbConnName;

};
#endif
