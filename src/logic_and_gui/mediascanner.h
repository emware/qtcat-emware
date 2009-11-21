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

#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include <QSqlDatabase>
/**
	@author Victor Nike <victor.nike@gmail.com>
 */

class QString;
class QFileInfo;
class dbNodeClass;


class MediaScanner{
	public:
		MediaScanner(dbNodeClass *_dbn, int catID, int parent, QString pathToScan);

		~MediaScanner();
        
		void startScan(int _currentDiskNo);
		void scan(QString path, int parent);
		void setJustCount(bool _flag) { justCountFiles = _flag; }
		void setDirProgressFunc(void p(int)) { dirProgress = p; }
		void setFilesProgressFunc(void p(int)) { filesProgress = p; }
		void setCatalogAndParent(int catId, int parent);
		int getItemsNum() { return cntItems; }
        
	private:
		void (*dirProgress)(int _progress);
		void (*filesProgress)(int _progress);
		void putNodeInDb(const QFileInfo &fi, int parent);
		QSqlDatabase db;
		QString startPath;
		int catNo;
		int parent;
		int currentDiskNo;
		bool justCountFiles;
		int cntItems;
		dbNodeClass *dbn;
                
};

#endif
